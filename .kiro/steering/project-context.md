# F1 Split Flap Tower — Project Context

## What This Is

A physical 5×5 split-flap display that emulates an F1 timing tower. 25 mechanical modules (45 flaps each) show live race positions, lap times, flags, and animated transitions.

## Architecture (3 Layers)

```
Host (Python, PC/Pi) ──WebSocket──→ ESP32 Master ──I2C──→ 5× Pico Row Controllers → 25 motors
```

- **Host**: Processes F1 data, decides what to display, sends commands. Language: Python.
- **ESP32 Master**: Receives commands over WiFi, manages transitions/timing, sends flap targets over I2C. Language: C++ (Arduino framework).
- **Pico Row Controllers**: One RP2040 per row. Drives 5 stepper motors via PIO hardware. Handles homing. Language: C++ (Arduino-pico framework).

## Key Design Decisions

- **Row controllers use PIO** for hardware-precise stepper timing (zero jitter, all 5 motors truly parallel).
- **Drums only spin forward.** To go "backward" to an earlier flap, go forward through a full revolution.
- **Flap positions 0–17 are identical across all drum types** (colors 0–7, numbers 8–17). This simplifies cross-column commands.
- **Positions 23–25 are reserved spare** on Col 0 and Col 1 (for future team colors).
- **Animation frames are contiguous** at positions 30–44 on Col 0. One forward sweep = full animation.
- **Transitions that need a clean start** (car down, rainbow, lights out) go to all-black first, then begin.
- **Fire-and-forget** for most motor commands. Row controllers report status on I2C read when polled.
- **Host sends semantic commands** (`{mode, endState, transition, data}`). ESP32 converts to flap positions. Picos just receive "go to flap N."

## Terminology

| Term | Meaning |
|------|---------|
| Row Controller | Raspberry Pi Pico that drives one row (5 motors). Formerly "slave". |
| Master | ESP32 that orchestrates the display. |
| Host | PC/Pi running Python that processes F1 data and sends display commands. |
| Flap position | 0–44 index on the drum. Each position shows specific content (color, letter, number, logo, etc). |
| End state | What the board looks like when done transitioning (22 defined states). |
| Transition | How the board changes from one end state to another (10 styles). |
| Drum type | There are 3 unique drum layouts: Col 0, Col 1, Cols 2–4. |

## Conventions

- **No use of the word "slave"** — use "row controller" or "peripheral" (I2C context).
- **Error codes follow HTTP-style pattern** `[LAYER]_[CODE]`: 2xx = success/info, 4xx = logic errors, 5xx = system failures. E.g. ESP_504, PICO_500, HOST_501.
- **File organization**: Host = Python modules. Firmware = Arduino .ino + .h header files per concern.
- **JSON configs** (flap_config.json, color_config.json) are the source of truth for flap mappings. Firmware has a compiled-in copy for speed, but these JSON files are canonical.
- **Comments in pseudocode files** describe intent and logic. Real implementation replaces the comments.

## File References

- #[[flap_config.json]] — Flap position mappings for all drum types
- #[[color_config.json]] — Color definitions, team associations, hex values
- #[[error_codes.md]] — Full error code reference across all layers
- #[[transitions.md]] — Transition styles, timing, configuration
- #[[end_states_final.md]] — All 22 end states with visual layouts

## Current State

The project is in **pseudocode/planning phase**. All code files contain function stubs with detailed comments describing what each function should do. No real implementation yet.

### What's Done
- Flap allocation finalized (flap_config.json)
- All display modes defined
- All transitions defined
- All end states defined
- Code structure laid out (host: 6 files, master: 9 files, row controller: 5 files)
- Error codes documented

### What's Next
- Implement Pico row controller firmware first (simplest, testable with single motor)
- Then ESP32 master (I2C communication, basic commands)
- Then host (WebSocket connection, manual commands)
- Then transitions and display logic
- F1 live data integration is last (can test everything without it)
