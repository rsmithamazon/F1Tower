// ===========================================================
// TEST 02: Motor Speed / Acceleration Tuning
// ===========================================================
// Spins a stepper motor with configurable speed, steps, and
// acceleration. Change the globals below, upload, and observe.
// Goal: find max speed, ideal acceleration, and step count
// per flap for smooth, reliable operation.
//
// Hardware: Pico + 1 ULN2003 + 1 28BYJ-48
// Library: AccelStepper (install via Library Manager)
// ===========================================================

#include <AccelStepper.h>

// --- TUNE THESE GLOBALS ---
float MOTOR_SPEED       = 800.0;    // Steps per second (try: 400, 600, 800, 1000, 1200)
float MOTOR_ACCEL       = 400.0;    // Steps per second^2 (try: 200, 400, 800, 1600)
int   STEPS_TO_MOVE     = 512;      // Steps to move (full rev = 2048, one flap ~39)
int   PAUSE_BETWEEN_MS  = 2000;     // Pause between movements (ms)

// --- MOTOR PINS (half-step mode, 4 pins) ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;

// AccelStepper in HALF4WIRE mode (8-step sequence for 28BYJ-48)
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int cycleCount = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("=== MOTOR SPEED TEST ===");
    Serial.println("Globals:");
    Serial.print("  Speed:        "); Serial.print(MOTOR_SPEED); Serial.println(" steps/sec");
    Serial.print("  Acceleration: "); Serial.print(MOTOR_ACCEL); Serial.println(" steps/sec^2");
    Serial.print("  Steps:        "); Serial.println(STEPS_TO_MOVE);
    Serial.print("  Pause:        "); Serial.print(PAUSE_BETWEEN_MS); Serial.println(" ms");
    Serial.println("");
    Serial.println("Motor will move forward, pause, repeat.");
    Serial.println("Change globals and re-upload to tune.");
    Serial.println("");

    stepper.setMaxSpeed(MOTOR_SPEED);
    stepper.setAcceleration(MOTOR_ACCEL);
}

void loop() {
    // Move forward
    cycleCount++;
    Serial.print("Cycle "); Serial.print(cycleCount);
    Serial.print(" — Moving "); Serial.print(STEPS_TO_MOVE); Serial.println(" steps...");

    unsigned long startTime = millis();
    stepper.move(STEPS_TO_MOVE);

    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }

    unsigned long elapsed = millis() - startTime;
    Serial.print("  Done in "); Serial.print(elapsed); Serial.println(" ms");

    // Power off coils (prevent heating)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    Serial.print("  Pausing "); Serial.print(PAUSE_BETWEEN_MS); Serial.println(" ms...");
    Serial.println("");
    delay(PAUSE_BETWEEN_MS);
}
