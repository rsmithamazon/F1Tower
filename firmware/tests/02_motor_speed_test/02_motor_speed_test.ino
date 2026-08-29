// ===========================================================
// TEST 02: Motor Speed / Acceleration Sanity Test
// ===========================================================
// Runs the real FlapMotor acceleration engine (motor_driver.h)
// through repeated full/partial sweeps and times them. Use this
// to confirm the motor spins smoothly and reliably at the speeds
// defined by the acceleration table.
//
// To change top speed / ramp, edit generate_acceleration.py and
// regenerate acceleration.h — the engine reads that table. This
// replaces the old AccelStepper-based tuning sketch so you're
// testing the actual firmware path.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48
// Wiring:   motor 0 coil pins = 6,7,8,9
// ===========================================================

#define NUM_MOTORS 1
#include <motor_driver.h>                 // from firmware/libraries/FlapMotor

int   STEPS_TO_MOVE   = 512;    // how far to sweep each cycle (steps)
int   PAUSE_BETWEEN_MS = 2000;

int cycleCount = 0;

// Move a raw number of steps forward through the engine (no homing
// needed — we treat wherever we are as the start and just sweep).
void sweepSteps(int steps) {
    // Convert a step count into a flap target relative to current.
    // The engine works in flaps, so approximate: steps -> flaps.
    uint8_t startFlap = getCurrentFlap(0);
    uint8_t flapsToMove = (uint8_t)((long)steps * FLAPS_PER_REV / STEPS_PER_REV);
    uint8_t target = (uint8_t)((startFlap + flapsToMove) % FLAPS_PER_REV);
    motor(0).goToFlap(target);
    while (motor(0).isMoving() && !motor(0).isError()) {
        motor(0).update();
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("=== MOTOR SPEED / ACCEL SANITY TEST ===");
    Serial.print("Flaps/rev: "); Serial.println(FLAPS_PER_REV);
    Serial.print("Top-speed period: ");
    Serial.print(Acceleration::ACCEL_STEP_PERIODS[Acceleration::MAX_ACCEL_STEP]);
    Serial.println(" us/step");
    Serial.println("");

    initMotorDriver();
    // No hall sensor required for this test — assume start = flap 0.
    motor(0).setHomed();

    Serial.println("Motor will sweep, pause, repeat. Watch for smooth motion.");
    Serial.println("");
}

void loop() {
    cycleCount++;
    Serial.print("Cycle "); Serial.print(cycleCount);
    Serial.print(" — sweeping ~"); Serial.print(STEPS_TO_MOVE); Serial.println(" steps...");

    unsigned long startTime = millis();
    sweepSteps(STEPS_TO_MOVE);
    unsigned long elapsed = millis() - startTime;

    Serial.print("  Done in "); Serial.print(elapsed);
    Serial.print(" ms, now at flap "); Serial.println(getCurrentFlap(0));

    motor(0).powerOff();
    delay(PAUSE_BETWEEN_MS);
}
