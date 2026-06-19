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

// Success / Info (ESP_2xx)
#define INFO_WIFI_CONNECTED       "ESP_200"
#define INFO_WIFI_RECONNECTED     "ESP_201"
#define INFO_HOST_CONNECTED       "ESP_202"
#define INFO_HOST_RECONNECTED     "ESP_203"
#define INFO_ALL_HOMED            "ESP_204"
#define INFO_STARTUP_COMPLETE     "ESP_205"

// Client / Logic errors (ESP_4xx)
#define ERR_CMD_PARSE_ERROR       "ESP_400"  // invalid JSON from host
#define ERR_CMD_UNKNOWN           "ESP_401"  // unrecognized cmd field
#define ERR_TRANSITION_INVALID    "ESP_402"  // unknown transition type
#define ERR_SETTING_INVALID       "ESP_403"  // bad key/value in set command
#define ERR_ROW_NOT_FOUND         "ESP_404"  // row index out of range
#define ERR_FLAP_OUT_OF_RANGE     "ESP_405"  // flap position > 44

// System / Hardware errors (ESP_5xx)
#define ERR_WIFI_CONNECT_FAILED   "ESP_500"
#define ERR_WIFI_DISCONNECTED     "ESP_501"
#define ERR_HOST_DISCONNECTED     "ESP_502"
#define ERR_HOST_TIMEOUT_WARN     "ESP_503"  // no heartbeat 30s
#define ERR_HOST_TIMEOUT_ERROR    "ESP_504"  // no data 5 min (live session)
#define ERR_ROW_NO_RESPONSE       "ESP_510"  // Pico didn't ACK
#define ERR_ROW_STALL             "ESP_511"  // Pico reported motor stall
#define ERR_ROW_NOT_HOMED         "ESP_512"  // Pico reports not homed
#define ERR_ROW_TIMEOUT           "ESP_513"  // Pico didn't reach target in expected time
#define ERR_NVS_READ_FAILED       "ESP_520"
#define ERR_NVS_WRITE_FAILED      "ESP_521"
#define ERR_TRANS_ABORTED         "ESP_530"  // higher priority interrupted

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
