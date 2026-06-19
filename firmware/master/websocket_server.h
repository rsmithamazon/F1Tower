// ===========================================================
// WEBSOCKET SERVER — Handles host communication
// ===========================================================
// ESP32 runs a WebSocket SERVER. Host connects as client.
// Receives commands, sends status/errors back to host.
// ===========================================================

#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

// --- INITIALIZATION ---

void initWebSocket();
// Start WebSocket server on configured port (default 81).
// Register event handlers for connect/disconnect/message.

// --- MESSAGE HANDLING ---

void handleWebSocket();
// Called in loop(). Processes incoming WebSocket events.
// Routes messages to appropriate handler based on "cmd" field.

void onMessageReceived(const char* payload);
// Parse incoming JSON command from host.
// Expected fields: cmd, mode, endState, transition, data.
//
// Command routing:
//   "display"      → processDisplayCommand()
//   "startup"      → runStartupSequence()
//   "shutdown"     → runShutdownSequence()
//   "heartbeat"    → resetHeartbeatTimer()
//   "query_status" → sendBoardStatus()
//   "set"          → updateSetting()

void processDisplayCommand(/* parsed JSON */);
// Extract mode, endState, transition, data from command.
// Convert row data to flap positions using flap_lookup.
// Pass to transition engine for execution.

// --- OUTGOING MESSAGES ---

void sendToHost(const char* jsonMessage);
// Send a message back to host over WebSocket.
// Used for status reports, errors, heartbeat ACKs.

void sendBoardStatus();
// Query all Pico slaves for current positions.
// Build status JSON and send to host.
// Format: {"status": "ok", "rows": [[pos,pos,pos,pos,pos], ...]}

void sendError(const char* code, const char* message);
// Send error event to host for logging.
// Format: {"event": "error", "code": "ESP_001", "msg": "..."}

// --- HEARTBEAT ---

void resetHeartbeatTimer();
// Called when heartbeat received from host. Resets timeout counter.

void checkHeartbeat();
// Called in loop(). If no heartbeat for 30s → warn.
// If no data for 5 min during live session → trigger blue error state.

#endif
