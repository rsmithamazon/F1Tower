# F1 Split Flap Tower — Error Codes Reference

All error codes used across the system. Each code identifies the source layer, category, and specific issue.

---

## Format

```
[LAYER]_[CATEGORY][NUMBER]
```

- **Layer**: `ESP` (ESP32 master), `PICO` (Pico row controller), `HOST` (Python host)
- **Category**: numeric group (0xx = system, 1xx = connection, 2xx = hardware, 3xx = logic)
- **Number**: specific error within category

---

## ESP32 Master Errors

### System (ESP_0xx)

| Code | Name | Description | Visual | Recovery |
|------|------|-------------|--------|----------|
| ESP_001 | WIFI_CONNECT_FAILED | Could not connect to configured WiFi network | None (pre-display) | Retry with backoff. Check credentials. |
| ESP_002 | WIFI_DISCONNECTED | WiFi connection lost during operation | None | Auto-reconnect. Log event. |
| ESP_003 | WIFI_RECONNECTED | WiFi connection restored (info, not error) | None | Clear WiFi error state. |
| ESP_004 | NVS_READ_FAILED | Could not read config/state from flash | None | Use defaults. Log warning. |
| ESP_005 | NVS_WRITE_FAILED | Could not write config/state to flash | None | Retry once. Non-critical if transient. |

### Host Connection (ESP_1xx)

| Code | Name | Description | Visual | Recovery |
|------|------|-------------|--------|----------|
| ESP_100 | HOST_CONNECT_FAILED | Host WebSocket client never connected | None | Wait for connection. |
| ESP_101 | HOST_DISCONNECTED | Host WebSocket connection dropped | None | Wait for reconnect. Continue showing last state. |
| ESP_102 | HOST_TIMEOUT_WARN | No heartbeat from host for 30 seconds | None | Log warning. Host may have crashed. |
| ESP_103 | HOST_TIMEOUT_ERROR | No data for 5 minutes during live session | **All blue** | Hold blue state until reconnect + new data. |
| ESP_104 | HOST_PARSE_ERROR | Invalid/malformed JSON received from host | None | Ignore bad message. Log for debug. |

### I2C / Row Controller Communication (ESP_2xx)

| Code | Name | Description | Visual | Recovery |
|------|------|-------------|--------|----------|
| ESP_200 | ROW_NO_RESPONSE | Pico didn't ACK on I2C write | None | Retry 3x. If persistent, log and skip row. |
| ESP_201 | ROW_STALL | Pico reported motor stall/error | Brief flash on affected row | Re-home that row. |
| ESP_202 | ROW_NOT_HOMED | Pico reports not-homed state | None | Send home command to that row. |
| ESP_203 | ROW_TIMEOUT | Pico didn't reach target in expected time | None | Log. May indicate mechanical jam. |

### Transition (ESP_3xx)

| Code | Name | Description | Visual | Recovery |
|------|------|-------------|--------|----------|
| ESP_300 | TRANS_ABORTED | Transition interrupted by higher-priority command | None | Expected behavior (flag override). Log for audit. |
| ESP_301 | TRANS_INVALID_TYPE | Unknown transition type in command | None | Fallback to TRANS_INSTANT. Log error. |

---

## Pico Slave Errors

### System (PICO_0xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| PICO_001 | FLASH_READ_FAILED | Could not read stored positions from flash | Set all positions to UNKNOWN. Force home. |
| PICO_002 | FLASH_WRITE_FAILED | Could not write positions to flash | Non-critical. Positions lost on next power cycle. |
| PICO_003 | PIO_INIT_FAILED | PIO state machine failed to initialize | Fatal. Requires reflash or hardware check. |

### Motor / Hardware (PICO_1xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| PICO_100 | MOTOR_STALL | Motor didn't complete steps in expected time | Report to master. Re-home this motor. |
| PICO_101 | MOTOR_OVERCURRENT | Unexpected current draw (if sensing available) | Stop motor. Report error. |
| PICO_102 | HALL_NO_TRIGGER | Hall sensor never triggered during homing (full revolution + extra) | Motor or sensor failure. Report to master. |
| PICO_103 | HALL_STUCK | Hall sensor continuously triggered (magnet stuck or sensor failure) | Report to master. Cannot home reliably. |

### Command (PICO_2xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| PICO_200 | CMD_INVALID | Unrecognized command byte received | Ignore. Report to master on next status read. |
| PICO_201 | CMD_OUT_OF_RANGE | Target flap position > 44 | Ignore. Report error. |
| PICO_202 | CMD_WHILE_HOMING | Received GOTO while homing in progress | Queue command, execute after homing completes. |

---

## Host (Python) Errors

### System (HOST_0xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| HOST_001 | CONFIG_LOAD_FAILED | Could not read/parse config.json | Exit with error message. |
| HOST_002 | CONFIG_INVALID | Config file missing required fields | Exit with details of missing fields. |

### Connection (HOST_1xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| HOST_100 | ESP32_CONNECT_FAILED | Could not establish WebSocket to ESP32 | Retry with backoff. Check IP/port. |
| HOST_101 | ESP32_DISCONNECTED | WebSocket connection to ESP32 lost | Auto-reconnect. Re-send full state on reconnect. |
| HOST_102 | ESP32_SEND_FAILED | Failed to send command (connection issue) | Queue command. Retry on reconnect. |

### Data Source (HOST_2xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| HOST_200 | API_CONNECT_FAILED | Could not connect to F1 data source | Retry. Operate without live data (manual mode). |
| HOST_201 | API_DISCONNECTED | F1 data source connection dropped | Auto-reconnect. |
| HOST_202 | API_PARSE_ERROR | Invalid data format from F1 source | Skip bad message. Log for debug. |
| HOST_203 | DATA_STALE | No new data from F1 source for extended period | Log warning. May indicate session ended or API issue. |

### Display Logic (HOST_3xx)

| Code | Name | Description | Recovery |
|------|------|-------------|----------|
| HOST_300 | MODE_UNKNOWN | Attempted to switch to unrecognized display mode | Stay in current mode. Log error. |
| HOST_301 | DRIVER_NOT_FOUND | Referenced driver code doesn't exist in standings | Skip. May indicate data sync issue. |

---

## Error Severity Levels

| Level | Meaning | Action |
|-------|---------|--------|
| **FATAL** | System cannot continue | Halt/restart required |
| **ERROR** | Something failed, needs attention | Log, attempt recovery, report to user |
| **WARN** | Unexpected but recoverable | Log, continue operation |
| **INFO** | Notable event (not an error) | Log for audit trail |

---

## Visual Error States

| Condition | Display | Trigger | Clear |
|-----------|---------|---------|-------|
| Data timeout (live session) | All 25 units → BLUE (flap 5) | ESP_103: no data 5 min in race/quali | New data received from host |
| Slave error (single row) | Affected row flashes briefly | ESP_201: motor stall | After successful re-home |
| No errors | Normal display | — | — |

---

## Error Logging Format

### ESP32 (Serial + WebSocket to host)

```
[LEVEL] [CODE] [TIMESTAMP_MS] message {context}
```

Example:
```
[ERROR] [ESP_201] [1234567] Motor stall detected {row:2, col:3}
[WARN]  [ESP_102] [1235000] No heartbeat from host for 30s
[INFO]  [ESP_003] [1240000] WiFi reconnected {ip:192.168.1.50}
```

### Host (Python logging)

```
[LEVEL] [CODE] [ISO_TIMESTAMP] message {context}
```

Example:
```
[ERROR] [HOST_101] [2024-03-15T14:30:22] ESP32 WebSocket disconnected {ip:192.168.1.50}
[WARN]  [HOST_203] [2024-03-15T14:35:22] No new F1 data for 60s {source:openf1}
[INFO]  [HOST_100] [2024-03-15T14:36:00] Reconnected to ESP32
```

---

## Error Flow

```
Pico detects stall → reports STATUS_ERROR on next I2C read
    ↓
ESP32 reads status → logs ESP_201 → sends error event to host
    ↓
Host receives error → logs with HOST layer context → displays in dashboard
    ↓
ESP32 attempts recovery (re-home affected motor)
    ↓
If recovery succeeds → clear error, resume normal operation
If recovery fails → escalate (log persistent error, may need manual intervention)
```
