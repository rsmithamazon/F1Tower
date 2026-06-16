# 🏎️ Split Flap F1 Tower Display

A mechanical split-flap display themed around Formula 1 — showing live race data, driver standings, timing, and animated transitions with team livery colors and a stop-motion F1 car animation.

## Overview

- **4 columns × 10 rows** (40 split-flap modules)
- **50 flaps per module** (2,000 total flaps)
- Real-time F1 data: standings, lap times, pit stops, flags
- Animated transitions: rolling rainbow, car cascade, checkered wipe
- No custom PCBs — built entirely with off-the-shelf components

## Demo Sequences

| Sequence | Description |
|----------|-------------|
| Race Start | Red flood → lights out (all black) |
| Leaderboard Reveal | F1 car drives down col 1 + rainbow cascade → driver standings |
| Race Finish | Checkered wipe → car animation + rainbow → final results |
| Position Change | Live updates with configurable delay |
| Pit Stop | Driver name flips to "PIT" with team logo |
| Fastest Lap | Purple wave → driver + time |

## Architecture

```
[Master ESP32/Nano] ──I2C──→ [Nano 1] → 4 motors
                              [Nano 2] → 4 motors
                              [Nano 3] → 4 motors
                              ...
                              [Nano 10] → 4 motors
```

- **10–13 Arduino Nanos** coordinated via I2C bus
- **ULN2003 driver boards** (off-the-shelf blue breakout boards)
- **28BYJ-48 stepper motors** (one per module)
- **Hall effect sensors** for homing
- **ESP-01** on master for WiFi control (optional)
- **5V 15A PSU** for motor power

## Display Layout

### Left Column (Col 0)

| Flaps | Content |
|-------|---------|
| 0–9 | F1 car animation frames |
| 10–19 | Numbers 0–9 |
| 20–30 | Team logos (11 teams) |
| 31–43 | Colors (13) |
| 44–49 | Extras (blank, icons) |

### Right Columns (Cols 1–3)

| Flaps | Content |
|-------|---------|
| 0–25 | Letters A–Z |
| 26–35 | Numbers 0–9 |
| 36–48 | Colors (13) |
| 49 | Dash (-) |

Flap order is **transition-optimized** — common sequences (blank → car → number → logo) are adjacent on the drum to minimize spin time.

## Color System

13 colors serving 11 F1 teams + flags + tires + timing sectors:

| Color | Team | Also serves |
|-------|------|-------------|
| Rosso Corsa (Red) | Ferrari | Red flag, Soft tire |
| Papaya Orange | McLaren | — |
| Petronas Teal | Mercedes | — |
| Navy Blue | Red Bull | Blue flag |
| British Racing Green | Aston Martin | Green flag |
| BWT Pink | Alpine | — |
| Light Blue | Williams | Wet tire |
| White | Haas | Hard tire, Checkered |
| Purple | Racing Bulls | Sector 3, Fastest lap |
| Maroon Red | Audi | — |
| Gold/Yellow | Cadillac | Yellow flag, SC, Medium tire |
| Black | — | Blank/space, Checkered |
| Lime Green | — | Sector 2 PB, Inter tire, DRS |

## Session Modes

| Mode | Behavior |
|------|----------|
| Practice | Position + name → sector colors during lap → time after lap |
| Qualifying | Same as practice + green/red elimination markers |
| Sprint | Shortened race mode |
| Race | Lights out → live standings → events → finish celebration |

## Performance

| Metric | Value |
|--------|-------|
| Step delay | 1500–2000μs |
| Time per flap | 60–80ms |
| Full drum revolution | 3–4 seconds |
| Car animation (10 frames, 1 row) | 0.5–0.7 seconds |
| Car full descent (10 rows) | 4–7 seconds (depending on overlap) |

## Bill of Materials

| Item | Qty | Est. Cost |
|------|-----|-----------|
| Arduino Nano clones | 10–13 | $30 |
| ULN2003 + 28BYJ-48 combos | 40 | $60 |
| Hall effect sensors + magnets | 40 | $20 |
| 5V 15A PSU | 1 | $15 |
| ESP-01 WiFi module | 1 | $3 |
| 3D printer filament (PLA/PETG) | ~2–3kg | $50 |
| Flap material (cardstock/PVC) | 2000 flaps | $30 |
| Wiring, connectors, misc | — | $20 |
| **Total** | | **~$230** |

## Build Guide

### 1. Prototype (Single Unit)

1. Print frame, drum, motor mount from STL files
2. Wire Nano → ULN2003 → 28BYJ-48 + Hall sensor
3. Flash `display_modes.ino`
4. Cut 50 flaps from cardstock, print characters
5. Assemble and test homing + character positioning

### 2. Scale to Full Display

1. Print all 40 module frames (optimized for tighter spacing)
2. Laser cut or Cricut 2,000 flaps
3. Wire I2C bus between 10–13 Nanos
4. Flash master + slave firmware
5. Mount to frame, connect PSU
6. Configure WiFi + data feed

## File Structure

```
splitflap-f1/
├── README.md                  ← You are here
├── PROJECT_SUMMARY.md         ← Full project discussion + decisions
├── display_modes.ino          ← Firmware: end states, animations, helpers
├── color_config.json          ← Team/tire/flag colors with hex + flap indices
├── color_allocation.md        ← Color analysis and overlap mapping
├── flap_allocation.md         ← Detailed flap position mapping
└── end_states.md              ← All display modes, race events, sequences
```

## Inspirations

- [scottbez1/splitflap](https://github.com/scottbez1/splitflap) — PCB-based design with ESP32
- [davidkingsman/split-flap](https://github.com/davidkingsman/split-flap) — Simple Nano-per-unit design
- [JonnyBooker/split-flap](https://github.com/JonnyBooker/split-flap) — Updated fork, simplified
- [Arne](https://www.printables.com/model/1365640-4-letter-split-flap-display-with-rs232-uart-daisy) - 3d print files used