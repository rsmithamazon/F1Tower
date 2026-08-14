# F1 Split Flap Tower — Error Codes Reference

All error codes used across the system. Follows HTTP-style numbering convention:
- **2xx** = Success / Informational
- **4xx** = Client or logic error (bad input, invalid state, parse failure)
- **5xx** = System or hardware failure (connectivity, motor, timeout)

---

## Format

```
[LAYER]_[CODE]
```

- **Layer**: `ESP` (ESP32 master), `PICO` (Pico row controller), `HOST` (Python host)
- **Code**: 3-digit number following HTTP convention

---

## ESP32 Master Codes

### Success / Info (ESP_2xx)

| Code | Name | Description |
|------|------|-------------|
| ESP_200 | WIFI_CONNECTED | WiFi connection established |
| ESP_201 | WIFI_RECONNECTED | WiFi connection restored after drop |
| ESP_202 | HOST_CONNECTED | Host WebSocket client connected |
| ESP_203 | HOST_RECONNECTED | Host reconnected after disconnect |
| ESP_204 | ALL_ROWS_HOMED | All row controllers homed successfully |
| ESP_205 | STARTUP_COMPLETE | Startup sequence finished |
| ESP_206 | SHUTDOWN_COMPLETE | Clean shutdown finished, state saved |

### Client / Logic Errors (ESP_4xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| ESP_400 | CMD_PARSE_ERROR | Invalid/malformed JSON from host | Ignore bad message. Log for debug. |
| ESP_401 | CMD_UNKNOWN | Unrecognized "cmd" field in message | Ignore. Log. |
| ESP_402 | TRANSITION_INVALID | Unknown transition type requested | Fallback to instant. Log. |
| ESP_403 | SETTING_INVALID | Invalid key or value in "set" command | Reject. Send error back to host. |
| ESP_404 | ROW_NOT_FOUND | Row index out of range (0-4) | Ignore command. Log. |
| ESP_405 | FLAP_OUT_OF_RANGE | Flap position > 44 in computed target | Clamp to 44. Log warning. |

### System / Hardware Errors (ESP_5xx)

| Code | Name | Description | Visual | Recovery |
|------|------|-------------|--------|----------|
| ESP_500 | WIFI_CONNECT_FAILED | Could not connect to WiFi network | None (pre-display) | Retry with backoff. |
| ESP_501 | WIFI_DISCONNECTED | WiFi connection lost during operation | None | Auto-reconnect. |
| ESP_502 | HOST_DISCONNECTED | Host WebSocket connection dropped | None | Wait for reconnect. Show last state. |
| ESP_503 | HOST_TIMEOUT_WARN | No heartbeat from host for 30 seconds | None | Log warning. |
| ESP_504 | HOST_TIMEOUT_ERROR | No data for 5 min during live session | **All blue** | Hold blue until reconnect + new data. |
| ESP_510 | ROW_NO_RESPONSE | Pico didn't ACK on I2C write | None | Retry 3x. If persistent, skip row. |
| ESP_511 | ROW_STALL | Pico reported motor stall/error | Brief row flash | Re-home that row. |
| ESP_512 | ROW_NOT_HOMED | Pico reports not-homed state | None | Send home command. |
| ESP_513 | ROW_TIMEOUT | Pico didn't reach target in expected time | None | Log. Mechanical jam likely. |
| ESP_520 | NVS_READ_FAILED | Could not read config/state from flash | None | Use defaults. |
| ESP_521 | NVS_WRITE_FAILED | Could not write config/state to flash | None | Retry once. Non-critical. |
| ESP_530 | TRANSITION_ABORTED | Transition interrupted by higher priority | None | Expected (flag override). Log for audit. |

---

## Pico Row Controller Codes

### Success / Info (PICO_2xx)

| Code | Name | Description |
|------|------|-------------|
| PICO_200 | HOMED_OK | All motors homed successfully |
| PICO_201 | TARGETS_REACHED | All motors reached commanded positions |
| PICO_202 | ANIMATION_COMPLETE | Frame sequence finished |

### Client / Logic Errors (PICO_4xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| PICO_400 | CMD_INVALID | Unrecognized command byte received | Ignore. Report on next status read. |
| PICO_401 | FLAP_OUT_OF_RANGE | Target flap position > 44 | Ignore. Report error. |
| PICO_402 | CMD_WHILE_HOMING | Received GOTO while homing in progress | Queue command, execute after homing. |

### System / Hardware Errors (PICO_5xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| PICO_500 | MOTOR_STALL | Motor didn't complete steps in expected time | Report to master. Re-home this motor. |
| PICO_501 | HALL_NO_TRIGGER | Hall sensor never triggered during homing | Motor or sensor failure. Report. |
| PICO_502 | HALL_STUCK | Hall sensor continuously triggered | Sensor failure. Report. Cannot home. |
| PICO_510 | FLASH_READ_FAILED | Could not read stored positions | Set positions to UNKNOWN. Force home. |
| PICO_511 | FLASH_WRITE_FAILED | Could not write positions to flash | Non-critical. Positions lost on power cycle. |
| PICO_520 | PIO_INIT_FAILED | PIO state machine failed to initialize | Fatal. Requires reflash or hardware check. |

---

## Host (Python) Codes

### Success / Info (HOST_2xx)

| Code | Name | Description |
|------|------|-------------|
| HOST_200 | ESP32_CONNECTED | WebSocket connection established |
| HOST_201 | ESP32_RECONNECTED | Reconnected after disconnect |
| HOST_202 | API_CONNECTED | F1 data source connected |
| HOST_203 | SESSION_STARTED | Live session data flowing |

### Client / Logic Errors (HOST_4xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| HOST_400 | CONFIG_INVALID | Config file missing required fields | Exit with error detail. |
| HOST_401 | MODE_UNKNOWN | Unrecognized display mode requested | Stay in current mode. Log. |
| HOST_402 | DRIVER_NOT_FOUND | Driver code not in standings data | Skip. Data sync issue. |
| HOST_403 | API_PARSE_ERROR | Invalid data format from F1 source | Skip bad message. Log. |

### System Errors (HOST_5xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| HOST_500 | CONFIG_LOAD_FAILED | Could not read/parse config.json | Exit with message. |
| HOST_501 | ESP32_CONNECT_FAILED | Could not establish WebSocket | Retry with backoff. |
| HOST_502 | ESP32_DISCONNECTED | WebSocket to ESP32 lost | Auto-reconnect. Re-send full state. |
| HOST_503 | ESP32_SEND_FAILED | Failed to send command | Queue. Retry on reconnect. |
| HOST_510 | API_CONNECT_FAILED | Could not connect to F1 data source | Retry. Manual mode fallback. |
| HOST_511 | API_DISCONNECTED | F1 data source connection dropped | Auto-reconnect. |
| HOST_512 | DATA_STALE | No new data from F1 source for extended period | Log warning. Session may have ended. |

---

## Error Severity Levels

| Level | Meaning | Action |
|-------|---------|--------|
| **FATAL** | System cannot continue | Halt/restart required |
| **ERROR** | Something failed, needs attention | Log, attempt recovery, report |
| **WARN** | Unexpected but recoverable | Log, continue |
| **INFO** | Notable event (not an error) | Log for audit |

---

## Visual Error States

| Condition | Display | Trigger | Clear |
|-----------|---------|---------|-------|
| Data timeout (live session) | All 25 units → BLUE (flap 5) | ESP_504 | New data received |
| Motor stall (single row) | Affected row flashes briefly | ESP_511 | After successful re-home |
| Normal operation | No indicator | — | — |

---

## Log Format

### ESP32 (Serial + WebSocket to host)

```
[LEVEL] [CODE] [TIMESTAMP_MS] message {context}
```

Example:
```
[ERROR] [ESP_511] [1234567] Motor stall detected {row:2, col:3}
[WARN]  [ESP_503] [1235000] No heartbeat from host for 30s
[INFO]  [ESP_201] [1240000] WiFi reconnected {ip:192.168.1.50}
```

### Host (Python logging)

```
[LEVEL] [CODE] [ISO_TIMESTAMP] message {context}
```

Example:
```
[ERROR] [HOST_502] [2024-03-15T14:30:22] ESP32 WebSocket disconnected {ip:192.168.1.50}
[WARN]  [HOST_512] [2024-03-15T14:35:22] No new F1 data for 60s {source:openf1}
[INFO]  [HOST_200] [2024-03-15T14:36:00] Connected to ESP32
```

---

## Error Flow

```
Pico detects stall → STATUS_ERROR on next I2C read → reports PICO_500
    ↓
ESP32 reads status → logs ESP_511 → sends error event to host
    ↓
Host receives error → logs HOST layer context → displays in dashboard
    ↓
ESP32 attempts recovery (re-home affected motor)
    ↓
Success → PICO_200 reported → ESP_204 logged → clear error
Failure → persistent error → manual intervention needed
```
