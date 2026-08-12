# Split Flap F1 — Flap Allocation v4 (52 Flaps)

## 5 Columns × 5 Rows, 52 Flaps Per Unit

---

## Defaults

- **Background:** Black (#000000)
- **Font color:** White (#FFFFFF)
- **Font:** Roboto Mono Bold, 36pt (numbers, letters, symbols)
- **Small text font:** Roboto Condensed Bold, 24pt (mph, compound labels)
- **Icons/Logos:** Custom vector artwork, fit to flap with padding
- Tables only note BG/FG where they **differ** from defaults.

---

## Position Alignment

| Range | Alignment |
|-------|-----------|
| 0–7 | ✅ ALL columns — base colors |
| 8–17 | ✅ ALL columns — numbers 0–9 |
| 18–22 | ✅ Col 0 + Col 1 — team colors |
| 23–49 | ❌ Diverges per drum type |
| 50 | ✅ ALL columns — Checkered pattern |
| 51 | ✅ ALL columns — F1 Logo |

---

## SHARED: All 5 Columns (Pos 0–17)

| Pos | Content | BG | Notes |
|-----|---------|----|----|
| 0 | Black (blank) | — | Home position |
| 1 | Red | #DC0000 | Ferrari, red flag, soft |
| 2 | Orange | #FF8000 | McLaren |
| 3 | Yellow | #FFD700 | Yellow flag, SC, medium |
| 4 | White | #FFFFFF | Hard, Haas |
| 5 | Green | #006F62 | Aston Martin, green flag, inter, sector PB |
| 6 | Blue | #1E41FF | Red Bull, blue flag, wet |
| 7 | Purple | #A020F0 | Racing Bulls, sector best, FL |
| 8 | 0 | — | |
| 9 | 1 | — | |
| 10 | 2 | — | |
| 11 | 3 | — | |
| 12 | 4 | — | |
| 13 | 5 | — | |
| 14 | 6 | — | |
| 15 | 7 | — | |
| 16 | 8 | — | |
| 17 | 9 | — | |

---

## SHARED: Col 0 + Col 1 (Pos 18–22)

| Pos | Content | BG | Team |
|-----|---------|----|----|
| 18 | Teal | #00D2BE | Mercedes |
| 19 | Pink | #FF87BC | Alpine |
| 20 | Light Blue | #005AFF | Williams |
| 21 | Maroon | #900000 | Audi |
| 22 | Gold | #C8A951 | Cadillac |

---

## SHARED: All 5 Columns (Pos 50–51)

| Pos | Content | BG | Notes |
|-----|---------|----|----|
| 50 | Checkered pattern | B/W checker | "Go to 50" = whole board checkered |
| 51 | F1 Logo | — (Red+White graphic) | "Go to 51" = whole board F1 logos |

---

## Col 0: Position / Status / Animation (52 flaps)

| Pos | Content | BG | FG | Type | Notes |
|-----|---------|----|----|------|-------|
| 0 | Black | — | — | solid | Home |
| 1 | Red | #DC0000 | — | solid | |
| 2 | Orange | #FF8000 | — | solid | |
| 3 | Yellow | #FFD700 | — | solid | |
| 4 | White | #FFFFFF | — | solid | |
| 5 | Green | #006F62 | — | solid | |
| 6 | Blue | #1E41FF | — | solid | |
| 7 | Purple | #A020F0 | — | solid | |
| 8 | 0 | — | — | digit | |
| 9 | 1 | — | — | digit | |
| 10 | 2 | — | — | digit | |
| 11 | 3 | — | — | digit | |
| 12 | 4 | — | — | digit | |
| 13 | 5 | — | — | digit | |
| 14 | 6 | — | — | digit | |
| 15 | 7 | — | — | digit | |
| 16 | 8 | — | — | digit | |
| 17 | 9 | — | — | digit | |
| 18 | Teal | #00D2BE | — | solid | Mercedes |
| 19 | Pink | #FF87BC | — | solid | Alpine |
| 20 | Light Blue | #005AFF | — | solid | Williams |
| 21 | Maroon | #900000 | — | solid | Audi |
| 22 | Gold | #C8A951 | — | solid | Cadillac |
| 23 | + | — | — | text | Gap positive |
| 24 | - | — | — | text | Gap negative |
| 25 | Trophy | — | #FFD700 | icon | Race winner (gold graphic) |
| 26 | 📍 Location pin | — | #DC0000 | icon | Track/weather location row |
| 27 | ☀️ Sun | #FFD700 | — | icon | Weather: clear |
| 28 | ⛅ Cloud | #808080 | — | icon | Weather: overcast |
| 29 | 🌧 Rain | #1E41FF | — | icon | Weather: rain |
| 30 | 💨 Wind | — | — | icon | Weather: wind row |
| 31 | Podium 1st | #FFD700 | #000000 | text | Gold BG, black "1" |
| 32 | Podium 2nd | #C0C0C0 | #000000 | text | Silver BG, black "2" |
| 33 | Car frame 1 | — | — | graphic | Animation start |
| 34 | Car frame 2 | — | — | graphic | |
| 35 | Car frame 3 | — | — | graphic | |
| 36 | Car frame 4 | — | — | graphic | |
| 37 | Car frame 5 | — | — | graphic | |
| 38 | Car frame 6 | — | — | graphic | |
| 39 | Car frame 7 | — | — | graphic | |
| 40 | Car frame 8 | — | — | graphic | |
| 41 | Car frame 9 | — | — | graphic | |
| 42 | Car frame 10 | — | — | graphic | |
| 43 | Car frame 11 | — | — | graphic | |
| 44 | Car frame 12 | — | — | graphic | |
| 45 | Car frame 13 | — | — | graphic | |
| 46 | Car frame 14 | — | — | graphic | |
| 47 | Car frame 15 | — | — | graphic | |
| 48 | Car frame 16 | — | — | graphic | |
| 49 | Car frame 17 | — | — | graphic | |
| 50 | Checkered | B/W check | — | pattern | Shared all cols |
| 51 | F1 Logo | — | Red+White | graphic | Shared all cols |

**Total: 13 colors + 10 numbers + 2 symbols + 1 trophy + 5 weather + 2 podium + 17 animation + checkered + F1 = 52**

---

## Col 1: Logo / Digits / Colors / Icons (52 flaps)

| Pos | Content | BG | FG | Type | Notes |
|-----|---------|----|----|------|-------|
| 0 | Black | — | — | solid | Home |
| 1 | Red | #DC0000 | — | solid | |
| 2 | Orange | #FF8000 | — | solid | |
| 3 | Yellow | #FFD700 | — | solid | |
| 4 | White | #FFFFFF | — | solid | |
| 5 | Green | #006F62 | — | solid | |
| 6 | Blue | #1E41FF | — | solid | |
| 7 | Purple | #A020F0 | — | solid | |
| 8 | 0 | — | — | digit | |
| 9 | 1 | — | — | digit | |
| 10 | 2 | — | — | digit | |
| 11 | 3 | — | — | digit | |
| 12 | 4 | — | — | digit | |
| 13 | 5 | — | — | digit | |
| 14 | 6 | — | — | digit | |
| 15 | 7 | — | — | digit | |
| 16 | 8 | — | — | digit | |
| 17 | 9 | — | — | digit | |
| 18 | Teal | #00D2BE | — | solid | Mercedes |
| 19 | Pink | #FF87BC | — | solid | Alpine |
| 20 | Light Blue | #005AFF | — | solid | Williams |
| 21 | Maroon | #900000 | — | solid | Audi |
| 22 | Gold | #C8A951 | — | solid | Cadillac |
| 23 | Ferrari logo | #DC0000 | — | logo | |
| 24 | McLaren logo | #FF8000 | — | logo | |
| 25 | Mercedes logo | #00D2BE | #000000 | logo | Black logo on teal |
| 26 | Red Bull logo | #1E41FF | — | logo | |
| 27 | Aston Martin logo | #006F62 | — | logo | |
| 28 | Alpine logo | #FF87BC | #000000 | logo | Black logo on pink |
| 29 | Williams logo | #005AFF | — | logo | |
| 30 | Haas logo | #FFFFFF | #000000 | logo | Black logo on white |
| 31 | Racing Bulls logo | #A020F0 | — | logo | |
| 32 | Audi logo | #900000 | — | logo | |
| 33 | Cadillac logo | #C8A951 | #000000 | logo | Black logo on gold |
| 34 | . | — | — | text | Period / decimal |
| 35 | 🇲🇨 Monaco | — | — | flag | Country flag |
| 36 | 🇮🇹 Italy | — | — | flag | Monza |
| 37 | 🇬🇧 United Kingdom | — | — | flag | Silverstone |
| 38 | 🇺🇸 USA | — | — | flag | Austin/Miami/Vegas |
| 39 | 🇯🇵 Japan | — | — | flag | Suzuka |
| 40 | 🇧🇷 Brazil | — | — | flag | Interlagos |
| 41 | 🇧🇪 Belgium | — | — | flag | Spa |
| 42 | 🇦🇪 UAE | — | — | flag | Abu Dhabi / Bahrain |
| 43 | 🇦🇺 Australia | — | — | flag | Melbourne |
| 44 | 🇳🇱 Netherlands | — | — | flag | Zandvoort |
| 45 | TBD | — | — | — | Reserved |
| 46 | TBD | — | — | — | Reserved |
| 47 | TBD | — | — | — | Reserved |
| 48 | TBD | — | — | — | Reserved |
| 49 | TBD | — | — | — | Reserved |
| 50 | Checkered | B/W check | — | pattern | Shared all cols |
| 51 | F1 Logo | — | Red+White | graphic | Shared all cols |

**Total: 13 colors + 10 numbers + 11 logos + 1 symbol + 10 flags + 5 TBD + checkered + F1 = 52**

---

## Col 2: Text / Numbers / Colors (52 flaps)

| Pos | Content | BG | FG | Type | Notes |
|-----|---------|----|----|------|-------|
| 0–7 | (colors) | (shared) | — | solid | |
| 8–17 | (digits) | — | — | digit | |
| 18 | A | — | — | letter | |
| 19 | B | — | — | letter | |
| 20 | C | — | — | letter | |
| 21 | D | — | — | letter | |
| 22 | E | — | — | letter | |
| 23 | F | — | — | letter | |
| 24 | G | — | — | letter | |
| 25 | H | — | — | letter | |
| 26 | I | — | — | letter | |
| 27 | J | — | — | letter | |
| 28 | K | — | — | letter | |
| 29 | L | — | — | letter | |
| 30 | M | — | — | letter | |
| 31 | N | — | — | letter | |
| 32 | O | — | — | letter | |
| 33 | P | — | — | letter | |
| 34 | Q | — | — | letter | |
| 35 | R | — | — | letter | |
| 36 | S | — | — | letter | |
| 37 | T | — | — | letter | |
| 38 | U | — | — | letter | |
| 39 | V | — | — | letter | |
| 40 | W | — | — | letter | |
| 41 | X | — | — | letter | |
| 42 | Y | — | — | letter | |
| 43 | Z | — | — | letter | |
| 44 | - | — | — | text | Dash |
| 45 | . | — | — | text | Period |
| 46 | : | — | — | text | Colon |
| 47 | ° | — | — | text | Degree symbol |
| 48 | % | — | — | text | Percent |
| 49 | + | — | — | text | Plus |
| 50 | Checkered | B/W check | — | pattern | Shared all cols |
| 51 | F1 Logo | — | Red+White | graphic | Shared all cols |

**Total: 8 colors + 10 numbers + 26 letters + 6 symbols + checkered + F1 = 52**

---

## Col 3: Text / Numbers / Colors (52 flaps)

| Pos | Content | BG | FG | Type | Notes |
|-----|---------|----|----|------|-------|
| 0–7 | (colors) | (shared) | — | solid | |
| 8–17 | (digits) | — | — | digit | |
| 18 | A | — | — | letter | |
| 19 | B | — | — | letter | |
| 20 | C | — | — | letter | |
| 21 | D | — | — | letter | |
| 22 | E | — | — | letter | |
| 23 | F | — | — | letter | |
| 24 | G | — | — | letter | |
| 25 | H | — | — | letter | |
| 26 | I | — | — | letter | |
| 27 | J | — | — | letter | |
| 28 | K | — | — | letter | |
| 29 | L | — | — | letter | |
| 30 | M | — | — | letter | |
| 31 | N | — | — | letter | |
| 32 | O | — | — | letter | |
| 33 | P | — | — | letter | |
| 34 | Q | — | — | letter | |
| 35 | R | — | — | letter | |
| 36 | S | — | — | letter | |
| 37 | T | — | — | letter | |
| 38 | U | — | — | letter | |
| 39 | V | — | — | letter | |
| 40 | W | — | — | letter | |
| 41 | X | — | — | letter | |
| 42 | Y | — | — | letter | |
| 43 | Z | — | — | letter | |
| 44 | - | — | — | text | Dash |
| 45 | . | — | — | text | Period |
| 46 | : | — | — | text | Colon |
| 47 | ° | — | — | text | Degree symbol |
| 48 | % | — | — | text | Percent |
| 49 | + | — | — | text | Plus |
| 50 | Checkered | B/W check | — | pattern | Shared all cols |
| 51 | F1 Logo | — | Red+White | graphic | Shared all cols |

**Total: 8 colors + 10 numbers + 26 letters + 6 symbols + checkered + F1 = 52**

---

## Col 4: Text / Numbers / Colors (52 flaps)

| Pos | Content | BG | FG | Type | Notes |
|-----|---------|----|----|------|-------|
| 0–7 | (colors) | (shared) | — | solid | |
| 8–17 | (digits) | — | — | digit | |
| 18 | A | — | — | letter | |
| 19 | B | — | — | letter | |
| 20 | C | — | — | letter | |
| 21 | D | — | — | letter | |
| 22 | E | — | — | letter | |
| 23 | F | — | — | letter | |
| 24 | G | — | — | letter | |
| 25 | H | — | — | letter | |
| 26 | I | — | — | letter | |
| 27 | J | — | — | letter | |
| 28 | K | — | — | letter | |
| 29 | L | — | — | letter | |
| 30 | M | — | — | letter | |
| 31 | N | — | — | letter | |
| 32 | O | — | — | letter | |
| 33 | P | — | — | letter | |
| 34 | Q | — | — | letter | |
| 35 | R | — | — | letter | |
| 36 | S | — | — | letter | |
| 37 | T | — | — | letter | |
| 38 | U | — | — | letter | |
| 39 | V | — | — | letter | |
| 40 | W | — | — | letter | |
| 41 | X | — | — | letter | |
| 42 | Y | — | — | letter | |
| 43 | Z | — | — | letter | |
| 44 | - | — | — | text | Dash |
| 45 | . | — | — | text | Period |
| 46 | : | — | — | text | Colon |
| 47 | ° | — | — | text | Degree symbol |
| 48 | % | — | — | text | Percent |
| 49 | mph | — | — | small text | Wind speed unit label |
| 50 | Checkered | B/W check | — | pattern | Shared all cols |
| 51 | F1 Logo | — | Red+White | graphic | Shared all cols |

**Total: 8 colors + 10 numbers + 26 letters + 5 symbols + mph + checkered + F1 = 52**

---

## CROSS-COLUMN ALIGNMENT SUMMARY

| Pos | Col 0 | Col 1 | Col 2 | Col 3 | Col 4 |
|-----|-------|-------|-------|-------|-------|
| 0–7 | Colors | Colors | Colors | Colors | Colors |
| 8–17 | Numbers | Numbers | Numbers | Numbers | Numbers |
| 18–22 | Team colors | Team colors | A–E | A–E | A–E |
| 23–49 | Symbols/WX/Anim | Logos/Flags/TBD | F–Z + symbols | F–Z + symbols | F–Z + symbols |
| 50 | Checkered | Checkered | Checkered | Checkered | Checkered |
| 51 | F1 Logo | F1 Logo | F1 Logo | F1 Logo | F1 Logo |

---

## DISPLAY EXAMPLES

### Driver Standings
```
[ 1] [Ferrari 🔴] [N] [O] [R]
[ 2] [McLaren 🟠] [P] [I] [A]
[ 3] [Red Bull 🔵] [V] [E] [R]
[ 4] [Mercedes ◻️] [H] [A] [M]
[ 5] [Aston 🟢] [A] [L] [O]
```

### Weather Display
```
[📍] [🇲🇨] [M] [O] [N]
[☀️] [ 3 ] [2] [°] [C]
[🌧] [ 1 ] [0] [%] [ ]
[💨] [ . ] [N] [W] [ ]
[ . ] [ 1 ] [5] [.] [mph]
```

### Podium Celebration
```
[🏆] [McLaren] [N] [O] [R]
[🥇] [Ferrari] [L] [E] [C]
[🥈] [Red Bull] [V] [E] [R]
[checkered] [checkered] [checkered] [checkered] [checkered]
[F1] [F1] [F1] [F1] [F1]
```

---

## TOTAL BUILD NUMBERS

| Metric | Value |
|--------|-------|
| Grid | 5 columns × 5 rows |
| Units | 25 |
| Flaps per unit | 52 |
| Total flaps | **1,300** |
| Unique drum types | **4** (Col 0, Col 1, Col 2/3 shared, Col 4) |
| Col 0 drums | 5 |
| Col 1 drums | 5 |
| Col 2–3 drums | 10 (identical) |
| Col 4 drums | 5 (differs from 2/3: has "mph" instead of "+") |
| Steps per revolution | 2048 |
| Steps per flap | ~39.4 (use lookup table) |

---

## MANUFACTURING SPEC

### Defaults (unless overridden)

| Property | Value |
|----------|-------|
| Background | Black #000000 full bleed |
| Foreground | White #FFFFFF |
| Font (digits/letters/symbols) | Roboto Mono Bold 36pt centered |
| Font (small labels: mph) | Roboto Condensed Bold 24pt centered |
| Icons | Custom vector, white on black unless noted |
| Logos | Team vector art on team-color BG |
| Flags | Full-color country flag artwork |
| Finish | Matte |

### Print Types

| Type | Description |
|------|-------------|
| solid | Full-bleed background color, no text |
| digit | Number character (default BG/FG) |
| letter | Letter character (default BG/FG) |
| text | Symbol character (default BG/FG) |
| small text | Smaller font label (mph) |
| icon | Vector graphic on colored or default BG |
| logo | Team logo on team-color BG |
| flag | Full-color country flag artwork |
| graphic | Complex artwork (F1 logo, car frames) |
| pattern | Repeating pattern (checkered) |
