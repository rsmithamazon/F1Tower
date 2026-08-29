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
//   3. flapSteps[0] = 0 is home (flap 0). Values start at even
//      2048/52 spacing (same as flap_positions.json "default").
//   4. When dialed in, copy the finished values into
//      Designs+Requirements/flap_positions.json (source of truth,
//      supports per-unit overrides for all 25 units).
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
// Homes at the same TOP_SPEED / ACCELERATION as normal moves (no
// separate slow homing speed).
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

// --- FLAP -> STEP MAPPING (the calibration table) ---
// flapSteps[i] = absolute step count from home where flap i shows.
// This is the editable master table for THIS unit under test. It starts
// as even 2048/52 spacing. Fine-tune any entry that lands off, then copy
// the finished values into Designs+Requirements/flap_positions.json
// (the per-unit source of truth for all 25 units).
long flapSteps[TOTAL_FLAPS] = {
    0,    39,   79,   118,  157,  197,  236,  276,  315,  354,   // 0-9
    394,  433,  472,  512,  551,  591,  630,  669,  709,  748,   // 10-19
    788,  827,  866,  906,  945,  985,  1024, 1063, 1103, 1142,  // 20-29
    1181, 1221, 1260, 1300, 1339, 1378, 1418, 1457, 1497, 1536,  // 30-39
    1575, 1615, 1654, 1693, 1733, 1772, 1812, 1851, 1890, 1930,  // 40-49
    1969, 2009                                                    // 50-51
};

int listIndex = 0;

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

// Spin at full speed until the magnet is found; set that spot as step 0.
// Returns false if the magnet isn't found within MAX_HOME_STEPS.
bool homeMotor() {
    int dir = REVERSE ? -1 : 1;
    // Home at the normal run speed/accel (no separate slow homing pass).
    stepper.setMaxSpeed(TOP_SPEED);
    stepper.setAcceleration(ACCELERATION);
    stepper.setCurrentPosition(0);

    // Spin at full speed (one far target + run() loop, like test 06) and
    // break the moment the magnet is seen.
    //
    // Because flap 0 = home = magnet, the drum often STARTS on the magnet.
    // If so we must first travel clear of the magnet zone, otherwise the
    // search below would trigger instantly at ~0. So: skip a fixed margin
    // forward first (ignoring the sensor), THEN look for the next magnet.
    const long CLEAR_ZONE = 300;   // steps to travel before trusting the sensor

    stepper.move(dir * 1000000L);
    while (labs(stepper.currentPosition()) < CLEAR_ZONE) {
        stepper.run();
    }

    // Search forward at full speed until the magnet triggers, then STOP
    // right there and call it home (step 0 = flap 0). We stop at the
    // magnet edge rather than decelerating past it, so home is at a
    // consistent, repeatable spot for calibration. (A full decel would
    // coast ~hundreds of steps past home and shift every flap position.)
    while (!isMagnetDetected()) {
        stepper.run();
        if (labs(stepper.currentPosition()) > MAX_HOME_STEPS) return false;
    }

    stepper.setCurrentPosition(0);   // home = step 0 = flap 0 (at magnet edge)
    return true;
}

// Print the full flap -> step table to serial (the current mapping).
void printFlapTable() {
    Serial.println(F("Flap position table (flap : steps : rev):"));
    for (int i = 0; i < TOTAL_FLAPS; i++) {
        Serial.print(F("  flap "));
        if (i < 10) Serial.print(' ');
        Serial.print(i);
        Serial.print(F(" : "));
        Serial.print(flapSteps[i]);
        Serial.print(F(" steps : "));
        Serial.print((float)flapSteps[i] / STEPS_PER_REV, 3);
        Serial.println(F(" rev"));
    }
    Serial.println();
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

    Serial.println(F("=== POSITION TEST / FLAP MAPPING ==="));
    Serial.print(F("Baud:        ")); Serial.println(SERIAL_BAUD);
    Serial.print(F("Speed/Accel: ")); Serial.print(TOP_SPEED);
    Serial.print(F(" / ")); Serial.println(ACCELERATION);
    Serial.print(F("Direction:   ")); Serial.println(REVERSE ? F("REVERSE") : F("forward"));
    Serial.print(F("Flaps:       ")); Serial.println(TOTAL_FLAPS);
    Serial.print(F("Visiting:    ")); Serial.print(LIST_LEN); Serial.println(F(" positions"));
    Serial.println(F("Each move homes first, then advances to the flap."));
    Serial.println();

    printFlapTable();
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
