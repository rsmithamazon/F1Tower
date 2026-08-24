// ===========================================================
// TEST 05: Position Marker — Flap Alignment Tool
// ===========================================================
// Steps through each flap position one at a time, pausing at
// each so you can verify/adjust physical flap alignment.
//
// Press ENTER in Serial Monitor (or wait for auto-advance)
// to move to the next flap. Prints current position so you
// can mark/adjust each flap on the drum.
//
// Use this to:
//   1. Verify the lookup table matches physical flap positions
//   2. Adjust flap placement on the drum if alignment is off
//   3. Find the exact step count where each flap shows correctly
//
// Hardware: Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Library: AccelStepper
// ===========================================================

#include <AccelStepper.h>

// --- GLOBALS ---
float MOTOR_SPEED       = 400.0;    // Slow speed for precise positioning
float MOTOR_ACCEL       = 200.0;
int   TOTAL_FLAPS       = 52;
bool  AUTO_ADVANCE      = false;    // true = auto-advance after delay, false = wait for Serial input
int   AUTO_DELAY_MS     = 3000;     // Delay between flaps if auto-advance
int   MAX_HOME_STEPS    = 2200;

// --- PINS ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int HALL_PIN = 2;
const int LED_PIN = LED_BUILTIN;   // Nano onboard LED (pin 13) — lights when magnet detected

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int flapStepTable[52];
int currentFlap = 0;

void buildFlapTable() {
    for (int i = 0; i < TOTAL_FLAPS; i++) {
        flapStepTable[i] = (int)((2048.0 / TOTAL_FLAPS) * i + 0.5);
    }
}

void powerOff() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

bool isMagnetDetected() {
    bool detected = (digitalRead(HALL_PIN) == LOW);
    digitalWrite(LED_PIN, detected ? HIGH : LOW);   // LED on while a magnet is present
    return detected;
}

bool homeMotor() {
    Serial.print("Homing... ");
    stepper.setMaxSpeed(400);
    stepper.setAcceleration(200);

    int steps = 0;
    while (!isMagnetDetected()) {
        stepper.move(1);
        while (stepper.distanceToGo() != 0) {
            stepper.run();
        }
        steps++;
        if (steps > MAX_HOME_STEPS) {
            Serial.println("FAILED!");
            return false;
        }
    }

    stepper.setCurrentPosition(0);
    Serial.print("OK ("); Serial.print(steps); Serial.println(" steps)");
    stepper.setMaxSpeed(MOTOR_SPEED);
    stepper.setAcceleration(MOTOR_ACCEL);
    return true;
}

void goToFlap(int flap) {
    int targetSteps = flapStepTable[flap];
    stepper.moveTo(targetSteps);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
}

void printFlapInfo(int flap) {
    Serial.println("-----------------------------");
    Serial.print("FLAP "); Serial.print(flap); Serial.print(" / "); Serial.println(TOTAL_FLAPS - 1);
    Serial.print("  Step position: "); Serial.println(flapStepTable[flap]);
    Serial.print("  Steps from previous: ");
    if (flap > 0) {
        Serial.println(flapStepTable[flap] - flapStepTable[flap - 1]);
    } else {
        Serial.println("(home)");
    }
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
            isMagnetDetected();   // refreshes LED_PIN from the hall sensor
            delay(50);
        }
        // Clear input buffer
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

    buildFlapTable();

    Serial.println("=== POSITION MARKER / FLAP ALIGNMENT TOOL ===");
    Serial.println("");
    Serial.println("This tool steps through each flap position one by one.");
    Serial.println("At each position, verify the correct flap is showing.");
    Serial.println("If a flap is misaligned, physically adjust it on the drum.");
    Serial.println("");
    Serial.print("Mode: "); Serial.println(AUTO_ADVANCE ? "Auto-advance" : "Manual (press ENTER)");
    Serial.print("Total flaps: "); Serial.println(TOTAL_FLAPS);
    Serial.println("");

    // Home first
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
    if (currentFlap >= TOTAL_FLAPS) {
        Serial.println("");
        Serial.println("=== FULL REVOLUTION COMPLETE ===");
        Serial.println("All 52 flap positions visited.");
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
    powerOff();
    printFlapInfo(currentFlap);
}
