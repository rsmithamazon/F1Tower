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
// Serial: 115200 baud. Prints direction, per-move detail (start,
// target, step distance, time), and live position while moving.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Wiring:   motor 0 coil pins = 6,7,8,9  (see MOTOR_PINS)
//           hall sensor        = HALL_PIN below
// ===========================================================

// Flip the physical spin direction WITHOUT rewiring the motor.
// Must be defined BEFORE including the engine so it takes effect.
// Set to true if the motor turns the wrong way on your rig.
#define REVERSE_DIRECTION true

#define NUM_MOTORS 1                      // single-motor test rig
#include "motor_driver.h"                 // engine copy lives in this sketch folder

// --- CONFIG ---
const long SERIAL_BAUD    = 115200;
const int  HALL_PIN       = 2;     // Hall sensor (active LOW = magnet at flap 0)
const int  LED_PIN        = LED_BUILTIN;
const int  MAX_HOME_STEPS = 2200;  // Safety limit (just over one revolution)
const int  PAUSE_MS       = 1500;  // Pause at each target flap
const int  LIVE_REPORT_MS = 100;   // How often to print live position during a move

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
    uint8_t startFlap = getCurrentFlap(0);
    // Forward-only distance (matches the engine's wrap-around logic).
    int flapDist = (int)flap - (int)startFlap;
    if (flapDist <= 0) flapDist += FLAPS_PER_REV;

    Serial.println(F("-----------------------------"));
    Serial.print(F("MOVE  from flap ")); Serial.print(startFlap);
    Serial.print(F(" -> ")); Serial.print(flap);
    Serial.print(F("  (forward ")); Serial.print(flapDist); Serial.println(F(" flaps)"));

    unsigned long start = millis();
    unsigned long lastReport = 0;
    motor(0).goToFlap(flap);

    while (motor(0).isMoving() && !motor(0).isError()) {
        motor(0).update();
        // Live position stream so you can watch the ramp progress.
        unsigned long now = millis();
        if (now - lastReport >= (unsigned long)LIVE_REPORT_MS) {
            lastReport = now;
            Serial.print(F("  ... at flap ")); Serial.print(getCurrentFlap(0));
            Serial.print(F("  t=")); Serial.print(now - start); Serial.println(F(" ms"));
        }
    }

    unsigned long elapsed = millis() - start;
    if (motor(0).isError()) {
        Serial.println(F("  STALL/ERROR!"));
        motor(0).clearError();
    } else {
        Serial.print(F("DONE  landed on flap ")); Serial.print(getCurrentFlap(0));
        Serial.print(F(" in ")); Serial.print(elapsed); Serial.println(F(" ms"));
    }
    motor(0).powerOff();
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    Serial.println(F("=== ACCELERATION ENGINE TEST ==="));
    Serial.print(F("Baud: ")); Serial.println(SERIAL_BAUD);
    Serial.print(F("Direction: "));
    Serial.println(REVERSE_DIRECTION ? F("REVERSED") : F("normal"));
    Serial.print(F("Flaps/rev: ")); Serial.println(FLAPS_PER_REV);
    Serial.print(F("MAX_ACCEL_STEP = ")); Serial.println(Acceleration::MAX_ACCEL_STEP);
    Serial.print(F("Top-speed period = "));
    Serial.print(Acceleration::ACCEL_STEP_PERIODS[Acceleration::MAX_ACCEL_STEP]);
    Serial.println(F(" us/step"));
    Serial.println();

    initMotorDriver();

    if (!homeMotorZero()) {
        Serial.println(F("Halting — check magnet/sensor."));
        while (true) { delay(1000); }
    }
    Serial.println(F("Homed. Starting acceleration sequence.\n"));
}

void loop() {
    uint8_t target = SEQUENCE[seqIndex];
    goToFlapBlocking(target);
    seqIndex = (seqIndex + 1) % SEQUENCE_LEN;
    delay(PAUSE_MS);
}
