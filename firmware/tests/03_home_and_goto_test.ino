// ===========================================================
// TEST 03: Home + Go-To-Flap Cycle Test
// ===========================================================
// Homes the motor (spins until magnet detected), then moves
// to a specific flap position, then homes again. Loops forever.
// Validates homing + position accuracy in a repeating cycle.
//
// Hardware: Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Library: AccelStepper
// ===========================================================

#include <AccelStepper.h>

// --- TUNE THESE GLOBALS ---
float MOTOR_SPEED       = 800.0;    // Steps per second
float MOTOR_ACCEL       = 400.0;    // Steps per second^2
int   TARGET_FLAP       = 26;       // Flap position to go to after homing (0-51)
int   STEPS_PER_FLAP    = 39;       // Steps between flaps (2048 / 52 ≈ 39.4)
int   TOTAL_FLAPS       = 52;
int   PAUSE_AT_FLAP_MS  = 2000;     // Pause at target before homing again
int   MAX_HOME_STEPS    = 2200;     // Safety limit for homing (slightly over 1 full rev)

// --- PINS ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int HALL_PIN = 2;

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int cycleCount = 0;

// --- FLAP STEP LOOKUP TABLE ---
// Pre-computed step positions for each flap (accounts for fractional steps)
int flapStepTable[52];

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

// Home: spin forward slowly until magnet detected
bool homeMotor() {
    Serial.print("  Homing... ");
    stepper.setMaxSpeed(400);  // Slow for homing
    stepper.setAcceleration(200);

    int steps = 0;
    while (!isMagnetDetected()) {
        stepper.move(1);
        while (stepper.distanceToGo() != 0) {
            stepper.run();
        }
        steps++;
        if (steps > MAX_HOME_STEPS) {
            Serial.println("FAILED (magnet not found!)");
            return false;
        }
    }

    stepper.setCurrentPosition(0);  // Zero the position
    Serial.print("OK ("); Serial.print(steps); Serial.println(" steps to home)");

    // Restore normal speed
    stepper.setMaxSpeed(MOTOR_SPEED);
    stepper.setAcceleration(MOTOR_ACCEL);
    return true;
}

// Move to a specific flap position
void goToFlap(int flap) {
    int targetSteps = flapStepTable[flap];
    Serial.print("  Going to flap "); Serial.print(flap);
    Serial.print(" (step "); Serial.print(targetSteps); Serial.print(")... ");

    stepper.moveTo(targetSteps);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
    Serial.println("Arrived.");
}

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    delay(1000);

    buildFlapTable();

    Serial.println("=== HOME + GO-TO-FLAP CYCLE TEST ===");
    Serial.println("Globals:");
    Serial.print("  Speed:        "); Serial.print(MOTOR_SPEED); Serial.println(" steps/sec");
    Serial.print("  Acceleration: "); Serial.print(MOTOR_ACCEL); Serial.println(" steps/sec^2");
    Serial.print("  Target flap:  "); Serial.println(TARGET_FLAP);
    Serial.print("  Steps/flap:   "); Serial.println(STEPS_PER_FLAP);
    Serial.println("");
    Serial.println("Cycle: Home → Go to flap → Pause → Home → Repeat");
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
    powerOff();
    delay(PAUSE_AT_FLAP_MS);

    Serial.println("");
}
