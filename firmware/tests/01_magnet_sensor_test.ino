// ===========================================================
// TEST 01: Magnet Sensor Validation
// ===========================================================
// Validates hall effect sensor + magnet wiring.
// Loops forever printing sensor state to Serial.
//   - "NOTHING DETECTED" when no magnet present
//   - "MAGNET DETECTED" while magnet is over sensor
//
// Hardware: Pico + 1 hall effect sensor
// Wiring: Sensor signal → HALL_PIN, VCC → 3.3V, GND → GND
// ===========================================================

// --- CONFIG ---
const int HALL_PIN = 2;          // GPIO pin for hall effect sensor
const int LOOP_DELAY_MS = 200;   // How often to print (ms)

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);

    Serial.println("=== MAGNET SENSOR TEST ===");
    Serial.println("Bring magnet near sensor to test detection.");
    Serial.println("Press reset to restart.");
    Serial.println("");
}

void loop() {
    int sensorValue = digitalRead(HALL_PIN);

    // Hall sensors are typically active-LOW (LOW = magnet detected)
    if (sensorValue == LOW) {
        Serial.println(">>> MAGNET DETECTED <<<");
    } else {
        Serial.println("    Nothing detected");
    }

    delay(LOOP_DELAY_MS);
}
