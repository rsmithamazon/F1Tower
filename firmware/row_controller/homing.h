// ===========================================================
// HOMING — Hall effect sensor homing sequence
// ===========================================================
// Each motor has a hall effect sensor that triggers when a
// magnet on the drum passes over it. Magnet is at flap 0 (black).
// Homing spins the motor forward slowly until sensor triggers,
// then sets position to 0.
// ===========================================================

#ifndef HOMING_H
#define HOMING_H

#include <stdint.h>

#define NUM_MOTORS      5
#define HOMING_SPEED_US 3000    // Slower step interval during homing (3ms for reliability)
#define MAX_HOMING_STEPS 2100   // Slightly more than one full revolution (safety limit)

// --- HALL SENSOR PINS ---
// GP0: Hall sensor motor 0 (Col 0)
// GP1: Hall sensor motor 1 (Col 1)
// GP2: Hall sensor motor 2 (Col 2)
// GP3: Hall sensor motor 3 (Col 3)
// GP4: (used by I2C) — Hall sensor motor 4 on GP28 or ADC pin
// Actual pin assignments TBD based on final PCB/wiring layout.

// --- INITIALIZATION ---

void initHallSensors();
// Configure hall sensor pins as INPUT_PULLUP.
// Hall sensors output LOW when magnet is detected (active low).

// --- HOMING SEQUENCE ---

void homeAllMotors();
// Home all 5 motors in parallel.
//
// Logic (per motor, all run concurrently):
//   1. Step forward slowly (HOMING_SPEED_US between steps)
//   2. Read hall sensor each step
//   3. When sensor triggers (LOW): stop, set position to flap 0
//   4. If MAX_HOMING_STEPS reached without trigger: flag error
//
// Uses PIO at reduced speed, or software stepping for more control.
// All 5 run simultaneously — each stops independently when its sensor triggers.

void homeSingleMotor(uint8_t motor);
// Home one specific motor. Used for error recovery.

// --- STATUS ---

bool isHomed(uint8_t motor);
// Return true if this motor has been successfully homed.

bool areAllHomed();
// Return true if all 5 motors are homed.

bool isHallTriggered(uint8_t motor);
// Read current state of hall sensor for a motor.
// Returns true if magnet is currently over sensor.

// --- ERROR ---

bool hasHomingError(uint8_t motor);
// Return true if motor failed to home (exceeded MAX_HOMING_STEPS).

#endif
