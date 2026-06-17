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

## Column Specs

| | Col 0 | Col 1 | Col 2–4 |
|--|-------|-------|---------|
| Numbers 0–9 | ✅ | ✅ | ✅ |
| Letters A–Z | — | — | ✅ |
| Team Logos | — | ✅ (11) | — |
| Car Frames | ✅ (5+5 spare) | — | — |
| Colors (base 8) | ✅ | ✅ | ✅ |
| Colors (team 5) | ✅ | ✅ | — |
| Symbols | +, - | -, . | - |
| Icons | 🏆, P | — | — |
| Spare | 8 | 9 | 0 |

## Color System

### Base 8 (all columns)

| Color | Flags | Tires | Sectors | Teams |
|-------|-------|-------|---------|-------|
| Red | Red flag | Soft | — | Ferrari, Audi |
| White | — | Hard | — | Haas |
| Black | Black flag | — | — | Blank/space |
| Yellow | Yellow flag, SC | Medium | Slower | Cadillac |
| Green | Green flag | Intermediate | PB | Aston Martin |
| Blue | Blue flag | Wet | — | Red Bull, Williams |
| Purple | — | — | Fastest, FL | Racing Bulls |
| Orange | — | — | — | McLaren |

### 5 Additional Team Colors (Col 0 + Col 1 only)

Teal (Mercedes), Pink (Alpine), Light Blue (Williams), Maroon (Audi), Gold (Cadillac)

### Drum Color Order (dark → light)

```
Black → Red → Blue → Green → Purple → Orange → Yellow → White
```

Ensures forward flips for key transitions: startup (black→white), lights out (red→white).

## Display Modes

| # | Mode | Description |
|---|------|-------------|
| 1 | Name + Gap | 2 rows per driver: name then gap time |
| 2 | Names Only | 5 drivers with position + logo + initials |
| 3 | Flip Name ↔ Time | Same row alternates between name and time |
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
| Car Down | Car animates down Col 0 |
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
[Master ESP32/Nano] ──I2C──→ [Nano 1] → 4–5 motors
                              [Nano 2] → 4–5 motors
                              ...
                              [Nano 6] → 4–5 motors
```

- **~6 Arduino Nanos** (25 modules ÷ 4–5 each)
- **ULN2003 driver boards** (off-the-shelf)
- **28BYJ-48 stepper motors**
- **Hall effect sensors** for homing
- **5V 10A PSU**

## Performance

| Metric | Value |
|--------|-------|
| Time per flap | 60–80ms |
| Full drum revolution (45 flaps) | 2.7–3.6 sec |
| Car animation (5 frames, 1 row) | 0.3–0.4 sec |
| Car full descent (5 rows) | 2–3.5 sec |

## Bill of Materials

| Item | Qty | Est. Cost |
|------|-----|-----------|
| Arduino Nano clones | 6–7 | $18 |
| ULN2003 + 28BYJ-48 combos | 25 | $38 |
| Hall effect sensors + magnets | 25 | $12 |
| 5V 10A PSU | 1 | $12 |
| ESP-01 WiFi module | 1 | $3 |
| 3D printer filament | ~1.5kg | $30 |
| Flap material (cardstock/PVC) | 1,125 flaps | $20 |
| Wiring, connectors | — | $15 |
| **Total** | | **~$150** |

## File Structure

```
splitflap-f1/
├── README.md                    ← You are here
├── PROJECT_SUMMARY.md           ← Full project discussion + decisions
├── flap_allocation_v3.md        ← Final flap positions per column (45 flaps)
├── end_states_final.md          ← All 22 end states + power on/off sequences
├── transitions.md               ← All transition styles + configurable settings
├── display_modes_v2.md          ← Display mode details (5×5)
├── color_config.json            ← Color definitions (hex, indices, teams, tires, flags)
├── display_modes.ino            ← Firmware: states + animations (WIP)
├── color_allocation.md          ← Color analysis (reference)
├── end_states.md                ← Earlier end states (superseded by final)
├── end_states_and_sequences.md  ← Earlier sequences (superseded)
└── flap_allocation.md           ← Original allocation (superseded by v3)
```

## Inspirations

- [scottbez1/splitflap](https://github.com/scottbez1/splitflap) — PCB-based, ESP32
- [davidkingsman/split-flap](https://github.com/davidkingsman/split-flap) — Simple Nano design
- [JonnyBooker/split-flap](https://github.com/JonnyBooker/split-flap) — Updated fork

## License

TBD
