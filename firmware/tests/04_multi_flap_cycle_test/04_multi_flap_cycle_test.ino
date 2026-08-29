// ===========================================================
// TEST 04: Multi-Flap Cycle (Home -> 3 Positions -> Home)
// ===========================================================
// Same as test 03 but visits 3 different flap positions after
// each home. Validates position accuracy across multiple targets
// in sequence, using the real acceleration engine.
//
// Cycle: Home -> Flap A -> Home -> Flap B -> Home -> Flap C -> repeat
//
// Uses the FlapMotor engine from motor_driver.h (same code the
// Pico row controller runs). Arduino is just the test rig.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Wiring:   motor 0 coil pins = 6,7,8,9 ; hall = HALL_PIN
// ===========================================================

#define NUM_MOTORS 1
#include <motor_driver.h>                 // from firmware/libraries/FlapMotor

// --- CONFIG ---
int TARGET_FLAP_A    = 10;      // e.g. number "2"
int TARGET_FLAP_B    = 30;      // e.g. letter "M"
int TARGET_FLAP_C    = 45;      // e.g. period "."
int PAUSE_AT_FLAP_MS = 2000;
int MAX_HOME_STEPS   = 2200;

const int HALL_PIN = 2;
const int LED_PIN  = LED_BUILTIN;

int cycleCount = 0;

bool isMagnetDetected() {
    bool detected = (digitalRead(HALL_PIN) == LOW);
    digitalWrite(LED_PIN, detected ? HIGH : LOW);
    return detected;
}

bool homeMotor() {
    Serial.print("  Homing... ");
    int steps = 0;
    while (!isMagnetDetected()) {
        motor(0).homeStepForward(3000);
        steps++;
        if (steps > MAX_HOME_STEPS) {
            Serial.println("FAILED!");
            return false;
        }
    }
    motor(0).powerOff();
    motor(0).setHomed();
    Serial.print("OK ("); Serial.print(steps); Serial.println(" steps)");
    return true;
}

void goToFlap(int flap) {
    Serial.print("  -> Flap "); Serial.print(flap); Serial.print("... ");
    motor(0).goToFlap((uint8_t)flap);
    while (motor(0).isMoving() && !motor(0).isError()) {
        motor(0).update();
    }
    Serial.print("at "); Serial.println(getCurrentFlap(0));
}

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    initMotorDriver();

    Serial.println("=== MULTI-FLAP CYCLE TEST ===");
    Serial.print("Flaps/rev: "); Serial.println(FLAPS_PER_REV);
    Serial.print("Flap A/B/C: ");
    Serial.print(TARGET_FLAP_A); Serial.print(" / ");
    Serial.print(TARGET_FLAP_B); Serial.print(" / ");
    Serial.println(TARGET_FLAP_C);
    Serial.println("Cycle: Home -> A -> Home -> B -> Home -> C -> repeat");
    Serial.println("");
}

void loop() {
    cycleCount++;
    Serial.print("=== Cycle "); Serial.print(cycleCount); Serial.println(" ===");

    int targets[] = { TARGET_FLAP_A, TARGET_FLAP_B, TARGET_FLAP_C };

    for (int i = 0; i < 3; i++) {
        if (!homeMotor()) {
            Serial.println("  ERROR: Halting.");
            while (true) { delay(1000); }
        }
        delay(300);
        goToFlap(targets[i]);
        motor(0).powerOff();
        delay(PAUSE_AT_FLAP_MS);
    }
    Serial.println("");
}
