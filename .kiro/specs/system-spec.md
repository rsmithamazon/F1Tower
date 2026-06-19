# F1 Split Flap Tower — System Specification

## 1. Physical System

| Attribute | Value |
|-----------|-------|
| Grid | 5 columns × 5 rows |
| Total modules | 25 |
| Flaps per module | 45 |
| Total flaps | 1,125 |
| Drum types | 3 (Col 0, Col 1, Cols 2–4) |
| Motor | 28BYJ-48 (2048 steps/rev, half-step) |
| Steps per flap | ~45.5 (use lookup table for exact values) |
| Homing | Hall effect sensor + magnet at flap 0 (black) |

## 2. Hardware Architecture

```
[Host PC/Pi] ──WiFi/WebSocket──→ [ESP32 Master] ──I2C 400kHz──→ [Pico Row 0]
                                                                  [Pico Row 1]
                                                                  [Pico Row 2]
                                                                  [Pico Row 3]
                                                                  [Pico Row 4]
```

| Component | Hardware | Qty | I2C Address |
|-----------|----------|-----|-------------|
| Master | ESP32 DevKit | 1 | — (bus master) |
| Row 0 | Raspberry Pi Pico | 1 | 0x10 |
| Row 1 | Raspberry Pi Pico | 1 | 0x11 |
| Row 2 | Raspberry Pi Pico | 1 | 0x12 |
| Row 3 | Raspberry Pi Pico | 1 | 0x13 |
| Row 4 | Raspberry Pi Pico | 1 | 0x14 |

### Pico Pin Assignments (per row controller)

| Function | Pins |
|----------|------|
| Motor 0 (Col 0) | GP6, GP7, GP8, GP9 |
| Motor 1 (Col 1) | GP10, GP11, GP12, GP13 |
| Motor 2 (Col 2) | GP14, GP15, GP16, GP17 |
| Motor 3 (Col 3) | GP18, GP19, GP20, GP21 |
| Motor 4 (Col 4) | GP22, GP26, GP27, GP28 |
| Hall sensors | GP0, GP1, GP2, GP3, GP4 |
| I2C (SDA/SCL) | GP4, GP5 |

## 3. Flap Position Mapping (Summary)

Canonical source: #[[flap_config.json]]

| Position | All Columns | Col 0+1 Only |
|----------|-------------|--------------|
| 0–7 | Base colors (8) | — |
| 8–17 | Numbers 0–9 | — |
| 18–22 | — | Team colors (5) |
| 23–25 | — | Future color spare (3) |

| Position | Col 0 | Col 1 | Col 2–4 |
|----------|-------|-------|---------|
| 18–43 | (see above + icons/symbols/animation) | (see above + logos/symbols/spare) | Letters A–Z |
| 26–27 | Trophy, Pit | Logos | Letters |
| 28–29 | +, - | Logos | Letters |
| 30–44 | Animation frames 1–15 | Logos + Symbols + Spare | Letters + Dash |

## 4. Communication Protocol

### 4.1 Host → ESP32 (WebSocket JSON)

```json
{
  "cmd": "display",
  "mode": "<mode_name>",
  "endState": "<end_state_name>",
  "transition": "<transition_type>",
  "data": { ... }
}
```

**Command types:**

| cmd | Purpose | Key fields |
|-----|---------|------------|
| `display` | Show content on the board | mode, endState, transition, data |
| `startup` | Run startup animation | — |
| `shutdown` | Run shutdown sequence | — |
| `heartbeat` | Liveness ping (every 10s) | timestamp |
| `query_status` | Request full board state | — |
| `set` | Change a setting | key, value |

**Data payload (display command):**

```json
{
  "rows": [
    {"pos": "1", "team": "ferrari",  "text": "NOR"},
    {"pos": "2", "team": "mclaren",  "text": "PIA"}
  ]
}
```

Or for single-row updates (race events):

```json
{
  "row": 1,
  "update": {"pos": "2", "team": "ferrari", "text": "PIT"}
}
```

### 4.2 ESP32 → Host (WebSocket JSON, reverse channel)

```json
{"event": "status", "rows": [[0,26,18,8,30], ...]}
{"event": "error", "code": "ESP_201", "msg": "Motor stall", "row": 2, "col": 3}
{"event": "heartbeat_ack"}
```

### 4.3 ESP32 → Pico (I2C)

| Command | Byte format | Description |
|---------|-------------|-------------|
| GOTO | `[0x01, col0, col1, col2, col3, col4]` | Move 5 motors to flap positions |
| HOME | `[0x02]` | Home all 5 motors |
| ANIMATE | `[0x03, col, startFlap, endFlap, intervalMs]` | Play frame sequence |
| REPORT | `[0x04]` | Request position dump on next read |

### 4.4 Pico → ESP32 (I2C read response)

| Bytes | Content |
|-------|---------|
| 1 byte (default) | Status: 0x00=idle, 0x01=moving, 0x02=error, 0xFF=not homed |
| 6 bytes (after REPORT) | [status, col0_pos, col1_pos, col2_pos, col3_pos, col4_pos] |

## 5. Display Modes

| # | Mode | End State | Rows Used |
|---|------|-----------|-----------|
| 1 | Name + Gap | driver_names + driver_gaps | 2 per driver |
| 2 | Names Only | driver_names | 5 drivers |
| 3 | Flip Name/Time | driver_names ↔ driver_gaps | 5 drivers (alternating) |
| 4 | Name + Tire | driver_names + tire_colors | 5 drivers |
| 5 | Standings Cycle | driver_names ↔ driver_points | Pages of 5 |
| 6 | Constructors | constructor_names ↔ constructor_points | Pages of 5 |
| 7 | Single Driver | single_driver_focus | 5 rows (1 driver detail) |
| 8 | Checkered | checkered_flag | Full board |
| 9 | Full Color | all_red / all_yellow / all_green / all_purple | Full board |
| 10 | Safety Car | safety_car / vsc | Full board |
| 11 | Lights Out | lights_out_sequence | Full board (animated) |
| 12 | Quali Elimination | quali_elimination | 5 drivers |
| 13 | Race Winner | race_winner | Full board |
| 14 | Fastest Lap | fastest_lap | Full board |

## 6. Transitions

| ID | Type | Needs Clean Start | Priority |
|----|------|-------------------|----------|
| T0 | none | No | — |
| T1 | instant | No | Any |
| T2 | cascade_down | No | Low |
| T3 | cascade_up | No | Low |
| T4 | sweep_lr | No | Low |
| T5 | in_place | No | Low |
| T6 | car_down | **Yes** (black) | Low |
| T7 | car_rainbow | **Yes** (black) | Low |
| T8 | car_color | **Yes** (black) | Low |
| T9 | rainbow | **Yes** (black) | Low |
| T10 | random | No | Low |
| T11 | lights_out | **Yes** (black) | Low |

**Priority override:** Flag commands (red, yellow) always use `instant` and override any in-progress transition.

## 7. Startup / Shutdown

### Startup

```
1. ESP32 boots → load config from NVS
2. Connect WiFi → start WebSocket server
3. Check homeOnStartup toggle:
   - ON:  home all row controllers (3-4 sec)
   - OFF: if stored positions valid AND all == 0 (black) → skip home
          else → home all
4. Board confirmed at all-black (flap 0)
5. Lights-out animation: red row-by-row → pause → all white
6. Transition to first display mode
```

### Shutdown

```
1. Send all rows to black (with configured transition)
2. Wait for all idle
3. Save state to NVS (all zeros)
4. Motors de-energize
```

## 8. Error Handling

Full reference: #[[error_codes.md]]

| Condition | Visual indicator | Trigger |
|-----------|-----------------|---------|
| Data timeout (5 min, live session) | All blue (flap 5) | ESP_103 |
| Motor stall | Brief row flash | ESP_201 |
| Normal operation | No indicator | — |

### Recovery Flow

```
Pico detects stall → STATUS_ERROR on I2C read
→ ESP32 logs ESP_201 → sends error to host → attempts re-home
→ If re-home succeeds → clear error → resume
→ If re-home fails → persistent error logged → manual intervention
```

## 9. Configuration

Stored in ESP32 NVS. Changeable via host `set` command.

| Setting | Default | Options |
|---------|---------|---------|
| homeOnStartup | true | true / false |
| transition.pageScroll | cascade_down | instant, cascade_down, cascade_up |
| transition.modeChange | car_rainbow | instant, cascade_down, rainbow, car_rainbow |
| transition.flagReturn | in_place | instant, cascade_up, in_place |
| transition.shutdown | cascade_down | cascade_down, cascade_up, instant |
| timing.cascadeRowDelay | 400ms | 100–1000 |
| timing.sweepColDelay | 200ms | 100–500 |
| timing.lightsOutRowDelay | 800ms | 400–1500 |
| timing.lightsOutPause | 1000ms | 500–3000 |
| heartbeat.warnSec | 30 | 10–120 |
| heartbeat.dataTimeoutSec | 300 | 60–600 |

## 10. Code Structure

### Host (Python)

| File | Responsibility |
|------|---------------|
| main.py | Entry point, event loop |
| config.py | Load/save settings |
| connection.py | WebSocket client, heartbeat, reconnect |
| f1_data.py | F1 data ingestion, session state |
| display_engine.py | Mode management, page scrolling, event → command |
| commands.py | Command message builders |
| logger.py | Structured logging with error codes |

### ESP32 Master (C++)

| File | Responsibility |
|------|---------------|
| master.ino | Setup + loop entry point |
| wifi_manager.h | WiFi connect/reconnect |
| websocket_server.h | WebSocket server, message routing |
| i2c_master.h | I2C bus, send targets, read status |
| display_state.h | 5×5 board state, NVS persistence |
| transition_engine.h | Transition state machine, timing |
| flap_lookup.h | Content → flap position conversion |
| config.h | Settings struct, NVS storage |
| error_handler.h | Error codes, logging, visual errors |
| startup.h | Boot sequence, shutdown |

### Pico Row Controller (C++)

| File | Responsibility |
|------|---------------|
| row_controller.ino | Setup + loop entry point |
| i2c_peripheral.h | I2C receive commands, respond to reads |
| pio_stepper.h | PIO hardware stepping, lookup table |
| motor_driver.h | Motor management, animation, stall detect |
| homing.h | Hall sensor homing sequence |
| position_tracker.h | Flash persistence, status byte |

## 11. Implementation Order

1. **Pico row controller** — single motor homing + goto. Testable standalone.
2. **ESP32 I2C** — send GOTO/HOME to one Pico, read status.
3. **ESP32 WebSocket** — receive manual commands from host.
4. **Host connection** — WebSocket client, send test commands.
5. **Transitions** — implement cascade, instant, then animated.
6. **Display modes** — standings, flags, then complex modes.
7. **F1 data** — live integration (last, can test everything without it).
