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
const int HALL_PIN = 2;              // GPIO pin for hall effect sensor
const int LED_PIN = LED_BUILTIN;     // Onboard LED — lights when magnet detected
const int LOOP_DELAY_MS = 200;       // How often to print (ms)

void setup() {
    Serial.begin(115200);
    pinMode(HALL_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("=== MAGNET SENSOR TEST ===");
    Serial.println("Bring magnet near sensor to test detection.");
    Serial.println("Press reset to restart.");
    Serial.println("");
}

void loop() {
    int sensorValue = digitalRead(HALL_PIN);

    // Hall sensors are typically active-LOW (LOW = magnet detected)
    bool detected = (sensorValue == LOW);
    digitalWrite(LED_PIN, detected ? HIGH : LOW);   // LED on while magnet present
    if (detected) {
        Serial.println(">>> MAGNET DETECTED <<<");
    } else {
        Serial.println("    Nothing detected");
    }

    delay(LOOP_DELAY_MS);
}
