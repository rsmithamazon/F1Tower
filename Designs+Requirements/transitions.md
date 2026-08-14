# Split Flap F1 — Transitions

## Design Principle

Transitions are **customizable** — most state changes offer multiple selectable styles. User picks their preference per transition type in settings.

---

## TRANSITION STYLES (Building Blocks)

| ID | Style | Description | Time (5 rows) |
|----|-------|-------------|---------------|
| T1 | Instant | All 25 units flip simultaneously | 0.3–3.6s |
| T2 | Cascade Down | Row 1 → 2 → 3 → 4 → 5 | ~2.5s |
| T3 | Cascade Up | Row 5 → 4 → 3 → 2 → 1 | ~2.5s |
| T4 | Sweep L→R | Col 0 → 1 → 2 → 3 → 4 | ~1.5s |
| T5 | In-Place Sequence | Pos 1 resolves, then pos 2, then pos 3... (wherever they sit) | ~2.5s |
| T6 | Car Down | Car drives col 0 top → bottom | ~2–3.5s |
| T7 | Car Down + Rainbow | Car descends, rainbow fills L→R behind it per row | ~3–4s |
| T8 | Car Down + Color | Car descends, selectable solid color fills behind it | ~3–4s |
| T9 | Rainbow Only | Rolling rainbow L→R, row by row | ~3s |
| T10 | Random | All units flip with randomized start offsets | ~2s chaotic |

---

## TRANSITIONS BY EVENT

### Standard Mode Changes (Standings → Standings, etc.)

| Transition | Options (selectable) |
|------------|---------------------|
| Page scroll (P1–5 → P6–10) | T1 (instant) · T2 (cascade down) · T3 (cascade up) |
| Name → Points (same page) | T1 (instant) · T4 (sweep L→R) |
| Points → Name (same page) | T1 (instant) · T4 (sweep L→R) |
| Name ↔ Time flip (Mode 3) | T1 (all rows sync) · T2 (cascade down) · T5 (in-place) |
| Mode change (e.g. standings → constructor) | T1 · T2 · T9 (rainbow) · T7 (car + rainbow) |

### Flag Events

| Transition | Options |
|------------|---------|
| Race → Yellow flag | **T1 only** (instant flood — urgency) |
| Race → Red flag | **T1 only** (instant flood) |
| Race → Safety car | **T1 only** (instant yellow + text) |
| Flag → Back to race | T1 (instant) · T3 (cascade up, P1 at top appears first) · T5 (in-place: P1 then P2 then P3...) |

### Race Events

| Transition | Options |
|------------|---------|
| Position change | Single row update (just the affected rows flip) |
| Pit stop in | Single row: name → "PIT" |
| Pit stop out | Single row: "PIT" → name |
| Tire change indicator | Single cell: Col 0 or Col 4 flips to tire color, 5 sec, flips back |
| Blue/BW flag on driver | Single cell: Col 0 flips to color, 5 sec, flips back |

### Race Finish

| Step | Transition | Options |
|------|-----------|---------|
| 1. Checkered | T1 (instant checkered pattern) | Fixed — always instant |
| 2. Celebration | T7 (car + rainbow) · T9 (rainbow only) · T6 (car + selectable color) | Selectable |
| 3. Final standings | T2 (cascade down) · T1 (instant) · T5 (in-place) | Selectable |

### Startup / Wake

| Transition | Options |
|------------|---------|
| Power on → first display | BLACK → WHITE (always, same as lights out) → then selectable (rainbow, car+rainbow, cascade down, instant) to first mode |

### Sleep / Shutdown

| Transition | Options |
|------------|---------|
| Any → Off (all black) | T2 (cascade down) · T3 (cascade up) · T1 (instant) — always ends at BLACK |

---

## CAR DOWN VARIANTS (Detailed)

### T6: Car Down (Pure)

```
Time 0:  [CAR] [COL] [COL] [COL] [COL]  ← row 1: car + selectable color
         [BLK] [BLK] [BLK] [BLK] [BLK]
         [BLK] [BLK] [BLK] [BLK] [BLK]
         [BLK] [BLK] [BLK] [BLK] [BLK]
         [BLK] [BLK] [BLK] [BLK] [BLK]

Time 1:  [COL] [COL] [COL] [COL] [COL]  ← stays color (or instant to final state)
         [CAR] [COL] [COL] [COL] [COL]  ← car moves down
         [BLK] [BLK] [BLK] [BLK] [BLK]
         ...

After car exits → instant flip to final screen
```

### T7: Car Down + Rainbow

```
Time 0:  [CAR] [RED ] [    ] [    ] [    ]  ← car + rainbow starts
Time 1:  [CAR] [RED ] [RED ] [    ] [    ]
Time 2:  [CAR] [RED ] [RED ] [RED ] [RED ]  ← row 1 full
Time 3:  [   ] [ORG ] [RED ] [RED ] [RED ]  ← car to row 2, orange pushes in
Time 4:  [CAR] [ORG ] [ORG ] [RED ] [RED ]
...resolves to final state after complete
```

### T8: Car Down + Selectable Color

Same as T6 but the fill color is configurable:
- Team color (match the driver being shown)
- Flag color
- Any of the 8 base colors
- Black (car only, no fill)

---

## CONFIGURABLE SETTINGS

```json
{
  "transitions": {
    "pageScroll": "cascade_down",       // instant | cascade_down | cascade_up
    "nameToPoints": "instant",          // instant | sweep_lr
    "nameTimeFlip": "cascade_down",     // instant | cascade_down | in_place
    "modeChange": "car_rainbow",        // instant | cascade_down | rainbow | car_rainbow
    "flagReturn": "in_place",           // instant | cascade_up | in_place
    "raceFinish": {
      "celebration": "car_rainbow",     // car_rainbow | rainbow | car_color
      "finalStandings": "cascade_down"  // instant | cascade_down | in_place
    },
    "startup": "rainbow",              // rainbow | car_rainbow | cascade_down | random
    "shutdown": "cascade_down",        // cascade_down | cascade_up | instant
    "carColor": "team"                 // team | black | red | blue | ... (for T6/T8)
  }
}
```

---

## TRANSITIONS I'D ADD (You Didn't Mention)

| Transition | Scenario | Why |
|------------|----------|-----|
| **Fastest lap alert** | Driver sets FL → purple flash on their row, then purple fills briefly | Makes FL dramatic without full-board takeover |
| **DRS enabled** | Quick green flash on Col 0 for affected driver | Subtle, doesn't disrupt standings |
| **Qualifying round change** | Q1 → Q2 transition | Could use rainbow or team color wipe between rounds |
| **Position swap animation** | Two drivers swap — briefly both rows flip, then settle in new order | Shows the overtake happened |
| **Weather change** | Dry → wet or vice versa | Color shift (blue wash?) before updating tire indicators |
| **Race start countdown** | Red fills top→bottom (mimicking 5 lights), then all black = GO | Only 5 rows = perfect for 5 lights! |

### Race Start = 5 Lights! (perfect for 5 rows)

```
Time 0:  [RED] [BLK] [BLK] [BLK] [BLK]  ← 1 light (row 1 goes red)
         [BLK] ...

Time 1:  [RED] [RED] [RED] [RED] [RED]   ← row 1 full
         [RED] [BLK] [BLK] [BLK] [BLK]  ← row 2 starts
         [BLK] ...

Time 4:  ALL RED (5×5 = 25 red)          ← 5 lights!
         ... pause ...

Time 5:  ALL WHITE                        ← LIGHTS OUT! (red→white = fast forward flip)
```

Device startup uses the same sequence:
```
1. Home motors → ALL BLACK (off state)
2. BLACK → WHITE (wake up — forward flip through colors = visual cascade)
3. WHITE → first display (selectable transition)
```

**Flap drum color order is dark → light** so black→white is always a forward spin:
```
[Black] → [Red] → [Blue] → [Green] → [Purple] → [Orange] → [Yellow] → [White]
```

---

## TRANSITION MATRIX (State × State)

| From ↓ \ To → | Standings | Points | Flag | Pit | Checkered | Off |
|----------------|-----------|--------|------|-----|-----------|-----|
| **Off** | startup anim | startup anim | — | — | — | — |
| **Standings** | page scroll | name→pts | instant flood | single row | instant | shutdown |
| **Points** | pts→name | page scroll | instant flood | single row | instant | shutdown |
| **Flag** | flag return | flag return | — | — | instant | shutdown |
| **Pit** | single row back | single row back | instant flood | — | instant | shutdown |
| **Checkered** | celebration→resolve | celebration→resolve | — | — | — | shutdown |

---

## SCROLL BEHAVIOR (More Than 5 Drivers)

Only 5 rows visible at a time. When showing more than 5 drivers:

### Modes

| Mode | Behavior |
|------|----------|
| **Locked** | Show only 5 specific drivers (e.g. top 5, or P6–P10). No scrolling. |
| **Auto-scroll** | Cycle through pages automatically. No button input. |

### Auto-Scroll Settings

```json
{
  "scroll": {
    "enabled": true,
    "driversToShow": 20,        // 5, 10, 15, or 20
    "pageDelay": 5000,          // ms to hold each page before scrolling
    "transition": "cascade_down", // cascade_down | cascade_up | instant
    "loop": true                // restart from page 1 after last page
  }
}
```

### Examples

**Show top 10, cascade down, 5 sec per page:**
```
Page 1 (5 sec):  P1–P5 visible
  ↓ cascade down transition
Page 2 (5 sec):  P6–P10 visible
  ↓ cascade down transition
Page 1 (loops):  P1–P5 visible
```

**Show all 20, instant flip, 8 sec per page:**
```
Page 1 (8 sec):  P1–P5
  ↓ instant
Page 2 (8 sec):  P6–P10
  ↓ instant
Page 3 (8 sec):  P11–P15
  ↓ instant
Page 4 (8 sec):  P16–P20
  ↓ instant
Page 1 (loops):  P1–P5
```

### Mode-Dependent Defaults

| Session | Default Scroll | Default Delay | Notes |
|---------|---------------|---------------|-------|
| Practice | Top 10 (2 pages) | 5 sec | Focus on front runners |
| Qualifying | All 20 (4 pages) | 5 sec | Everyone matters |
| Race | Top 10 (2 pages) | 8 sec | Less flipping during race |
| Standings (post-session) | All 20 (4 pages) | 5 sec | Full results |

### Interaction with Points Toggle

When alternating between name and points (Mode 5):

```
Page 1 names (5 sec) → Page 1 points (5 sec) → Page 2 names (5 sec) → Page 2 points (5 sec) → ...
```

Or just names across all pages, then all points:
```
Page 1 names → Page 2 names → ... → Page 1 points → Page 2 points → ...
```

Configurable:
```json
{
  "standings": {
    "showPoints": true,
    "pointsMode": "alternate_per_page",  // alternate_per_page | names_then_points | points_only | names_only
    "pointsDelay": 5000
  }
}
```

---

## SUMMARY

| Category | Transitions Available |
|----------|---------------------|
| Standard mode changes | Instant, Cascade ↓, Cascade ↑, Sweep L→R, In-place, Rainbow, Car+Rainbow |
| Flags | Instant only (in), configurable (out) |
| Race events | Single row/cell |
| Race finish | Instant checkered → selectable celebration → selectable resolve |
| Scroll/Paging | Instant, Cascade ↓, Cascade ↑ (configurable delay) |
| Startup | Selectable |
| Shutdown | Selectable |
| **Total configurable points** | **~12 settings** |
