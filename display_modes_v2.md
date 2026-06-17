# Split Flap F1 — 5×5 Display Modes & Flap Allocation (v2)

## Grid: 5 Columns × 5 Rows (25 units)

| Col 0 | Col 1 | Col 2 | Col 3 | Col 4 |
|-------|-------|-------|-------|-------|
| Position/Color | Logo/Symbol | Text/Num | Text/Num | Text/Num/Color |

---

## DISPLAY MODES

### Mode 1: Name + Gap (2 rows per driver = 2 drivers + header possible)

```
[ 1] [🔴] [N] [O] [R]
[  ] [- ] [0] [5] [9]   ← gap +0.059 (or leader time)
[ 2] [◇ ] [R] [U] [S]
[  ] [- ] [1] [0] [6]   ← +0.106
[ 3] [★ ] [V] [E] [R]
```

- Col 0 row 2: blank (black) or sector color
- Col 1 row 2: dash/plus sign or sector color
- Cols 2-4 row 2: time digits

### Mode 2: Names Only (5 drivers)

```
[ 1] [🔴] [N] [O] [R]
[ 2] [◇ ] [R] [U] [S]
[ 3] [★ ] [V] [E] [R]
[ 4] [🔴] [L] [E] [C]
[ 5] [→ ] [P] [I] [A]
```

### Mode 3: Flip Between Name ↔ Time (same row, timed)

```
Shows for 3 sec:  [ 1] [🔴] [N] [O] [R]
Flips to 3 sec:   [ 1] [🔴] [1] [1] [5]   ← 1:15.xxx
Flips back:       [ 1] [🔴] [N] [O] [R]
```

All 5 rows do this in sync or staggered.

### Mode 4: Name + Tire/Penalty (Col 4 = status)

```
[ 1] [N] [O] [R] [RED ]   ← soft tire
[ 2] [R] [U] [S] [YEL ]   ← medium tire
[ 3] [V] [E] [R] [WHT ]   ← hard tire
[ 4] [L] [E] [C] [RED ]   ← soft tire
[ 5] [P] [I] [A] [BLK ]   ← penalty/no status
```

Note: Col 1 becomes first letter (no logo) to fit 3 letters + color in 5 cols.
OR keep logo and use only 2-char name:

```
[ 1] [🔴] [N] [O] [RED ]   ← NOR + soft tire
```

### Mode 5: Standings Cycle (Configurable)

**Step 1 (5 sec):** Top 5 drivers + names
```
[ 1] [🔴] [N] [O] [R]
[ 2] [◇ ] [R] [U] [S]
[ 3] [★ ] [V] [E] [R]
[ 4] [🔴] [L] [E] [C]
[ 5] [→ ] [P] [I] [A]
```

**Step 2 (5 sec):** Same 5 drivers + points
```
[ 1] [🔴] [3] [2] [1]   ← 321 points
[ 2] [◇ ] [2] [8] [9]
[ 3] [★ ] [2] [7] [5]
[ 4] [🔴] [2] [5] [0]
[ 5] [→ ] [2] [1] [8]
```

**Step 3 (5 sec):** Next 5 (P6–P10)
```
[ 6] [ALP] [A] [L] [O]
[ 7] [WIL] [R] [U] [S]
...
```

Configurable: Show top 5, 10, 15, or all 20 (cycles through pages).
Configurable: Alternate name ↔ points, or points only, or name only.

### Mode 6: Constructor Standings (same cycling logic)

**Step 1:** Team name
```
[ 1] [🔴] [F] [E] [R]
[ 2] [→ ] [M] [C] [L]
[ 3] [◇ ] [M] [E] [R]
[ 4] [★ ] [R] [B] [L]
[ 5] [AM] [A] [M] [R]
```

**Step 2:** Team points
```
[ 1] [🔴] [6] [5] [2]
[ 2] [→ ] [5] [8] [1]
[ 3] [◇ ] [5] [4] [3]
[ 4] [★ ] [4] [9] [2]
[ 5] [AM] [3] [2] [1]
```

### Mode 7: Single Driver Focus

```
[🔴] [RED] [RED] [RED] [RED]   ← team color bar
[ 1] [🔴] [N]   [O]   [R]     ← position + name
[PUR] [1]  [-]   [1]   [5]    ← lap time (purple = fastest)
[GRN] [2]  [8]   [-]   [3]    ← S1 (green = PB)
[YEL] [3]  [1]   [-]   [2]    ← S2 (yellow = slower)
```

### Mode 8: Checkered Flag

```
[BLK] [WHT] [BLK] [WHT] [BLK]
[WHT] [BLK] [WHT] [BLK] [WHT]
[BLK] [WHT] [BLK] [WHT] [BLK]
[WHT] [BLK] [WHT] [BLK] [WHT]
[BLK] [WHT] [BLK] [WHT] [BLK]
```

### Mode 9: Full Board Color (Flags)

- All RED = red flag
- All YELLOW = yellow flag / safety car
- All PURPLE = fastest lap flash
- All GREEN = green flag / go
- All BLACK = off / sleep

### Mode 10: Safety Car (Yellow + Text)

```
[YEL] [YEL] [YEL] [YEL] [YEL]
[YEL] [S]   [A]   [F]   [E]
[YEL] [T]   [Y]   [YEL] [YEL]
[YEL] [C]   [A]   [R]   [YEL]
[YEL] [YEL] [YEL] [YEL] [YEL]
```

---

## WHAT EACH COLUMN NEEDS

Based on all modes above:

| | Col 0 | Col 1 | Col 2 | Col 3 | Col 4 |
|--|-------|-------|-------|-------|-------|
| Numbers 0–9 | ✅ (position) | ✅ (time digit) | ✅ | ✅ | ✅ |
| Letters A–Z | ❌ | ❌ (logos instead) | ✅ | ✅ | ✅ |
| Team Logos | ❌ | ✅ (11 logos) | ❌ | ❌ | ❌ |
| Colors | ✅ (flags/sectors) | ✅ (team color fill) | ✅ (checkered/flags) | ✅ (checkered/flags) | ✅ (tire/flags) |
| Car Frames | ✅ (animation) | ❌ | ❌ | ❌ | ❌ |
| Dash/Plus | ❌ | ✅ (gap indicator) | ✅ (time format) | ✅ | ❌ |

---

## FLAP ALLOCATION PER COLUMN

### Col 0: Position + Animation + Colors

| Category | Items | Count |
|----------|-------|-------|
| Numbers 0–9 | Position display | 10 |
| Car frames | Animation | 10 |
| Colors | Sector/flag colors | 13 |
| **Total** | | **33** |

| Budget | Remaining | Extras |
|--------|-----------|--------|
| 45 flaps | **12** | Arrows ▲▼, icons, blank covered by black |
| 50 flaps | **17** | Even more room |

### Col 1: Logo + Numbers + Colors + Symbols

| Category | Items | Count |
|----------|-------|-------|
| Team logos | 11 teams | 11 |
| Numbers 0–9 | Time digits (Mode 3, 7) | 10 |
| Colors | Team color fills, flags | 13 |
| Dash / Plus | Gap indicator | 2 |
| **Total** | | **36** |

| Budget | Remaining | Extras |
|--------|-----------|--------|
| 45 flaps | **9** | Could add select letters for text modes |
| 50 flaps | **14** | Could add partial alphabet |

### Col 2: Full Text + Numbers + Colors

| Category | Items | Count |
|----------|-------|-------|
| Letters A–Z | Names/text | 26 |
| Numbers 0–9 | Times/points | 10 |
| Colors | Flags/checkered | ? |
| Dash (-) | Time format | 1 |
| **Total without colors** | | **37** |

| Budget | Remaining for colors | |
|--------|---------------------|--|
| 45 flaps | **8 colors** | Black, White, Red, Yellow, Green, Purple, Orange, Teal |
| 50 flaps | **13 colors** | All 13 fit |

### Col 3: Same as Col 2

Identical to Col 2 — same needs.

### Col 4: Full Text + Numbers + Colors (tire/penalty focus)

Same as Col 2 but colors are extra important here (tire compound display).

Identical allocation to Col 2.

---

## FINAL ALLOCATION TABLES

### 45-Flap Version

| | Col 0 | Col 1 | Col 2 | Col 3 | Col 4 |
|--|-------|-------|-------|-------|-------|
| Numbers 0–9 | 10 | 10 | 10 | 10 | 10 |
| Letters A–Z | — | — | 26 | 26 | 26 |
| Team Logos | — | 11 | — | — | — |
| Car Frames | 10 | — | — | — | — |
| Colors | 13 | 13 | 8 | 8 | 8 |
| Symbols (-/+) | — | 2 | 1 | 1 | 1 |
| **Total** | **33** | **36** | **45** | **45** | **45** |
| **Remaining** | **12** | **9** | **0** | **0** | **0** |

**Cols 2–4 colors (8):** Black, White, Red, Yellow, Green, Purple, Orange, Teal

Missing from right cols: Navy, Pink, Light Blue, Maroon, Lime — only affects rainbow animation variety. Flags/sectors/checkered all covered.

### 50-Flap Version

| | Col 0 | Col 1 | Col 2 | Col 3 | Col 4 |
|--|-------|-------|-------|-------|-------|
| Numbers 0–9 | 10 | 10 | 10 | 10 | 10 |
| Letters A–Z | — | — | 26 | 26 | 26 |
| Team Logos | — | 11 | — | — | — |
| Car Frames | 10 | — | — | — | — |
| Colors | 13 | 13 | 13 | 13 | 13 |
| Symbols (-/+) | — | 2 | 1 | 1 | 1 |
| **Total** | **33** | **36** | **50** | **50** | **50** |
| **Remaining** | **17** | **14** | **0** | **0** | **0** |

All 13 colors on every column. Full rainbow animation, all team colors available everywhere.

---

## EXTRAS (What to do with remaining flaps)

### Col 0 extras (12–17 spare)

| Idea | Count |
|------|-------|
| Arrow ▲ (gained position) | 1 |
| Arrow ▼ (lost position) | 1 |
| Dash (—) for "no change" | 1 |
| Trophy icon | 1 |
| DRS icon | 1 |
| Safety car icon | 1 |
| Pit icon | 1 |
| Rain icon | 1 |
| Checkered mini icon | 1 |
| Red flag mini icon | 1 |
| "P" for pole position | 1 |
| "F" for fastest lap | 1 |
| Still have 0–5 spare | |

### Col 1 extras (9–14 spare)

| Idea | Count |
|------|-------|
| Letters for text modes (S, A, F, E, T, Y, C, R, P) | 9 |
| Allows "SAFETY CAR", "PIT", etc. on Col 1 | |

Or add partial alphabet — most used letters in F1 context:
S, A, F, E, T, Y, C, R, P, I, O, L, D, N = 14 letters

---

## SUMMARY: WHAT YOU GET

| Mode | Works with 45? | Works with 50? |
|------|---------------|----------------|
| Mode 1: Name + Gap | ✅ | ✅ |
| Mode 2: Names only (5 drivers) | ✅ | ✅ |
| Mode 3: Flip name ↔ time | ✅ | ✅ |
| Mode 4: Name + tire color | ✅ | ✅ |
| Mode 5: Standings cycle (name/points) | ✅ | ✅ |
| Mode 6: Constructor cycle | ✅ | ✅ |
| Mode 7: Single driver focus | ✅ | ✅ |
| Mode 8: Checkered flag | ✅ (need B+W) | ✅ |
| Mode 9: Full board color (flags) | ✅ (8 colors) | ✅ (13 colors) |
| Mode 10: Safety car text | ✅ (if Col 1 has letters) | ✅ |
| Rainbow animation | Partial (8 colors) | Full (13 colors) |
| All team colors on right cols | ❌ (8 of 13) | ✅ (all 13) |

### Recommendation: Go with 50 flaps

- No compromises on any mode
- All 13 colors everywhere
- Only 5 more flaps per unit (125 extra total)
- Same drum size either way (50 vs 45 is negligible mechanically)
