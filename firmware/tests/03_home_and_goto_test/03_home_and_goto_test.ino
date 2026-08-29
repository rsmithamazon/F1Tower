// ===========================================================
// TEST 03: Home + Go-To-Flap Cycle Test
// ===========================================================
// Homes the motor (spins until magnet detected), moves to a
// target flap using the real acceleration engine, pauses, then
// homes again. Loops forever. Validates homing + position
// accuracy + acceleration in a repeating cycle.
//
// Uses the FlapMotor engine from motor_driver.h — the same code
// that runs on the Pico row controller. Arduino is just the rig.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Wiring:   motor 0 coil pins = 6,7,8,9 ; hall = HALL_PIN
// ===========================================================

#define NUM_MOTORS 1
#include <motor_driver.h>                 // from firmware/libraries/FlapMotor

// --- CONFIG ---
int TARGET_FLAP      = 26;      // Flap to go to after homing (0-51)
int PAUSE_AT_FLAP_MS = 2000;
int MAX_HOME_STEPS   = 2200;    // Safety limit (~1 revolution)

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
            Serial.println("FAILED (magnet not found!)");
            return false;
        }
    }
    motor(0).powerOff();
    motor(0).setHomed();
    Serial.print("OK ("); Serial.print(steps); Serial.println(" steps to home)");
    return true;
}

void goToFlap(int flap) {
    Serial.print("  Going to flap "); Serial.print(flap); Serial.print("... ");
    motor(0).goToFlap((uint8_t)flap);
    while (motor(0).isMoving() && !motor(0).isError()) {
        motor(0).update();
    }
    Serial.print("arrived at "); Serial.println(getCurrentFlap(0));
}

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    initMotorDriver();

    Serial.println("=== HOME + GO-TO-FLAP CYCLE TEST ===");
    Serial.print("Flaps/rev:   "); Serial.println(FLAPS_PER_REV);
    Serial.print("Target flap: "); Serial.println(TARGET_FLAP);
    Serial.println("Cycle: Home -> Go to flap -> Pause -> Home -> Repeat");
    Serial.println("");
}

void loop() {
    cycleCount++;
    Serial.print("--- Cycle "); Serial.print(cycleCount); Serial.println(" ---");

    if (!homeMotor()) {
        Serial.println("  ERROR: Halting. Check magnet/sensor.");
        while (true) { delay(1000); }
    }

    delay(500);
    goToFlap(TARGET_FLAP);

    Serial.print("  Pausing "); Serial.print(PAUSE_AT_FLAP_MS); Serial.println(" ms...");
    motor(0).powerOff();
    delay(PAUSE_AT_FLAP_MS);
    Serial.println("");
}
