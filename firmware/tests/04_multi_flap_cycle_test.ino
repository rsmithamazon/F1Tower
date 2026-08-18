// ===========================================================
// TEST 04: Multi-Flap Cycle (Home → 3 Positions → Home)
// ===========================================================
// Same as test 03 but visits 3 different flap positions
// after each home. Validates position accuracy across
// multiple targets in sequence.
//
// Cycle: Home → Flap A → Home → Flap B → Home → Flap C → repeat
//
// Hardware: Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Library: AccelStepper
// ===========================================================

#include <AccelStepper.h>

// --- TUNE THESE GLOBALS ---
float MOTOR_SPEED       = 800.0;
float MOTOR_ACCEL       = 400.0;
int   TARGET_FLAP_A     = 10;       // First target (e.g. number "2")
int   TARGET_FLAP_B     = 30;       // Second target (e.g. letter "M")
int   TARGET_FLAP_C     = 45;       // Third target (e.g. period ".")
int   TOTAL_FLAPS       = 52;
int   PAUSE_AT_FLAP_MS  = 2000;
int   MAX_HOME_STEPS    = 2200;

// --- PINS ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int HALL_PIN = 2;

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int flapStepTable[52];
int cycleCount = 0;

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
    return digitalRead(HALL_PIN) == LOW;
}

bool homeMotor() {
    Serial.print("  Homing... ");
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
    Serial.print("  → Flap "); Serial.print(flap);
    Serial.print(" (step "); Serial.print(targetSteps); Serial.print(")... ");

    stepper.moveTo(targetSteps);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
    Serial.println("OK");
}

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    delay(1000);

    buildFlapTable();

    Serial.println("=== MULTI-FLAP CYCLE TEST ===");
    Serial.println("Globals:");
    Serial.print("  Speed:    "); Serial.print(MOTOR_SPEED); Serial.println(" steps/sec");
    Serial.print("  Accel:    "); Serial.print(MOTOR_ACCEL); Serial.println(" steps/sec^2");
    Serial.print("  Flap A:   "); Serial.println(TARGET_FLAP_A);
    Serial.print("  Flap B:   "); Serial.println(TARGET_FLAP_B);
    Serial.print("  Flap C:   "); Serial.println(TARGET_FLAP_C);
    Serial.println("");
    Serial.println("Cycle: Home → A → Home → B → Home → C → repeat");
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
        powerOff();
        delay(PAUSE_AT_FLAP_MS);
    }

    Serial.println("");
}
