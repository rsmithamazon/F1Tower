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
| Car animation (15 frames, 1 row) | 0.9–1.2 sec |
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
F1 Tower/
├── README.md                  ← You are here
├── flap_config.json           ← Flap position mapping per drum (firmware lookup)
├── color_config.json          ← Color definitions (hex, indices, teams, tires, flags)
├── flap_allocation_v3.md      ← Flap allocation design rationale
├── end_states_final.md        ← All 22 end states + power on/off sequences
├── transitions.md             ← All transition styles + configurable settings
├── display_modes_v2.md        ← Display mode details (5×5)
├── ideas.md                   ← Feature ideas and future plans
└── Archive/
    ├── end_states.md          ← Earlier end states (superseded)
    └── end_states_and_sequences.md ← Earlier sequences (superseded)
```

## Key Config Files

| File | Purpose |
|------|---------|
| `flap_config.json` | Position-to-content mapping for all 3 drum types. Used by firmware to look up which flap position to target for any given content (color, number, letter, logo, animation frame). Positions 0–17 are aligned across all columns. |
| `color_config.json` | Color definitions with hex values, text contrast colors, team associations, and flag/tire/sector usage. |

## Inspirations

- [scottbez1/splitflap](https://github.com/scottbez1/splitflap) — PCB-based, ESP32
- [davidkingsman/split-flap](https://github.com/davidkingsman/split-flap) — Simple Nano design
- [JonnyBooker/split-flap](https://github.com/JonnyBooker/split-flap) — Updated fork

## License

TBD
