// ===========================================================
// F1 SPLIT FLAP TOWER — ESP32 MASTER FIRMWARE
// ===========================================================
// Runs on ESP32. Receives display commands from host over WiFi
// (WebSocket), manages transitions, sends flap targets to
// Pico row controllers over I2C.
// ===========================================================

#include "wifi_manager.h"
#include "websocket_server.h"
#include "i2c_master.h"
#include "display_state.h"
#include "transition_engine.h"
#include "flap_lookup.h"
#include "config.h"
#include "error_handler.h"

// --- SETUP ---

void setup() {
    // Serial.begin(115200)
    // loadConfig()                    — load settings from NVS (home toggle, transitions)
    // initI2C()                       — setup I2C bus as master (SDA/SCL pins, 400kHz)
    // initWiFi()                      — connect to configured network
    // initWebSocket()                 — start WebSocket server on port 81
    // initDisplayState()              — load last known board state from NVS
    // runStartupSequence()            — home (if needed) → black → red rows → white
}

// --- MAIN LOOP ---

void loop() {
    // handleWebSocket()               — poll for incoming messages, dispatch commands
    // updateTransitionEngine()        — step any in-progress transition forward
    // checkHeartbeat()                — track host liveness, trigger error state if timeout
    // handleErrorStates()             — check for slave errors, connection issues
    // delay(1)                        — yield (or use FreeRTOS tasks instead)
}
