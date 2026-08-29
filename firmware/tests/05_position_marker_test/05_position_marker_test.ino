// ===========================================================
// TEST 05: Position Marker — Flap Alignment Tool
// ===========================================================
// Steps through each flap position one at a time, pausing at each
// so you can verify/adjust physical flap alignment.
//
// Press ENTER in Serial Monitor (or wait for auto-advance) to move
// to the next flap. Prints current position so you can mark/adjust
// each flap on the drum.
//
// Use this to:
//   1. Verify the engine's flap->step mapping matches the physical drum
//   2. Adjust flap placement on the drum if alignment is off
//
// Uses the FlapMotor engine from motor_driver.h (same code the Pico
// row controller runs). Arduino is just the test rig.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Wiring:   motor 0 coil pins = 6,7,8,9 ; hall = HALL_PIN
// ===========================================================

#define NUM_MOTORS 1
#include <motor_driver.h>                 // from firmware/libraries/FlapMotor

// --- CONFIG ---
bool AUTO_ADVANCE   = false;    // true = auto-advance after delay, false = wait for input
int  AUTO_DELAY_MS  = 3000;
int  MAX_HOME_STEPS = 2200;

const int HALL_PIN = 2;
const int LED_PIN  = LED_BUILTIN;

int currentFlap = 0;

bool isMagnetDetected() {
    bool detected = (digitalRead(HALL_PIN) == LOW);
    digitalWrite(LED_PIN, detected ? HIGH : LOW);   // LED on while magnet present
    return detected;
}

bool homeMotor() {
    Serial.print("Homing... ");
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
    motor(0).goToFlap((uint8_t)flap);
    while (motor(0).isMoving() && !motor(0).isError()) {
        motor(0).update();
    }
    motor(0).powerOff();
}

void printFlapInfo(int flap) {
    Serial.println("-----------------------------");
    Serial.print("FLAP "); Serial.print(flap); Serial.print(" / "); Serial.println(FLAPS_PER_REV - 1);
    Serial.print("  Engine reports flap: "); Serial.println(getCurrentFlap(0));
    Serial.println("  >> Check alignment. Adjust flap on drum if needed.");
    if (!AUTO_ADVANCE) {
        Serial.println("  >> Press ENTER to advance to next flap.");
    }
    Serial.println("-----------------------------");
}

void waitForInput() {
    if (AUTO_ADVANCE) {
        delay(AUTO_DELAY_MS);
    } else {
        // Wait for any serial input; keep the LED tracking the magnet meanwhile
        while (!Serial.available()) {
            isMagnetDetected();
            delay(50);
        }
        while (Serial.available()) {
            Serial.read();
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    initMotorDriver();

    Serial.println("=== POSITION MARKER / FLAP ALIGNMENT TOOL ===");
    Serial.println("");
    Serial.println("Steps through each flap position one by one.");
    Serial.println("At each position, verify the correct flap is showing.");
    Serial.println("If misaligned, physically adjust it on the drum.");
    Serial.println("");
    Serial.print("Mode: "); Serial.println(AUTO_ADVANCE ? "Auto-advance" : "Manual (press ENTER)");
    Serial.print("Total flaps: "); Serial.println(FLAPS_PER_REV);
    Serial.println("");

    if (!homeMotor()) {
        Serial.println("ERROR: Cannot home. Check magnet/sensor. Halting.");
        while (true) { delay(1000); }
    }

    Serial.println("");
    Serial.println("Starting at Flap 0 (Home / Magnet position)");
    printFlapInfo(0);
}

void loop() {
    waitForInput();

    currentFlap++;
    if (currentFlap >= FLAPS_PER_REV) {
        Serial.println("");
        Serial.println("=== FULL REVOLUTION COMPLETE ===");
        Serial.print("All "); Serial.print(FLAPS_PER_REV); Serial.println(" flap positions visited.");
        Serial.println("Re-homing and starting over...");
        Serial.println("");

        currentFlap = 0;
        if (!homeMotor()) {
            Serial.println("ERROR: Halting.");
            while (true) { delay(1000); }
        }
        printFlapInfo(0);
        return;
    }

    goToFlap(currentFlap);
    printFlapInfo(currentFlap);
}
