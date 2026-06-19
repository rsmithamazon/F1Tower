# 🏎️ Split Flap F1 Timing Tower

A mechanical split-flap display emulating the F1 timing tower — showing live positions, lap times, sector colors, pit stops, flags, and animated transitions with team livery colors.

## Overview

- **5 columns × 5 rows** (25 split-flap modules)
- **45 flaps per module** (1,125 total)
- 3 unique drum types (Col 0, Col 1, Cols 2–4)
- Real-time F1 data: standings, gaps, sectors, tires, flags
- Animated transitions: car cascade, rolling rainbow, checkered wipe
- No custom PCBs — built entirely with off-the-shelf components

## Display Layout

```
[Col 0]  [Col 1]  [Col 2] [Col 3] [Col 4]
 Pos/     Logo/    Text/   Text/   Text/
 Status   Color    Num     Num     Num
```

Example — Driver Standings:
```
[ 1] [🔴] [N] [O] [R]
[ 2] [◇ ] [L] [E] [C]
[ 3] [★ ] [V] [E] [R]
[ 4] [→ ] [H] [A] [M]
[ 5] [⬡ ] [N] [O] [R]
```

## Flap Allocation (per drum)

All drums share positions 0–17 (colors + numbers). Col 0 and Col 1 share 18–25 (team colors + future color reserve).

| Position | Col 0 | Col 1 | Col 2–4 |
|----------|-------|-------|---------|
| 0–7 | Base colors (8) | Base colors (8) | Base colors (8) |
| 8–17 | Numbers 0–9 | Numbers 0–9 | Numbers 0–9 |
| 18–22 | Team colors (5) | Team colors (5) | Letters A–E |
| 23–25 | Future color spare (3) | Future color spare (3) | Letters F–H |
| 26–29 | Icons + Symbols | Logos (4) | Letters I–L |
| 30–44 | Animation (15 frames) | Logos (7) + Symbols + Spare (6) | Letters M–Z + dash |

### Column Details

| | Col 0 | Col 1 | Col 2–4 |
|--|-------|-------|---------|
| Numbers 0–9 | ✅ | ✅ | ✅ |
| Letters A–Z | — | — | ✅ (26) |
| Team Logos | — | ✅ (11) | — |
| Car Animation | ✅ (15 frames) | — | — |
| Colors (base 8) | ✅ | ✅ | ✅ |
| Colors (team 5) | ✅ | ✅ | — |
| Future color reserve | ✅ (3) | ✅ (3) | — |
| Symbols | +, - | -, . | - |
| Icons | Trophy, P | — | — |
| Spare (general) | 0 | 6 | 0 |
| **Total** | **45** | **45** | **45** |

### Animation (Col 0)

15 car animation frames grouped as a contiguous block at positions 30–44. Frame 15 lands on the last flap, so a single forward sweep through the drum plays the full animation.

### Future Color Reserve (Col 0 + Col 1)

Positions 23–25 are held spare on both Col 0 and Col 1 at the same drum position. If new team colors are needed, both drums can be updated identically without shifting other content.

## Color System

### Base 8 (all columns)

| Color | Hex | Flags | Tires | Sectors | Teams |
|-------|-----|-------|-------|---------|-------|
| Black | #000000 | Black flag | — | — | Blank/space |
| Red | #DC0000 | Red flag | Soft | — | Ferrari, Audi |
| White | #FFFFFF | — | Hard | — | Haas |
| Yellow | #FFD700 | Yellow flag, SC | Medium | Slower | Cadillac |
| Green | #006F62 | Green flag | Intermediate | PB | Aston Martin |
| Blue | #1E41FF | Blue flag | Wet | — | Red Bull, Williams |
| Purple | #A020F0 | — | — | Fastest, FL | Racing Bulls |
| Orange | #FF8000 | — | — | — | McLaren |

### 5 Additional Team Colors (Col 0 + Col 1 only)

| Color | Hex | Team |
|-------|-----|------|
| Teal | #00D2BE | Mercedes |
| Pink | #FF87BC | Alpine |
| Light Blue | #005AFF | Williams |
| Maroon | #900000 | Audi |
| Gold | #C8A951 | Cadillac |

## Display Modes

| # | Mode | Description |
|---|------|-------------|
| 1 | Name + Gap | 2 rows per driver: name then gap time |
| 2 | Names Only | 5 drivers with position + logo + initials |
| 3 | Flip Name/Time | Same row alternates between name and time |
| 4 | Name + Tire/Status | Position + name + tire color on Col 4 |
| 5 | Standings Cycle | Pages through top 5/10/15/20, optional points toggle |
| 6 | Constructor Standings | Team logo + abbreviation, cycles to points |
| 7 | Single Driver Focus | Full details: team color bar, time, sectors |
| 8 | Checkered Flag | Alternating black/white |
| 9 | Full Board Color | All red/yellow/green/purple (flags + events) |
| 10 | Safety Car / VSC | Yellow + text |
| 11 | Lights Out | Red cascade → ALL WHITE = GO! |
| 12 | Quali Elimination | Green (advanced) / Red (eliminated) markers |
| 13 | Race Winner | Trophy + winner + podium |
| 14 | Fastest Lap | Purple flash → driver + time detail |

**Total end states: 22** (see `end_states_final.md`)

## Transitions

All transitions are **configurable** — user selects preferred style per event.

| Style | Description |
|-------|-------------|
| Instant | All 25 units flip simultaneously |
| Cascade Down/Up | Row by row, top→bottom or bottom→top |
| Sweep L→R | Column by column |
| In-Place Sequence | P1 resolves, then P2, then P3... |
| Car Down | Car animates down Col 0 (15 frames) |
| Car + Rainbow | Car descends, rainbow fills L→R behind |
| Car + Color | Car descends, selectable color fills behind |
| Rainbow | Rolling color wave across board |

### Key Behaviors

- **Flags** — always instant in, configurable out
- **Race events** (pit, tire) — single row/cell change
- **Page scroll** — configurable delay (default 5–8 sec), cascade or instant
- **Startup** — BLACK → WHITE (same as lights out), then selectable transition to first mode
- **Shutdown** — cascade to ALL BLACK

See `transitions.md` for full configuration options.

## Session Modes

| Session | Behavior |
|---------|----------|
| Practice | Position + name → sector colors during lap → time after |
| Qualifying | Same + green/red elimination markers per round |
| Sprint | Shortened race mode |
| Race | Lights out → live standings → events → finish |

## Race Events

| Event | Display | Duration |
|-------|---------|----------|
| Position change | Update with X sec delay | Configurable |
| Pit stop | Name → "PIT", logo stays | Until pit exit |
| Tire change | Tire color on Col 0 | 5 sec |
| Yellow/Red flag | Entire board that color | Until cleared |
| Safety car | Yellow + text | Until SC in |
| Blue/BW flag | Color on that driver | 5 sec |
| Finish | Checkered → car + rainbow → standings | Full sequence |

## Architecture

```
[Host PC/Pi]                          [Hardware]
 Python app ──WebSocket──→ [ESP32 Master] ──I2C──→ [Pico Row 0] → 5 motors
 (F1 data,                   (transitions,          [Pico Row 1] → 5 motors
  display logic,              flap lookup,           [Pico Row 2] → 5 motors
  mode management)            timing, state)         [Pico Row 3] → 5 motors
                                                     [Pico Row 4] → 5 motors
```

### Three-Layer System

| Layer | Hardware | Role |
|-------|----------|------|
| **Host** | PC or Raspberry Pi | Ingests F1 data, decides what to display, sends commands |
| **Master** | ESP32 | Receives commands via WiFi (WebSocket), manages transitions, sends I2C to row controllers |
| **Row Controllers** | 5× Raspberry Pi Pico (one per row) | Drives 5 stepper motors each via PIO, handles homing, reports position |

### Communication

| Link | Protocol | Direction |
|------|----------|-----------|
| Host → ESP32 | WebSocket (WiFi) | Bidirectional (commands down, status/errors up) |
| ESP32 → Picos | I2C (400kHz) | Master writes commands, reads status/positions |

### Hardware Per Row

- **1× Raspberry Pi Pico** (RP2040) — PIO state machines for jitter-free stepping
- **5× ULN2003 driver boards** (off-the-shelf)
- **5× 28BYJ-48 stepper motors**
- **5× Hall effect sensors** for homing (magnet at flap 0/black)
- **5V 10A PSU** (shared)

## Performance

| Metric | Value |
|--------|-------|
| Time per flap | 60–80ms |
| Full drum revolution (45 flaps) | 2.7–3.6 sec |
| Car animation (15 frames, 1 row) | 0.9–1.2 sec |
| Car full descent (5 rows) | 2–3.5 sec |

## Bill of Materials

| Item | Qty | Est. Cost |
|------|-----|-----------|
| ESP32 DevKit | 1 | $6 |
| Raspberry Pi Pico | 5 | $20 |
| ULN2003 + 28BYJ-48 combos | 25 | $38 |
| Hall effect sensors + magnets | 25 | $12 |
| 5V 10A PSU | 1 | $12 |
| 3D printer filament | ~1.5kg | $30 |
| Flap material (cardstock/PVC) | 1,125 flaps | $20 |
| Wiring, connectors, I2C bus | — | $15 |
| **Total** | | **~$155** |

## File Structure

```
F1 Tower/
├── README.md                       ← You are here
├── error_codes.md                  ← Error code reference (all layers)
├── flap_config.json                ← Flap position mapping per drum (firmware lookup)
├── color_config.json               ← Color definitions (hex, teams, tires, flags)
├── flap_allocation_v3.md           ← Flap allocation design rationale
├── end_states_final.md             ← All 22 end states + power on/off sequences
├── transitions.md                  ← Transition styles + configurable settings
├── display_modes_v2.md             ← Display mode details (5×5)
├── ideas.md                        ← Feature ideas and future plans
│
├── simulator/                      ← VISUAL SIMULATOR (Python + Pygame)
│   ├── main.py                     ← Entry point: Pygame window + WebSocket server
│   ├── board_state.py              ← Board state, transitions, command processing
│   ├── flap_renderer.py            ← Draw individual flap modules (colors, text, flip anim)
│   ├── ws_server.py                ← WebSocket server (same protocol as ESP32)
│   ├── test_commands.py            ← Send test commands to simulator
│   └── requirements.txt            ← pygame, websockets
│
├── host/                           ← HOST APPLICATION (Python, runs on PC/Pi)
│   ├── main.py                     ← Entry point: boot, connect, run loop
│   ├── config.py                   ← Load/save settings (ESP32 IP, mode prefs, scroll)
│   ├── connection.py               ← WebSocket client (connect, send, heartbeat, reconnect)
│   ├── f1_data.py                  ← F1 data ingestion (API/mock/replay)
│   ├── display_engine.py           ← Display logic (mode switching, page scroll, events)
│   ├── commands.py                 ← Command builders (display, startup, shutdown, etc.)
│   └── logger.py                   ← Logging with error codes
│
├── firmware/
│   ├── master/                     ← ESP32 MASTER FIRMWARE (Arduino/C++)
│   │   ├── master.ino              ← Entry point: setup + loop
│   │   ├── wifi_manager.h          ← WiFi connect/reconnect
│   │   ├── websocket_server.h      ← WebSocket server (receive commands, send status)
│   │   ├── i2c_master.h            ← I2C bus master (send targets, read status from Picos)
│   │   ├── display_state.h         ← Track 5×5 board state + NVS persistence
│   │   ├── transition_engine.h     ← Transition state machine (cascade, car, rainbow, etc.)
│   │   ├── flap_lookup.h           ← Content → flap position conversion
│   │   ├── config.h                ← Settings struct + NVS load/save
│   │   ├── error_handler.h         ← Error detection, logging, visual error states
│   │   └── startup.h               ← Boot sequence (home, lights out animation, shutdown)
│   │
│   └── row_controller/             ← PICO ROW CONTROLLER FIRMWARE (Arduino/C++ on RP2040)
│       ├── row_controller.ino      ← Entry point: setup + loop
│       ├── i2c_peripheral.h        ← I2C peripheral (receive commands, respond to reads)
│       ├── pio_stepper.h           ← PIO hardware stepper control (zero-jitter stepping)
│       ├── motor_driver.h          ← High-level motor management (move, animate, stall detect)
│       ├── homing.h                ← Hall sensor homing sequence
│       └── position_tracker.h      ← Persistent position storage + status reporting
│
└── Archive/
    ├── end_states.md               ← Earlier end states (superseded)
    └── end_states_and_sequences.md ← Earlier sequences (superseded)
```

## Key Config Files

| File | Purpose |
|------|---------|
| `flap_config.json` | Position-to-content mapping for all 3 drum types. Used by firmware to look up which flap position to target for any given content. Positions 0–17 aligned across all columns. |
| `color_config.json` | Color definitions with hex values, text contrast colors, team associations, and flag/tire/sector usage. |
| `error_codes.md` | Full error code reference across all 3 layers. Codes, descriptions, visual indicators, and recovery actions. |

## Command Protocol (Host → ESP32)

Commands are JSON over WebSocket. Every display command follows this format:

```json
{
  "cmd": "display",
  "mode": "standings",
  "endState": "driver_names",
  "transition": "cascade_down",
  "data": {
    "rows": [
      {"pos": "1", "team": "ferrari",  "text": "NOR"},
      {"pos": "2", "team": "mclaren",  "text": "PIA"},
      {"pos": "3", "team": "redbull",  "text": "VER"},
      {"pos": "4", "team": "mercedes", "text": "HAM"},
      {"pos": "5", "team": "williams", "text": "ALB"}
    ]
  }
}
```

Other commands: `startup`, `shutdown`, `heartbeat`, `query_status`, `set` (config change).

## Startup Sequence

```
1. ESP32 boots → load config from NVS
2. Connect WiFi → start WebSocket server
3. Check homeOnStartup toggle:
   - ON: home all Picos (3-4 sec)
   - OFF: check stored positions. If all at black → skip home. Else → home.
4. All motors confirmed at black (flap 0)
5. Lights-out animation: red row-by-row → pause → all white
6. Transition to first display mode
```

## Inspirations

- [scottbez1/splitflap](https://github.com/scottbez1/splitflap) — PCB-based, ESP32
- [davidkingsman/split-flap](https://github.com/davidkingsman/split-flap) — Simple Nano design
- [JonnyBooker/split-flap](https://github.com/JonnyBooker/split-flap) — Updated fork

## License

TBD
