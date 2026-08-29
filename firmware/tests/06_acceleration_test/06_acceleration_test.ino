// ===========================================================
// TEST 06: Acceleration Engine Test
// ===========================================================
// Exercises the table-driven acceleration in motor_driver.h on a
// single motor. Homes, then repeatedly jumps to a sequence of
// flaps so you can watch/listen for smooth ramp-up and ramp-down
// (no hard starts, no stalling) and confirm it lands on target.
//
// This uses the SAME FlapMotor acceleration engine the real Pico
// row controller runs — not a separate AccelStepper path. Arduino
// here is just the test rig; the identical code runs on the Pico.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Wiring:   motor 0 coil pins = 6,7,8,9  (see MOTOR_PINS)
//           hall sensor        = HALL_PIN below
// ===========================================================

#define NUM_MOTORS 1                      // single-motor test rig
#include <motor_driver.h>                 // from firmware/libraries/FlapMotor

// --- CONFIG ---
const int HALL_PIN       = 2;      // Hall sensor (active LOW = magnet at flap 0)
const int LED_PIN        = LED_BUILTIN;
const int MAX_HOME_STEPS = 2200;   // Safety limit (just over one revolution)
const int PAUSE_MS       = 1500;   // Pause at each target flap

// Sequence of flaps to visit after homing (mix of short + long moves).
const uint8_t SEQUENCE[] = { 1, 26, 8, 51, 17, 5, 40, 0 };
const int SEQUENCE_LEN   = sizeof(SEQUENCE) / sizeof(SEQUENCE[0]);

int seqIndex = 0;

bool isMagnetDetected() {
    bool detected = (digitalRead(HALL_PIN) == LOW);
    digitalWrite(LED_PIN, detected ? HIGH : LOW);
    return detected;
}

// Home motor 0: step forward slowly until the magnet is detected,
// then declare that position as flap 0 in the acceleration engine.
bool homeMotorZero() {
    Serial.print("Homing... ");
    int steps = 0;
    while (!isMagnetDetected()) {
        motor(0).homeStepForward(3000);   // slow, reliable homing speed
        steps++;
        if (steps > MAX_HOME_STEPS) {
            Serial.println("FAILED (magnet not found)");
            return false;
        }
    }
    motor(0).powerOff();
    motor(0).setHomed();
    Serial.print("OK ("); Serial.print(steps); Serial.println(" steps to home)");
    return true;
}

void goToFlapBlocking(uint8_t flap) {
    Serial.print("  -> flap "); Serial.print(flap);
    unsigned long start = millis();
    motor(0).goToFlap(flap);
    while (motor(0).isMoving() && !motor(0).isError()) {
        motor(0).update();
    }
    unsigned long elapsed = millis() - start;
    if (motor(0).isError()) {
        Serial.println("   STALL/ERROR!");
        motor(0).clearError();
    } else {
        Serial.print("   landed on ");
        Serial.print(getCurrentFlap(0));
        Serial.print(" in "); Serial.print(elapsed); Serial.println(" ms");
    }
    motor(0).powerOff();
}

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    Serial.println("=== ACCELERATION ENGINE TEST ===");
    Serial.print("Flaps/rev: "); Serial.println(FLAPS_PER_REV);
    Serial.print("MAX_ACCEL_STEP = "); Serial.println(Acceleration::MAX_ACCEL_STEP);
    Serial.print("Top-speed period = ");
    Serial.print(Acceleration::ACCEL_STEP_PERIODS[Acceleration::MAX_ACCEL_STEP]);
    Serial.println(" us/step");
    Serial.println("");

    initMotorDriver();

    if (!homeMotorZero()) {
        Serial.println("Halting — check magnet/sensor.");
        while (true) { delay(1000); }
    }
    Serial.println("Homed. Starting acceleration sequence.\n");
}

void loop() {
    uint8_t target = SEQUENCE[seqIndex];
    goToFlapBlocking(target);
    seqIndex = (seqIndex + 1) % SEQUENCE_LEN;
    delay(PAUSE_MS);
}
