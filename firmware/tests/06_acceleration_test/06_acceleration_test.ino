// ===========================================================
// TEST 06: Manual Speed / Acceleration Tuning
// ===========================================================
// Simple ramp test: accelerate up to TOP_SPEED, hold it for
// SPIN_SECONDS, then decelerate smoothly to a stop. Repeat.
//
// Edit the three knobs below, re-upload, and watch/listen. Use
// it to find the fastest speed + acceleration your 28BYJ-48 runs
// at without stalling. Good values can later be baked into the
// engine's acceleration table (generate_acceleration.py).
//
// Self-contained AccelStepper rig (simple speed/accel knobs).
//
// Serial: 115200 baud. Prints settings and per-run timing.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48
// Wiring:   coil pins IN1,IN2,IN3,IN4 = 6,7,8,9
// ===========================================================

#include <AccelStepper.h>

// ===========================================================
// ===============  TUNE THESE KNOBS  ========================
// ===========================================================

// TOP_SPEED — steps per second the motor ramps up to.
//   HIGHER = FASTER spin (e.g. 900, 1000, 1200)
//   LOWER  = SLOWER spin (e.g. 400, 600)
//   28BYJ-48 at 5V tops out around ~1000-1100 steps/s before stalling.
float TOP_SPEED = 800.0;

// ACCELERATION — how fast it ramps up/down, in steps/sec^2.
//   HIGHER = reaches top speed FASTER (snappier; too high = stall on start)
//   LOWER  = ramps up/down SLOWER and gentler
//   Typical: 200 (gentle) .. 2000 (aggressive).
float ACCELERATION = 400.0;

// SPIN_SECONDS — how long to hold TOP_SPEED before decelerating to a stop.
float SPIN_SECONDS = 5.0;

// REVERSE — direction of spin (does not affect speed).
//   false = forward, true = backward
const bool REVERSE = false;

// ===========================================================

int  PAUSE_MS        = 1500;   // pause between runs
const long SERIAL_BAUD = 115200;

// --- PINS (IN1,IN3,IN2,IN4 order for correct 28BYJ-48 coil sequence) ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int cycleCount = 0;

void powerOff() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);

    stepper.setMaxSpeed(TOP_SPEED);
    stepper.setAcceleration(ACCELERATION);

    Serial.println(F("=== MANUAL SPEED / ACCEL TUNING ==="));
    Serial.print(F("Baud:         ")); Serial.println(SERIAL_BAUD);
    Serial.print(F("TOP_SPEED:    ")); Serial.print(TOP_SPEED);    Serial.println(F(" steps/s   (higher = faster)"));
    Serial.print(F("ACCELERATION: ")); Serial.print(ACCELERATION); Serial.println(F(" steps/s^2 (higher = ramps faster)"));
    Serial.print(F("SPIN_SECONDS: ")); Serial.print(SPIN_SECONDS); Serial.println(F(" s at top speed"));
    Serial.print(F("Direction:    ")); Serial.println(REVERSE ? F("REVERSE (backward)") : F("forward"));
    Serial.println(F("Edit knobs at top, re-upload to tune."));
    Serial.println();
}

void loop() {
    cycleCount++;
    int dir = REVERSE ? -1 : 1;

    Serial.print(F("Cycle ")); Serial.print(cycleCount);
    Serial.print(F(" — ramp up, hold ")); Serial.print(SPIN_SECONDS); Serial.println(F(" s, decel to stop..."));

    unsigned long start = millis();
    unsigned long durationMs = (unsigned long)(SPIN_SECONDS * 1000.0);

    // Phase 1: one far-away target so AccelStepper ramps up to TOP_SPEED
    // and HOLDS it (it only decelerates near a target). Run for the timer.
    stepper.setCurrentPosition(0);
    stepper.move(dir * 1000000L);   // effectively "spin" during the hold phase
    while (millis() - start < durationMs) {
        stepper.run();
    }

    // Phase 2: decelerate smoothly to a stop from full speed.
    stepper.stop();                 // sets a new target that respects ACCELERATION
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }

    unsigned long elapsed = millis() - start;
    long moved = stepper.currentPosition();
    long stepsTaken = moved >= 0 ? moved : -moved;
    float avgSpeed = (elapsed > 0) ? (1000.0 * stepsTaken / elapsed) : 0;

    Serial.print(F("  spun ")); Serial.print(stepsTaken);
    Serial.print(F(" steps in ")); Serial.print(elapsed);
    Serial.print(F(" ms  (avg ")); Serial.print(avgSpeed); Serial.println(F(" steps/s)"));

    powerOff();        // de-energize between runs (prevents heating)
    delay(PAUSE_MS);
}
