# Split Flap F1 — Final Flap Allocation (v3)

## 5 Columns × 5 Rows, 45 Flaps Per Unit

---

## Base 8 Colors (Same on ALL columns)

| # | Color | Hex | Flags | Tires | Sectors | Teams (reuse) |
|---|-------|-----|-------|-------|---------|---------------|
| 1 | Red | #DC0000 | Red flag | Soft | — | Ferrari, Audi |
| 2 | White | #FFFFFF | — | Hard | — | Haas |
| 3 | Black | #000000 | Black flag | — | — | Blank/space |
| 4 | Yellow | #FFD700 | Yellow flag, SC | Medium | Slower | Cadillac |
| 5 | Green | #006F62 | Green flag | Intermediate | Personal best | Aston Martin |
| 6 | Blue | #1E41FF | Blue flag | Wet | — | Red Bull, Williams |
| 7 | Purple | #A020F0 | — | — | Overall best, FL | Racing Bulls |
| 8 | Orange | #FF8000 | — | — | — | McLaren |

✅ Flags: Red, Yellow, Green, Blue, Black, Checkered (B+W)
✅ Tires: Soft (R), Medium (Y), Hard (W), Inter (G), Wet (Blu)
✅ Sectors: PB (G), Fastest (Pur), Slower (Y)

## 5 Additional Team Colors (Col 0 + Col 1 only)

| # | Color | Hex | Team |
|---|-------|-----|------|
| 9 | Teal | #00D2BE | Mercedes |
| 10 | Pink | #FF87BC | Alpine |
| 11 | Light Blue | #005AFF | Williams |
| 12 | Maroon | #900000 | Audi |
| 13 | Gold | #FFD700 | Cadillac |

---

## Col 0: Position / Status / Animation (45 flaps)

| Pos | Content | Count |
|-----|---------|-------|
| 0–9 | Numbers 0–9 | 10 |
| 10–14 | Car animation frames | 5 |
| 15–22 | Base colors (8) | 8 |
| 23–27 | Team-unique colors (5) | 5 |
| 28 | Trophy 🏆 | 1 |
| 29 | P (Pit) | 1 |
| 30 | Plus (+) | 1 |
| 31 | Dash/Minus (-) | 1 |
| 32–36 | Spare car frames (expansion to 10 total) | 5 |
| 37–44 | Spare | 8 |
| **Total** | | **45** |

**Col 0 totals: 10 num + 5 car + 13 colors + 4 icons/symbols + 5 spare car + 8 spare**

---

## Col 1: Logo / Digits / Colors / Symbols (45 flaps)

| Pos | Content | Count |
|-----|---------|-------|
| 0–10 | Team logos (11 teams) | 11 |
| 11–20 | Numbers 0–9 | 10 |
| 21–28 | Base colors (8) | 8 |
| 29–33 | Team-unique colors (5) | 5 |
| 34 | Dash (-) | 1 |
| 35 | Period (.) | 1 |
| 36–44 | Spare | 9 |
| **Total** | | **45** |

**Col 1 totals: 11 logo + 10 num + 13 colors + 2 symbols + 9 spare**

---

## Cols 2, 3, 4: Text / Numbers / Colors (45 flaps each, IDENTICAL)

| Pos | Content | Count |
|-----|---------|-------|
| 0–25 | Letters A–Z | 26 |
| 26–35 | Numbers 0–9 | 10 |
| 36–43 | Base colors (8) | 8 |
| 44 | Dash (-) | 1 |
| **Total** | | **45** |

**Cols 2–4 totals: 26 letters + 10 num + 8 colors + 1 symbol = 45 (full, no spare)**

---

## FULL GRID SUMMARY

| | Col 0 | Col 1 | Col 2 | Col 3 | Col 4 |
|--|-------|-------|-------|-------|-------|
| Numbers 0–9 | ✅ 10 | ✅ 10 | ✅ 10 | ✅ 10 | ✅ 10 |
| Letters A–Z | — | — | ✅ 26 | ✅ 26 | ✅ 26 |
| Team Logos | — | ✅ 11 | — | — | — |
| Car Frames | ✅ 5 (+5 spare) | — | — | — | — |
| Base Colors (8) | ✅ | ✅ | ✅ | ✅ | ✅ |
| Team Colors (5) | ✅ | ✅ | — | — | — |
| Total Colors | **13** | **13** | **8** | **8** | **8** |
| Symbols (-/+) | — | ✅ 2 | ✅ 1 | ✅ 1 | ✅ 1 |
| Icons | ✅ 2 (🏆, P) | — | — | — | — |
| Spare | **15** | **9** | **0** | **0** | **0** |
| **Total** | **45** | **45** | **45** | **45** | **45** |

---

## SPARE USAGE PLAN

| Column | Spare | Purpose |
|--------|-------|---------|
| **Col 0** | 5 | Spare car frames (expand animation from 5→10 if desired) |
| **Col 0** | 10 | Future icons, indicators, or leave blank for flexibility |
| **Col 1** | 9 | Future use — could add key letters, more symbols, or leave blank |

Spare flaps can be left as blank/unprinted on the drum. No harm in having unused positions — just skip them in firmware.

---

## COLOR CONSISTENCY CHECK

| Color | Col 0 | Col 1 | Col 2 | Col 3 | Col 4 | Purpose |
|-------|-------|-------|-------|-------|-------|---------|
| Red | ✅ | ✅ | ✅ | ✅ | ✅ | Flag + Soft + Ferrari |
| White | ✅ | ✅ | ✅ | ✅ | ✅ | Hard + Checkered + Haas |
| Black | ✅ | ✅ | ✅ | ✅ | ✅ | Blank + Checkered + Flag |
| Yellow | ✅ | ✅ | ✅ | ✅ | ✅ | Flag + Medium + SC + Sector |
| Green | ✅ | ✅ | ✅ | ✅ | ✅ | Flag + Inter + Sector + AM |
| Blue | ✅ | ✅ | ✅ | ✅ | ✅ | Flag + Wet + RB |
| Purple | ✅ | ✅ | ✅ | ✅ | ✅ | Sector + FL + Racing Bulls |
| Orange | ✅ | ✅ | ✅ | ✅ | ✅ | McLaren |
| Teal | ✅ | ✅ | ❌ | ❌ | ❌ | Mercedes |
| Pink | ✅ | ✅ | ❌ | ❌ | ❌ | Alpine |
| Light Blue | ✅ | ✅ | ❌ | ❌ | ❌ | Williams |
| Maroon | ✅ | ✅ | ❌ | ❌ | ❌ | Audi |
| Gold | ✅ | ✅ | ❌ | ❌ | ❌ | Cadillac |

---

## TOTAL BUILD NUMBERS

| Metric | Value |
|--------|-------|
| Grid | 5 columns × 5 rows |
| Units | 25 |
| Flaps per unit | 45 |
| Total flaps | **1,125** |
| Unique drum types | **3** |
| Col 0 drums | 5 (position/animation) |
| Col 1 drums | 5 (logo/digits) |
| Col 2–4 drums | 15 (text/numbers) |
