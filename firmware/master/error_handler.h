// ===========================================================
// ERROR HANDLER — Error detection, logging, and recovery
// ===========================================================
// Monitors system health: WiFi, host connection, I2C row controllers.
// Logs errors with codes, triggers visual error states,
// forwards errors to host when possible.
// ===========================================================

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdint.h>

// --- ERROR CODES (see error_codes.md for full reference) ---

// ESP32 system errors (ESP_0xx)
#define ERR_WIFI_CONNECT_FAILED   "ESP_001"
#define ERR_WIFI_DISCONNECTED     "ESP_002"
#define ERR_WIFI_RECONNECTED      "ESP_003"  // info, not error
#define ERR_NVS_READ_FAILED       "ESP_004"
#define ERR_NVS_WRITE_FAILED      "ESP_005"

// Host connection errors (ESP_1xx)
#define ERR_HOST_CONNECT_FAILED   "ESP_100"
#define ERR_HOST_DISCONNECTED     "ESP_101"
#define ERR_HOST_TIMEOUT_WARN     "ESP_102"  // no heartbeat 30s
#define ERR_HOST_TIMEOUT_ERROR    "ESP_103"  // no data 5 min (live session)
#define ERR_HOST_PARSE_ERROR      "ESP_104"  // invalid JSON from host

// I2C / Row controller errors (ESP_2xx)
#define ERR_ROW_NO_RESPONSE     "ESP_200"  // Pico didn't ACK
#define ERR_ROW_STALL           "ESP_201"  // Pico reported motor stall
#define ERR_ROW_NOT_HOMED       "ESP_202"  // Pico reports not homed
#define ERR_ROW_TIMEOUT         "ESP_203"  // Pico didn't reach target in expected time

// Transition errors (ESP_3xx)
#define ERR_TRANS_ABORTED         "ESP_300"  // Transition interrupted by higher priority
#define ERR_TRANS_INVALID_TYPE    "ESP_301"  // Unknown transition type received

// --- FUNCTIONS ---

void initErrorHandler();
// Reset error state, clear error log.

void logError(const char* code, const char* message);
// Log an error locally (circular buffer in RAM).
// Also sends to host if WebSocket connected.
//
// Args:
//   code: error code string (e.g. "ESP_201")
//   message: human-readable detail

void logInfo(const char* message);
// Log informational event (non-error). Debug use.

void triggerErrorState(const char* code);
// Trigger a visual error indicator on the board.
//
// ESP_103 (data timeout in live session): all blue
// ESP_200/201 (slave error): flash affected row briefly
// Others: no visual change (just log)

void clearErrorState();
// Clear any active visual error state.
// Return board to last known good display.

uint8_t getErrorCount();
// Return number of errors in current session.

void getRecentErrors(char* buffer, uint16_t maxLen);
// Dump recent error log as string (for status report to host).

void handleErrorStates();
// Called in loop(). Checks for ongoing error conditions.
// Auto-recovers where possible (e.g. WiFi reconnect clears WiFi error).

#endif
