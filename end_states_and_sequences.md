# Split Flap F1 Display — End States & Animation Sequences

## Layout Reference
- 4 columns × 10 rows
- Col 0: Left (numbers, logos, car frames, colors)
- Cols 1–3: Right (letters, numbers, colors, dash)

---

## ANIMATIONS (Transitions)

| # | Animation | Description |
|---|-----------|-------------|
| A1 | Rolling Rainbow L→R | Color pushes left to right, previous color shifts across |
| A2 | Team Color Fill L→R | Row fills with team color left to right |
| A3 | Car Down Column | F1 car stop-motions down col 0, triggers row-by-row reveal |
| A4 | Checkered Wipe | Black/white alternates cascade down the board |
| A5 | Color Wave Down | Single color rolls row by row top to bottom |
| A6 | Random Flip → Resolve | All units flip chaotically, then settle to final state |

---

## END STATES

### 1. Driver Standings

```
[ 1] [V] [E] [R]
[ 2] [H] [A] [M]
[ 3] [L] [E] [C]
[ 4] [N] [O] [R]
[ 5] [S] [A] [I]
[ 6] [A] [L] [O]
[ 7] [R] [U] [S]
[ 8] [P] [I] [A]
[ 9] [S] [T] [R]
[ 0] [G] [A] [S]   ← 10 shown as 0 (or use position 10 flap if added)
```

### 2. Constructor Standings

```
[RB LOGO ] [R] [B] [L]   ← Red Bull
[FER LOGO] [F] [E] [R]
[MER LOGO] [M] [E] [R]
[MCL LOGO] [M] [C] [L]
[AM LOGO ] [A] [M] [R]
[ALP LOGO] [A] [L] [P]
[WIL LOGO] [W] [I] [L]
[HAAS LOGO][H] [A] [S]
[RBL LOGO] [R] [B] [L]
[AUDI LOGO][A] [U] [D]
```

Or with points (right cols show number):

```
[RB LOGO ] [8] [4] [2]   ← 842 points
[FER LOGO] [7] [1] [9]
...
```

### 3. Checkered Flag (Finish)

Alternating black and white across entire board:

```
[BLK] [WHT] [BLK] [WHT]
[WHT] [BLK] [WHT] [BLK]
[BLK] [WHT] [BLK] [WHT]
[WHT] [BLK] [WHT] [BLK]
[BLK] [WHT] [BLK] [WHT]
[WHT] [BLK] [WHT] [BLK]
[BLK] [WHT] [BLK] [WHT]
[WHT] [BLK] [WHT] [BLK]
[BLK] [WHT] [BLK] [WHT]
[WHT] [BLK] [WHT] [BLK]
```

### 4. Logo + Team Color Rows

```
[RB LOGO ] [NVY] [NVY] [NVY]
[FER LOGO] [RED] [RED] [RED]
[MER LOGO] [TEA] [TEA] [TEA]
[MCL LOGO] [ORG] [ORG] [ORG]
[AM LOGO ] [GRN] [GRN] [GRN]
[ALP LOGO] [PNK] [PNK] [PNK]
[WIL LOGO] [LBL] [LBL] [LBL]
[HAAS LOGO][WHT] [WHT] [WHT]
[RBL LOGO] [PUR] [PUR] [PUR]
[AUDI LOGO][MAR] [MAR] [MAR]
```

### 5. Race Winner Celebration

```
[🏆 LOGO] [V] [E] [R]   ← Trophy/logo
[  1    ] [RED] [RED] [RED]  ← team color
[       ] [W] [I] [N]
[       ] [N] [E] [R]
[       ] [BLK] [BLK] [BLK]
[  2    ] [H] [A] [M]
[  3    ] [L] [E] [C]
[       ] [BLK] [BLK] [BLK]
[       ] [BLK] [BLK] [BLK]
[       ] [BLK] [BLK] [BLK]
```

### 6. Qualifying / Sector Times

```
[ 1] [1] [-] [2]   ← 1:2X.XXX (showing first digits)
[ 2] [1] [-] [2]
[ 3] [1] [-] [2]
...
```

With sector colors on left col:
- Green = personal best sector
- Purple = overall best sector  
- Yellow = slower sector

### 7. Lights Out (Race Start)

```
[RED] [RED] [RED] [RED]   ← lights on (all red)
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[BLK] [BLK] [BLK] [BLK]  ← lights out rows start going black
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
```

Then all flip to black = LIGHTS OUT, GO!

### 8. Fastest Lap

```
[PUR] [PUR] [PUR] [PUR]   ← all purple (fastest lap indicator)
[LOGO] [V] [E] [R]
[PUR] [1] [-] [2]         ← time in purple
[PUR] [3] [-] [4]
[PUR] [PUR] [PUR] [PUR]
[BLK] [BLK] [BLK] [BLK]
...
```

### 9. Red Flag / Safety Car

```
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
[RED] [S]  [A]  [F]       ← or SC icon on left
[RED] [E]  [T]  [Y]
[RED] [RED] [RED] [RED]
[RED] [RED] [RED] [RED]
...
```

### 10. DRS Enabled

```
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
[GRN] [D]  [R]  [S]
[GRN] [GRN] [GRN] [GRN]
[GRN] [GRN] [GRN] [GRN]
[BLK] [BLK] [BLK] [BLK]
...
```

### 11. Pit Stop Alert

```
[LOGO] [P]  [I]  [T]
[ORG ] [ORG] [ORG] [ORG]  ← team color
[ 3  ] [L]  [A]  [P]      ← pit window
[BLK] [BLK] [BLK] [BLK]
...
```

### 12. Off / Sleep (All Black)

```
[BLK] [BLK] [BLK] [BLK]
[BLK] [BLK] [BLK] [BLK]
... (all black)
```

---

## FULL SEQUENCES (Animation → End State)

| # | Name | Animation | Resolves To |
|---|------|-----------|-------------|
| S1 | Leaderboard Reveal | Rainbow L→R + Car down | Driver Standings |
| S2 | Constructor Reveal | Team Color Fill L→R | Constructor Standings |
| S3 | Race Finish | Checkered wipe | Race Winner Celebration |
| S4 | Qualifying Results | Color wave (purple) | Sector Times |
| S5 | Lights Out | Red fill top→bottom, then all black | Off (then race starts) |
| S6 | Position Change | Car down + team color | Updated Driver Standings |
| S7 | Fastest Lap | Purple wave | Fastest Lap display |
| S8 | Red Flag | Red flood all at once | Red Flag / Safety Car |
| S9 | Team Parade | Team color fill row by row | Logo + Color Rows |
| S10 | Sleep | Fade to black row by row | All Black |
| S11 | Wake Up | Rainbow cascade | Driver Standings or Logo+Color |

---

## SUGGESTED ADDITIONS

| Idea | Description |
|------|-------------|
| Gap to Leader | Show +0.3, +1.2 etc. in right cols instead of initials |
| Tire Compound | Color indicator (red=soft, yellow=med, white=hard) on left col |
| Weather Alert | Rain icon on left, "WET" on right with blue background |
| Sprint Indicator | "SPR" displayed with different animation style |
| Championship Clinch | Special gold animation when WDC/WCC decided |
| Clock Mode | Display current time when no race (idle mode) |
| Custom Message | Free text entry for party/display use |
