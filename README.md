# F1 Tower

A split-flap display tower that shows live Formula 1 standings, race events, and animations. The display is arranged as a grid with a dedicated left column and three right columns, each driven by physical split-flap characters. It cycles through various end states (driver standings, constructor standings, race winner, etc.) connected by themed transition animations.

---

## Display Layout

```
┌──────┬───────────────┐
│ LOGO │ V E R         │  ← team logo + driver name
│  1   │ 1 : 2 3       │  ← position + lap time
│ LOGO │ H A M         │
│  2   │ 1 : 2 4       │
│  ●   │               │  ← team color dot / black
└──────┴───────────────┘
```

### Column Specifications

| Column | Flap Set | Details |
|--------|----------|---------|
| **1 (Left)** | 0–9 (10), Car animation (10), Team logos (11), Colors (up to 17), +/− (2) | ~50 flaps |
| **2–4 (Right)** | 0–9 (10), Letters A–Z (26), Colors (up to 13), Period/symbol | ~50 flaps |

---

## End States

| # | State | Left Column | Right Columns |
|---|-------|-------------|---------------|
| 1 | **Driver Standings** | Position 1–10 | Driver initials (VER, HAM, LEC…) |
| 2 | **Constructor Standings** | Team logos | Team abbreviation or points |
| 3 | **Checkered Flag** | ⬛⬜ alternating | ⬜⬛ alternating |
| 4 | **Logo + Color Rows** | Logo | Team color solid fill |
| 5 | **Race Winner** | Trophy + logo | Winner name + team color |
| 6 | **Sector Times** | Sector color | Lap times |
| 7 | **Lights Out** | All red → all black | Same |
| 8 | **Fastest Lap** | Purple | Driver + time |
| 9 | **Red Flag / Safety Car** | All red | "SAFETY" or red fill |
| 10 | **DRS Enabled** | Green | "DRS" |
| 11 | **Pit Alert** | Logo | "PIT" + lap window |
| 12 | **Off/Sleep** | All black | All black |

### Additional Display Modes

- **Gap to Leader** — +0.3, +1.2 shown in right columns
- **Tire Compound** — Red/Yellow/White indicator
- **Clock Mode** — Idle display when no session is active
- **Custom Message** — Free text / party mode
- **Championship Clinch** — Special gold animation

---

## Animations & Sequences

| Sequence | Transition | Resolves To |
|----------|------------|-------------|
| Leaderboard Reveal | Rainbow L→R + Car ↓ | Driver Standings |
| Constructor Reveal | Team color fill L→R | Constructor Standings |
| Race Finish | Checkered wipe | Winner Celebration |
| Lights Out | Positions → black → red rows (×5) → black → positions | Race Mode |
| Position Change | Car ↓ + team color | Updated Standings |
| Fastest Lap | Purple wave | Fastest Lap display |
| Red Flag | Red flood (instant) | Red Flag state |
| Team Parade | Row-by-row team color | Logo + Color Rows |

---

## Session Modes

| Mode | Behavior |
|------|----------|
| **Practice** | Position + Name by default → sector colors during lap → time after lap |
| **Qualifying** | Same as practice + GREEN (advanced) / RED (eliminated) after each round |
| **Sprint** | Same as race but shorter, fewer pits |
| **Race** | Lights out → running order (configurable) → events → checkered → car + rainbow → standings |

---

## Race Events (Temporary Overlays)

| Event | Display | Duration |
|-------|---------|----------|
| Position change | Update with configurable delay | Instant after delay |
| Pit stop | Name → "PIT", keep logo | Until pit exit |
| Tire change | Tire color on left column | 5 sec then revert |
| Yellow flag | Entire board yellow | Until cleared |
| Red flag | Entire board red | Until resumed |
| Safety car | Yellow + "SAFETY CAR" text | Until SC in |
| Blue / Black-white flag | Color on that driver's left column | 5 sec |
| DRS enabled | Green flash | 3 sec |
| **Finish** | Checkered → Car ↓ with rainbow → final standings | Full sequence |

---

## Tire Compound Colors

| Compound | Color |
|----------|-------|
| Soft | Red |
| Medium | Yellow |
| Hard | White |
| Intermediate | Green |
| Wet | Blue |

---

## Project Files

| File | Purpose |
|------|---------|
| `color-config.json` | Team color definitions and flap color mappings |
| `README.md` | This document |
