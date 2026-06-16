# Split Flap F1 Display — End States, Modes & Sequences

## Layout Reference
- 4 columns × 10 rows
- Col 0: Left (numbers, logos, car frames, colors, icons)
- Cols 1–3: Right (letters, numbers, colors, dash)

---

## SESSION MODES

### Practice Mode

| State | Left Col | Right Cols | Trigger |
|-------|----------|------------|---------|
| Standings | Position 1–10 | Driver initials | Default between laps |
| During lap | Sector color (YEL/GRN/PUR) | Sector time as completed | Live timing feed |
| After lap | Position | Full lap time | Lap completed |

```
Default:
[ 1] [V] [E] [R]
[ 2] [H] [A] [M]
...

During lap (driver on hot lap):
[ 1] [V] [E] [R]
[PUR] [2] [8] [3]   ← S1 purple (fastest), showing 28.3
[GRN] [BLK][BLK][BLK] ← S2 in progress (green = PB so far)
[ 3] [L] [E] [C]
...

After lap:
[ 1] [V] [E] [R]
[ 2] [1] [-] [2]   ← lap time 1:23.xxx (truncated to fit)
[ 3] [L] [E] [C]
...
```

---

### Qualifying Mode

Same as practice PLUS elimination rounds:

| State | Left Col | Right Cols | Trigger |
|-------|----------|------------|---------|
| Live timing | Same as practice | Same as practice | During session |
| Eliminated | RED on left | Driver initials | Knocked out of Q1/Q2 |
| Advanced | GREEN on left | Driver initials | Made it through |

```
After Q1 cutoff:
[GRN] [V] [E] [R]   ← made Q2
[GRN] [H] [A] [M]
...
[GRN] [A] [L] [O]   ← P15 made it
[RED] [S] [T] [R]   ← P16 eliminated
[RED] [G] [A] [S]
[RED] [Z] [H] [O]
[RED] [B] [O] [T]
[RED] [S] [A] [R]
```

---

### Sprint Mode

Same as Race Mode (below) but shorter — no pit stops typically.

---

### Race Mode

#### Phase 1: Lights Out

```
[RED] [RED] [RED] [RED]    ← all red (5 lights)
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[BLK] [BLK] [BLK] [BLK]  ← bottom half dark
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]

Then ALL flip to black = GO!
```

#### Phase 2: Race Running (configurable display)

Setting A — Logo + Color + Position:
```
[RB LOGO ] [NVY] [NVY] [NVY]
[FER LOGO] [RED] [RED] [RED]
...
```

Setting B — Position + Name:
```
[ 1] [V] [E] [R]
[ 2] [H] [A] [M]
...
```

Setting C — Logo + Name:
```
[RB LOGO ] [V] [E] [R]
[FER LOGO] [H] [A] [M]
...
```

#### Race Events (temporary overlays)

| Event | Left Col | Right Cols | Duration |
|-------|----------|------------|----------|
| **Position change** | Update positions | Update names | X second delay before showing |
| **Pit stop** | Keep LOGO | [P] [I] [T] | Until driver exits pit |
| **Tire change** | TIRE COLOR | Return to name | 5 seconds then back |
| **Yellow flag** | ALL YELLOW | ALL YELLOW | Until flag cleared |
| **Red flag** | ALL RED | [S] [T] [O] [P] or ALL RED | Until resumed |
| **Safety car** | ALL YELLOW | [S] [A] [F] row 1, [E] [T] [Y] row 2 | Until SC in |
| **Blue flag** | BLUE (on that driver) | Keep name | 5 seconds |
| **Black & white flag** | BLK/WHT (on that driver) | Keep name | 5 seconds |
| **VSC** | ALL YELLOW | [V] [S] [C] | Until cleared |
| **DRS enabled** | GREEN (brief flash) | Keep current | 3 seconds |

```
Pit stop example:
[ 1] [V] [E] [R]
[FER LOGO] [P] [I] [T]   ← Leclerc pitting
[ 3] [N] [O] [R]
...

After pit — tire indicator:
[ 1] [V] [E] [R]
[RED] [L] [E] [C]   ← RED = soft tire, 5 sec then back to normal
[ 3] [N] [O] [R]

Yellow flag:
[YEL] [YEL] [YEL] [YEL]
[YEL] [YEL] [YEL] [YEL]
[YEL] [YEL] [YEL] [YEL]
... (entire board yellow)

Safety car:
[YEL] [S] [A] [F]
[YEL] [E] [T] [Y]
[YEL] [C] [A] [R]
[YEL] [YEL] [YEL] [YEL]
[YEL] [YEL] [YEL] [YEL]
...

Red flag:
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
... (entire board red)
```

#### Phase 3: Race Finish

1. **Checkered flag** — board goes alternating black/white
```
[BLK] [WHT] [BLK] [WHT]
[WHT] [BLK] [WHT] [BLK]
[BLK] [WHT] [BLK] [WHT]
...
```

2. **Car animation + Rainbow** — F1 car drives down col 0 with rolling rainbow on right
```
[CAR▼] [RED ] [BLK ] [WHT ]
[     ] [ORG ] [RED ] [BLK ]
[     ] [YEL ] [ORG ] [RED ]
...
```

3. **Resolves to final standings**
```
[ 1] [V] [E] [R]
[ 2] [H] [A] [M]
...
```

---

## STATIC END STATES

| # | Name | Left Col | Right Cols |
|---|------|----------|------------|
| 1 | Driver Standings | Position 1–10 | Initials |
| 2 | Constructor Standings | Team logos | Abbreviation or points |
| 3 | Checkered Flag | ⬛⬜ alt | ⬜⬛ alt |
| 4 | Logo + Color Rows | Logo | Team color solid |
| 5 | Race Winner | Trophy/Logo | Winner name + color |
| 6 | Sector Times | Sector color | Lap times |
| 7 | Fastest Lap | Purple | Driver + time |
| 8 | Red Flag | All red | All red |
| 9 | Safety Car | All yellow | "SAFETY CAR" |
| 10 | Off / Sleep | All black | All black |
| 11 | Logo + Name | Logo | Initials |
| 12 | Position + Time | Position | Lap time |
| 13 | Gap to Leader | Position | +0.3, +1.2 etc |
| 14 | Tire Strategy | Tire color | Driver initials |
| 15 | Weather Alert | Rain icon / Blue | "WET" / "DRY" |
| 16 | Clock Mode | Numbers | Time display (idle) |
| 17 | Custom Message | Black/color | Free text |
| 18 | Championship Clinch | Gold | Winner + "WDC" |

---

## ANIMATION TRANSITIONS

| # | Animation | Description | Used in |
|---|-----------|-------------|---------|
| A1 | Rolling Rainbow L→R | Color pushes left, prev shifts right | Race finish, wake up |
| A2 | Team Color Fill L→R | Row fills team color left to right | Constructor reveal |
| A3 | Car Down Column | F1 car stop-motions down col 0 | Finish, leaderboard reveal |
| A4 | Checkered Wipe | B/W alternates cascade down | Race finish |
| A5 | Color Wave Down | Single color rolls top to bottom | Flag events |
| A6 | Random Flip → Resolve | Chaotic flips settle to final state | Wake up, celebrations |
| A7 | Red Flood | All units flip to red instantly | Red flag |
| A8 | Yellow Flood | All units flip to yellow instantly | Yellow flag / SC |
| A9 | Fade to Black | Row by row goes black top→bottom | Sleep |
| A10 | Green/Red Column Flash | Left col flashes green or red | Quali elimination |
| A11 | Purple Pulse | Purple wave across board | Fastest lap alert |

---

## FULL SEQUENCES

| # | Name | Animation | End State |
|---|------|-----------|-----------|
| S1 | Leaderboard Reveal | A3 (Car ↓) + A1 (Rainbow) | Driver Standings |
| S2 | Constructor Reveal | A2 (Team Color Fill) | Constructor Standings |
| S3 | Race Finish | A4 (Checkered) → A3+A1 (Car+Rainbow) | Final Standings |
| S4 | Lights Out | Red fill → all black | Race Running |
| S5 | Position Change | Brief flash, X sec delay | Updated Standings |
| S6 | Fastest Lap | A11 (Purple pulse) | Fastest Lap display |
| S7 | Red Flag | A7 (Red flood) | Red Flag state |
| S8 | Safety Car | A8 (Yellow flood) | Safety Car text |
| S9 | Team Parade | Row-by-row team color | Logo + Color |
| S10 | Sleep | A9 (Fade to black) | All Black |
| S11 | Wake Up | A1 (Rainbow) or A6 (Random) | Driver Standings |
| S12 | Quali Elimination | A10 (Green/Red flash) | Standings w/ red/green |
| S13 | Pit Stop | Swap name → PIT | Tire color → back to normal |
| S14 | DRS | Brief green flash on left | Back to race state |
| S15 | Championship Clinch | A6 (Random) → Gold wave | WDC/WCC celebration |

---

## TIMING & SETTINGS

| Parameter | Default | Notes |
|-----------|---------|-------|
| Position change delay | 3 sec | Wait before updating board after overtake |
| Tire color display | 5 sec | Show tire compound then revert |
| Flag display (non-yellow/red) | 5 sec | Blue/B&W flag per driver |
| Yellow/Red flag | Until cleared | Stays until race control clears |
| Safety car | Until "SC IN" | Full board remains yellow |
| Pit stop display | Until pit exit | Show PIT while driver in pit lane |
| Sector time display | Until next sector | Update per sector completion |

---

## TIRE COLORS

| Compound | Flap Color |
|----------|-----------|
| Soft | Red |
| Medium | Yellow |
| Hard | White |
| Intermediate | Green |
| Wet | Blue |
