// ===========================================================
// WIFI MANAGER — ESP32 WiFi connection handling
// ===========================================================
// Connects to configured network, handles reconnection,
// reports connection status.
// ===========================================================

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

// --- FUNCTIONS ---

void initWiFi();
// Connect to WiFi using credentials from config (NVS).
// Blocks until connected or timeout.
// On failure: retry with backoff, log WIFI_CONNECT_FAILED.

bool isWiFiConnected();
// Return current WiFi connection status.

void checkWiFiReconnect();
// Called in loop. If disconnected, attempt reconnect.
// Does not block — uses non-blocking reconnect with millis() timer.

String getLocalIP();
// Return assigned IP address as string (for debug/logging).

#endif
