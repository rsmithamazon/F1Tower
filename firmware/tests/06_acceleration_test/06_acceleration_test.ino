// ===========================================================
// TEST 06: Manual Speed / Acceleration Tuning
// ===========================================================
// Hand-tunable speed and acceleration test. Edit the two knobs
// below, re-upload, and watch/listen. Use it to find the fastest
// speed + acceleration your 28BYJ-48 runs at WITHOUT stalling or
// skipping steps. Once you find good values, they can be baked
// into the engine's acceleration table (generate_acceleration.py).
//
// This is a SELF-CONTAINED tuning rig using AccelStepper (which
// gives simple max-speed / acceleration knobs). It does NOT use
// the table-driven FlapMotor engine, on purpose — the whole point
// is to sweep speed/accel by hand. The values you land on here
// feed back into the real engine's table.
//
// Each cycle the motor spins CONTINUOUSLY, stopping either when the
// magnet is detected (STOP_ON_MAGNET = true) or after SPIN_SECONDS
// (STOP_ON_MAGNET = false, default 5 s).
//
// Serial: 115200 baud. Prints the current settings, direction, spin
// mode, and per-spin timing so you can compare runs.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 (+ hall for magnet mode)
// Wiring:   coil pins IN1,IN2,IN3,IN4 = 6,7,8,9 ; hall sensor = pin 2
// ===========================================================

#include <AccelStepper.h>

// ===========================================================
// ===============  TUNE THESE TWO KNOBS  ====================
// ===========================================================

// TOP_SPEED — steps per second the motor ramps up to.
//   HIGHER value  = FASTER spin   (e.g. 900, 1000, 1200)
//   LOWER  value  = SLOWER spin   (e.g. 400, 600)
//   Too high and the 28BYJ-48 will stall/skip or just buzz.
//   Practical ceiling for a 28BYJ-48 at 5V is ~1000-1100 steps/s.
float TOP_SPEED = 800.0;

// ACCELERATION — how quickly it ramps up to TOP_SPEED, in steps/sec^2.
//   HIGHER value  = reaches top speed FASTER (snappier, more torque
//                   demand — too high causes stall on start).
//   LOWER  value  = ramps up SLOWER / gentler (smoother, safer).
//   Typical range: 200 (gentle) .. 2000 (aggressive).
float ACCELERATION = 400.0;

// ===========================================================

// --- DIRECTION ---
// The 28BYJ-48 coil order via ULN2003 is A,C,B,D, so AccelStepper is
// given the pins as IN1,IN3,IN2,IN4 (= 6,8,7,9). To reverse the spin,
// set REVERSE = true (negates the move distance).
//   REVERSE = false -> forward
//   REVERSE = true  -> backward
// NOTE: one direction is not inherently faster than the other — speed
// is set purely by TOP_SPEED/ACCELERATION above. Direction only flips
// which way the drum turns.
const bool REVERSE = false;

// --- SPIN MODE ---
// Instead of a fixed number of steps, spin CONTINUOUSLY each cycle.
// Choose how the spin ends:
//   STOP_ON_MAGNET = true  -> spin until the hall sensor sees the magnet
//   STOP_ON_MAGNET = false -> spin for SPIN_SECONDS, then stop
const bool  STOP_ON_MAGNET = false;
float       SPIN_SECONDS   = 5.0;    // duration when STOP_ON_MAGNET is false
const int   MAX_SPIN_STEPS = 6000;   // safety cap for magnet mode (~3 revolutions)

int  PAUSE_MS        = 1500;   // pause between sweeps
const long SERIAL_BAUD = 115200;

// --- PINS (IN1,IN3,IN2,IN4 order for correct coil sequence) ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int HALL_PIN = 2;        // hall sensor (active LOW = magnet detected)
const int LED_PIN  = LED_BUILTIN;
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int cycleCount = 0;

void powerOff() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

bool isMagnetDetected() {
    bool detected = (digitalRead(HALL_PIN) == LOW);
    digitalWrite(LED_PIN, detected ? HIGH : LOW);
    return detected;
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    stepper.setMaxSpeed(TOP_SPEED);
    stepper.setAcceleration(ACCELERATION);

    Serial.println(F("=== MANUAL SPEED / ACCEL TUNING ==="));
    Serial.print(F("Baud:         ")); Serial.println(SERIAL_BAUD);
    Serial.print(F("TOP_SPEED:    ")); Serial.print(TOP_SPEED);    Serial.println(F(" steps/s   (higher = faster)"));
    Serial.print(F("ACCELERATION: ")); Serial.print(ACCELERATION); Serial.println(F(" steps/s^2 (higher = ramps up faster)"));
    Serial.print(F("Direction:    ")); Serial.println(REVERSE ? F("REVERSE (backward)") : F("forward"));
    Serial.print(F("Spin mode:    "));
    if (STOP_ON_MAGNET) {
        Serial.println(F("until MAGNET detected"));
    } else {
        Serial.print(SPIN_SECONDS); Serial.println(F(" seconds per spin"));
    }
    Serial.println(F("Edit TOP_SPEED / ACCELERATION at top, re-upload to tune."));
    Serial.println();
}

void loop() {
    cycleCount++;
    int dir = REVERSE ? -1 : 1;

    Serial.print(F("Cycle ")); Serial.print(cycleCount);
    if (STOP_ON_MAGNET) {
        Serial.println(F(" — spinning until magnet..."));
    } else {
        Serial.print(F(" — spinning for ")); Serial.print(SPIN_SECONDS); Serial.println(F(" s..."));
    }

    unsigned long start = millis();
    unsigned long durationMs = (unsigned long)(SPIN_SECONDS * 1000.0);
    long stepsTaken = 0;
    bool stoppedByMagnet = false;

    // Spin continuously: keep pushing the target far ahead so AccelStepper
    // ramps up and holds TOP_SPEED, and stop when our condition is met.
    while (true) {
        // Keep a far-away target so it never decelerates toward a target.
        if (stepper.distanceToGo() == 0) {
            stepper.move(dir * 2000L);
        }
        stepper.run();

        long moved = stepper.currentPosition();
        stepsTaken = moved >= 0 ? moved : -moved;

        if (STOP_ON_MAGNET) {
            if (isMagnetDetected()) { stoppedByMagnet = true; break; }
            if (stepsTaken >= MAX_SPIN_STEPS) break;   // safety cap
        } else {
            if (millis() - start >= durationMs) break;
        }
    }

    unsigned long elapsed = millis() - start;
    float avgSpeed = (elapsed > 0) ? (1000.0 * stepsTaken / elapsed) : 0;

    if (STOP_ON_MAGNET) {
        if (stoppedByMagnet) {
            Serial.print(F("  MAGNET found after ")); Serial.print(stepsTaken); Serial.println(F(" steps"));
        } else {
            Serial.print(F("  NO magnet within ")); Serial.print(MAX_SPIN_STEPS); Serial.println(F(" steps (check sensor)"));
        }
    }
    Serial.print(F("  spun ")); Serial.print(stepsTaken);
    Serial.print(F(" steps in ")); Serial.print(elapsed);
    Serial.print(F(" ms  (avg ")); Serial.print(avgSpeed); Serial.println(F(" steps/s)"));

    stepper.setCurrentPosition(0);   // reset counter for next cycle
    powerOff();                      // de-energize between spins (prevents heating)
    delay(PAUSE_MS);
}
