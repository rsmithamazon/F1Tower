// ===========================================================
// CONFIG — ESP32 settings stored in NVS (non-volatile storage)
// ===========================================================
// Persistent settings: WiFi creds, transition prefs, timing,
// startup behavior. Loaded on boot, updated via host commands.
// ===========================================================

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// --- STRUCTURES ---

typedef struct {
    // WiFi
    char ssid[32];
    char password[64];

    // WebSocket
    uint16_t wsPort;                // Default: 81

    // Startup behavior
    bool homeOnStartup;             // true = always home, false = trust stored state
    uint8_t startupTransition;      // TRANS_LIGHTS_OUT (default)

    // Transition preferences (per event type)
    uint8_t transPageScroll;        // Default: TRANS_CASCADE_DOWN
    uint8_t transModeChange;        // Default: TRANS_CAR_RAINBOW
    uint8_t transFlagReturn;        // Default: TRANS_IN_PLACE
    uint8_t transShutdown;          // Default: TRANS_CASCADE_DOWN

    // Timing (milliseconds)
    uint16_t cascadeRowDelay;       // Delay between rows in cascade (default: 400)
    uint16_t sweepColDelay;         // Delay between cols in sweep (default: 200)
    uint16_t lightsOutRowDelay;     // Delay between rows in lights out (default: 800)
    uint16_t lightsOutPause;        // Pause after all red before white (default: 1000)

    // Heartbeat / error
    uint16_t heartbeatWarnSec;      // Warn after N sec no heartbeat (default: 30)
    uint16_t dataTimeoutSec;        // Error state after N sec no data (default: 300)

    // Session
    bool liveSessionActive;         // true = race/quali mode, enables data timeout
} MasterConfig;

// --- FUNCTIONS ---

void loadConfig();
// Load all settings from NVS. If missing/corrupt, use defaults.

void saveConfig();
// Save current config to NVS.

void updateSetting(const char* key, const char* value);
// Update a single setting by key name (from host "set" command).
// Validates value before applying.
// Saves to NVS after update.
//
// Keys: "homeOnStartup", "transition.pageScroll", "timing.cascadeDelay", etc.

void resetConfigToDefaults();
// Wipe NVS config and reload with defaults.
// Used for factory reset.

MasterConfig* getConfig();
// Return pointer to current config struct.

#endif
