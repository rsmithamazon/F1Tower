// ===========================================================
// TEST 07: Position Test / Flap Mapping
// ===========================================================
// Goes to a hardcoded list of flap positions, one at a time.
// EVERY move re-homes first: it spins forward until the magnet
// (home = flap 0), then continues past home to the target flap.
// So no matter where the drum starts, each "go to flap N" is
// absolute and repeatable.
//
// Use this to map/fine-tune each flap's exact step position:
//   1. Run it, watch which physical flap shows at each stop.
//   2. If a flap is a bit off, tweak its entry in flapSteps[]
//      below (add/subtract a few steps), re-upload, re-check.
//   3. flapSteps[0] = 0 is home (flap 0). Others default to an
//      even 2048/52 spacing until you fine-tune them.
//
// Serial: 115200 baud. Prints each target flap + step position.
//
// Hardware: Arduino/Pico + 1 ULN2003 + 1 28BYJ-48 + 1 hall sensor
// Wiring:   coil pins IN1,IN2,IN3,IN4 = 6,7,8,9 ; hall = pin 2
// ===========================================================

#include <AccelStepper.h>

// --- MOTION (matches test 06 tuning) ---
float TOP_SPEED    = 1300.0;   // steps/s
float ACCELERATION = 3000.0;   // steps/s^2
const bool REVERSE = true;     // spin direction (matches test 06)

// --- HOMING ---
float HOME_SPEED     = 400.0;  // slow, reliable speed while searching for home
long  MAX_HOME_STEPS = 2400;   // safety cap (a bit over one revolution)

// --- FLAP GEOMETRY ---
const int  TOTAL_FLAPS    = 52;
const long STEPS_PER_REV  = 2048;

// --- POSITION LIST (edit for whatever you want to visit) ---
// The motor visits these flaps in order, re-homing before each.
uint8_t POSITION_LIST[] = { 0, 1, 2, 3, 8, 17, 26, 43, 51 };
const int LIST_LEN = sizeof(POSITION_LIST) / sizeof(POSITION_LIST[0]);

int  PAUSE_AT_FLAP_MS = 2500;  // pause at each flap so you can inspect it
const long SERIAL_BAUD = 115200;

// --- PINS (IN1,IN3,IN2,IN4 order for correct 28BYJ-48 coil sequence) ---
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int HALL_PIN = 2;
const int LED_PIN  = LED_BUILTIN;
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

// --- FLAP -> STEP MAPPING ---
// flapSteps[i] = absolute step position (from home) where flap i shows.
// Defaults to even spacing; FINE-TUNE individual entries as needed.
long flapSteps[TOTAL_FLAPS];

int listIndex = 0;

void buildFlapTable() {
    for (int i = 0; i < TOTAL_FLAPS; i++) {
        flapSteps[i] = (long)((double)STEPS_PER_REV / TOTAL_FLAPS * i + 0.5);
    }
    // ----- FINE-TUNE OVERRIDES (uncomment/edit as you map) -----
    // flapSteps[0]  = 0;      // home
    // flapSteps[1]  = 39;
    // flapSteps[26] = 1024;
    // ... adjust any flap that lands off by a few steps ...
}

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

// Spin forward slowly until the magnet is found; set that as step 0.
// Returns false if the magnet isn't found within MAX_HOME_STEPS.
bool homeMotor() {
    int dir = REVERSE ? -1 : 1;
    stepper.setMaxSpeed(HOME_SPEED);
    stepper.setAcceleration(ACCELERATION);
    stepper.setCurrentPosition(0);

    long steps = 0;
    // If we're sitting ON the magnet already, step off it first so we
    // detect a fresh edge rather than stopping immediately.
    while (isMagnetDetected() && steps < 200) {
        stepper.move(dir * 1);
        while (stepper.distanceToGo() != 0) stepper.run();
        steps++;
    }

    steps = 0;
    while (!isMagnetDetected()) {
        stepper.move(dir * 1);
        while (stepper.distanceToGo() != 0) stepper.run();
        steps++;
        if (steps > MAX_HOME_STEPS) return false;
    }

    stepper.setCurrentPosition(0);   // home = step 0 = flap 0
    // Restore run speed for the move to the target flap.
    stepper.setMaxSpeed(TOP_SPEED);
    stepper.setAcceleration(ACCELERATION);
    return true;
}

// Home, then advance forward to the target flap's step position.
void goToFlap(uint8_t flap) {
    Serial.println(F("-----------------------------"));
    Serial.print(F("Target flap ")); Serial.print(flap);
    Serial.print(F(" (step ")); Serial.print(flapSteps[flap]); Serial.println(F(")"));

    Serial.print(F("  homing... "));
    if (!homeMotor()) {
        Serial.println(F("FAILED (magnet not found) — check sensor. Halting."));
        powerOff();
        while (true) { delay(1000); }
    }
    Serial.println(F("home found."));

    // From home (step 0), advance forward to the flap's step position.
    int dir = REVERSE ? -1 : 1;
    long target = dir * flapSteps[flap];
    stepper.moveTo(target);
    while (stepper.distanceToGo() != 0) stepper.run();

    Serial.print(F("  arrived at flap ")); Serial.print(flap);
    Serial.print(F("  (")); Serial.print(flapSteps[flap]); Serial.println(F(" steps from home)"));
    Serial.println(F("  >> Check alignment. Adjust flapSteps[] if off."));
    powerOff();
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    delay(1000);

    buildFlapTable();

    Serial.println(F("=== POSITION TEST / FLAP MAPPING ==="));
    Serial.print(F("Baud:        ")); Serial.println(SERIAL_BAUD);
    Serial.print(F("Speed/Accel: ")); Serial.print(TOP_SPEED);
    Serial.print(F(" / ")); Serial.println(ACCELERATION);
    Serial.print(F("Direction:   ")); Serial.println(REVERSE ? F("REVERSE") : F("forward"));
    Serial.print(F("Flaps:       ")); Serial.println(TOTAL_FLAPS);
    Serial.print(F("Visiting:    ")); Serial.print(LIST_LEN); Serial.println(F(" positions"));
    Serial.println(F("Each move homes first, then advances to the flap."));
    Serial.println();
}

void loop() {
    uint8_t flap = POSITION_LIST[listIndex];

    if (flap >= TOTAL_FLAPS) {
        Serial.print(F("SKIP invalid flap ")); Serial.println(flap);
    } else {
        goToFlap(flap);
    }

    listIndex++;
    if (listIndex >= LIST_LEN) {
        Serial.println(F("\n=== list complete — repeating ===\n"));
        listIndex = 0;
    }

    delay(PAUSE_AT_FLAP_MS);
}
