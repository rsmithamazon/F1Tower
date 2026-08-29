// ===========================================================
// MOTOR DRIVER — Accelerated 28BYJ-48 stepper control
// ===========================================================
// Software-timed half-step driver with trapezoidal acceleration.
//
// Acceleration model (adapted from scottbez1/splitflap, Apache-2.0):
//   - A lookup table (acceleration.h) maps an "accel step" index
//     to a step *period* in microseconds. Index 0 = idle, higher
//     index = faster.
//   - Each motor ramps current_accel_step toward a target that is
//     capped by delta_steps (distance remaining). So as the motor
//     nears its target, target_accel_step shrinks and the motor
//     decelerates. One table => smooth accel AND decel.
//   - update() is non-blocking: it only steps when the current
//     period has elapsed (micros()), so all 5 motors advance
//     independently from a single loop.
//
// Drum constraint: motion is FORWARD ONLY. To reach a flap that is
// "behind" the current position, we go forward and wrap around.
// ===========================================================

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>
#include <stdint.h>
#include "acceleration.h"

// NUM_MOTORS defaults to 5 (a full Pico row controller). Test sketches
// running on a single-motor Arduino rig can build with fewer motors by
// defining NUM_MOTORS before including this header (e.g. `#define NUM_MOTORS 1`).
#ifndef NUM_MOTORS
#define NUM_MOTORS         5
#endif
#define STEPS_PER_REV      2048    // 28BYJ-48 half-step, one full revolution
#define FLAPS_PER_REV      52      // Canonical flap count (flap_allocation_v4.md)
#define POS_UNKNOWN        0xFF    // Position not yet homed/known

// Aggregate status codes (returned by getMotorStatus()).
#define STATUS_IDLE        0
#define STATUS_MOVING      1
#define STATUS_ERROR       2

// If a commanded move hasn't finished in this long, flag a stall.
#define STALL_TIMEOUT_MS   5000

// --- MOTOR DIRECTION ---
// The drum is mechanically forward-only, but "forward" depends on how the
// coils are wired. Set REVERSE_DIRECTION to true to flip the physical spin
// without rewiring (define it before including this header to override).
// All flap/position logic is unchanged — only the coil phase order flips.
#ifndef REVERSE_DIRECTION
#define REVERSE_DIRECTION false
#endif

// --- HALF-STEP PHASE PATTERN (28BYJ-48 via ULN2003) ---
// 8-entry half-step sequence over the 4 coil outputs (IN1..IN4).
// Bit0=IN1, Bit1=IN2, Bit2=IN3, Bit3=IN4.
static const uint8_t HALFSTEP_PATTERN[8] = {
    0b0001, 0b0011, 0b0010, 0b0110,
    0b0100, 0b1100, 0b1000, 0b1001,
};

// Map the logical phase index (always increments forward) to an actual
// pattern index, reversed when REVERSE_DIRECTION is set.
static inline uint8_t phaseToPattern(uint8_t phase) {
#if REVERSE_DIRECTION
    return (uint8_t)((8 - phase) & 0x07);
#else
    return phase;
#endif
}

// --- MOTOR PIN MAP (4 coil pins per motor) ---
// Always defines 5 rows (the full Pico row controller wiring). Only the
// first NUM_MOTORS rows are used, so a 1-motor test build ignores the rest.
// Single-motor test rigs use motor 0 = pins 6,7,8,9 (matches the other tests).
// Adjust the later rows to the final Pico PCB wiring.
#define MAX_MOTORS 5
static const uint8_t MOTOR_PINS[MAX_MOTORS][4] = {
    {  6,  7,  8,  9 },   // Motor 0 (Col 0)
    { 10, 11, 12, 13 },   // Motor 1 (Col 1)
    { 14, 15, 16, 17 },   // Motor 2 (Col 2)
    { 18, 19, 20, 21 },   // Motor 3 (Col 3)
    { 22, 26, 27, 28 },   // Motor 4 (Col 4)
};

// ===========================================================
// Per-motor state + acceleration engine
// ===========================================================
class FlapMotor {
  public:
    void begin(uint8_t index) {
        _index = index;
        for (uint8_t i = 0; i < 4; i++) {
            pinMode(MOTOR_PINS[index][i], OUTPUT);
        }
        _setCoils(0);
        _currentStep = 0;
        _deltaSteps = 0;
        _accelStep = 0;
        _phase = 0;
        _period = Acceleration::ACCEL_STEP_PERIODS[0];
        _lastStepMicros = micros();
        _known = false;
        _error = false;
    }

    // Establish a known reference (e.g. after homing): we are at
    // flap 0 / step 0. Motion targets become meaningful after this.
    void setHomed() {
        _currentStep = 0;
        _deltaSteps = 0;
        _accelStep = 0;
        _known = true;
        _error = false;
    }

    // Force the tracked flap without moving (e.g. restored from flash).
    void setKnownFlap(uint8_t flap) {
        if (flap >= FLAPS_PER_REV) { _known = false; return; }
        _currentStep = _stepForFlap(flap);
        _deltaSteps = 0;
        _known = true;
    }

    bool isKnown() const { return _known; }
    bool isMoving() const { return _deltaSteps > 0 || _accelStep > 0; }
    bool isError()  const { return _error; }

    uint8_t currentFlap() const {
        if (!_known) return POS_UNKNOWN;
        // Floor division maps a step back to the flap it belongs to.
        return (uint8_t)(((uint32_t)_currentStep * FLAPS_PER_REV) / STEPS_PER_REV);
    }

    // Command a forward move to the given flap (0..44). Always
    // forward; wraps around the drum if the target is "behind".
    void goToFlap(uint8_t flap) {
        if (_error || !_known || flap >= FLAPS_PER_REV) return;
        uint16_t target = _stepForFlap(flap);

        // Can't stop instantly — reserve room to decelerate. If the
        // target is within our stopping distance, go around instead.
        uint16_t minStopStep = _currentStep + _accelStep;
        if (target <= minStopStep) {
            _deltaSteps = (uint16_t)(STEPS_PER_REV - _currentStep + target);
        } else {
            _deltaSteps = (uint16_t)(target - _currentStep);
        }
        _moveStartMs = millis();
    }

    // Non-blocking tick. Steps the motor if its period has elapsed
    // and updates the acceleration ramp. Call every loop iteration.
    void update() {
        if (_error) return;

        unsigned long now = micros();
        if ((now - _lastStepMicros) < _period) return;
        _lastStepMicros = now;

        // Target accel: ramp up when far, but never faster than the
        // distance left — this makes us coast down into the target.
        uint8_t targetAccel;
        if (_deltaSteps > Acceleration::MAX_ACCEL_STEP) {
            targetAccel = Acceleration::MAX_ACCEL_STEP;
        } else {
            targetAccel = (uint8_t)_deltaSteps;
        }

        if (_accelStep < targetAccel)       _accelStep++;
        else if (_accelStep > targetAccel)  _accelStep--;

        _period = Acceleration::ACCEL_STEP_PERIODS[_accelStep];

        if (_accelStep > 0) {
            // Take one half-step forward.
            _currentStep++;
            if (_currentStep >= STEPS_PER_REV) _currentStep = 0;
            _phase = (uint8_t)((_phase + 1) & 0x07);
            _setCoils(HALFSTEP_PATTERN[phaseToPattern(_phase)]);
            if (_deltaSteps > 0) _deltaSteps--;
        } else {
            // Idle: de-energize coils to stay cool and quiet.
            _setCoils(0);
            // Stall check only matters while a move is outstanding.
            if (_deltaSteps > 0 && (millis() - _moveStartMs) > STALL_TIMEOUT_MS) {
                _error = true;
                _deltaSteps = 0;
            }
        }
    }

    void powerOff() { _setCoils(0); }

    void clearError() { _error = false; }

    // --- HOMING SUPPORT ---
    // Take exactly one slow half-step forward, blocking for the given
    // period. Used by homing loops that step until the hall sensor
    // triggers. Does not touch the acceleration ramp or delta tracking.
    void homeStepForward(uint16_t periodMicros = 3000) {
        _phase = (uint8_t)((_phase + 1) & 0x07);
        _setCoils(HALFSTEP_PATTERN[phaseToPattern(_phase)]);
        _currentStep++;
        if (_currentStep >= STEPS_PER_REV) _currentStep = 0;
        delayMicroseconds(periodMicros);
    }

  private:
    // Round-up flap->step mapping so currentFlap() (floor) round-trips.
    uint16_t _stepForFlap(uint8_t flap) const {
        uint32_t intermediate = (uint32_t)flap * STEPS_PER_REV;
        uint16_t result = (uint16_t)(intermediate / FLAPS_PER_REV);
        if (intermediate % FLAPS_PER_REV != 0) result++;
        if (result >= STEPS_PER_REV) result -= STEPS_PER_REV;
        return result;
    }

    void _setCoils(uint8_t pattern) {
        for (uint8_t i = 0; i < 4; i++) {
            digitalWrite(MOTOR_PINS[_index][i], (pattern >> i) & 0x01);
        }
    }

    uint8_t  _index = 0;
    uint16_t _currentStep = 0;     // 0..STEPS_PER_REV-1
    uint16_t _deltaSteps = 0;      // steps remaining in current move
    uint8_t  _accelStep = 0;       // index into ACCEL_STEP_PERIODS
    uint16_t _period = 0;          // current step period (us)
    uint8_t  _phase = 0;           // 0..7 half-step phase
    unsigned long _lastStepMicros = 0;
    unsigned long _moveStartMs = 0;
    bool _known = false;
    bool _error = false;
};

// ===========================================================
// C-style API used by the rest of the row controller firmware
// ===========================================================

static FlapMotor _motors[NUM_MOTORS];

inline void initMotorDriver() {
    for (uint8_t m = 0; m < NUM_MOTORS; m++) _motors[m].begin(m);
}

inline void moveToFlap(uint8_t motor, uint8_t targetFlap) {
    if (motor < NUM_MOTORS) _motors[motor].goToFlap(targetFlap);
}

inline void moveAllToFlaps(uint8_t targets[NUM_MOTORS]) {
    for (uint8_t m = 0; m < NUM_MOTORS; m++) _motors[m].goToFlap(targets[m]);
}

inline void updateMotors() {
    for (uint8_t m = 0; m < NUM_MOTORS; m++) _motors[m].update();
}

inline bool isMotorAtTarget(uint8_t motor) {
    return motor < NUM_MOTORS ? !_motors[motor].isMoving() : true;
}

inline bool isMotorError(uint8_t motor) {
    return motor < NUM_MOTORS ? _motors[motor].isError() : false;
}

inline uint8_t getMotorStatus() {
    bool anyMoving = false;
    for (uint8_t m = 0; m < NUM_MOTORS; m++) {
        if (_motors[m].isError()) return STATUS_ERROR;
        if (_motors[m].isMoving()) anyMoving = true;
    }
    return anyMoving ? STATUS_MOVING : STATUS_IDLE;
}

inline uint8_t getCurrentFlap(uint8_t motor) {
    return motor < NUM_MOTORS ? _motors[motor].currentFlap() : POS_UNKNOWN;
}

inline void getAllPositions(uint8_t positions[NUM_MOTORS]) {
    for (uint8_t m = 0; m < NUM_MOTORS; m++) positions[m] = _motors[m].currentFlap();
}

inline void setCurrentFlap(uint8_t motor, uint8_t flap) {
    if (motor < NUM_MOTORS) _motors[motor].setKnownFlap(flap);
}

inline void setMotorHomed(uint8_t motor) {
    if (motor < NUM_MOTORS) _motors[motor].setHomed();
}

inline void clearMotorError(uint8_t motor) {
    if (motor < NUM_MOTORS) _motors[motor].clearError();
}

inline void clearAllErrors() {
    for (uint8_t m = 0; m < NUM_MOTORS; m++) _motors[m].clearError();
}

// Direct access to a motor (used by homing routines).
inline FlapMotor& motor(uint8_t index) { return _motors[index]; }

#endif
