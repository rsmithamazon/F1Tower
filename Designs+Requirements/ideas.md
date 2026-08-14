col 1 reserved flaps
0-9 (10)
car animation (10)
Team logos (11)
Colors (up to 17)
-+ (2)

col 2-4 reserved flaps
0-9 (10)
letters (26)
Colors (up to 13)
period symbol



┌──────┬───────────────┐
│ LOGO │ V E R         │  ← team logo + driver name
│  1   │ 1 : 2 3       │  ← position + lap time
│ LOGO │ H A M         │
│  2   │ 1 : 2 4       │
│  ●   │               │  ← team color dot / black
└──────┴───────────────┘


end state layouts

left col position starting 1 ending 10 with driver initials

another end state is row of colors (all matching)
another is 
┌──────┬───────────────┐
│ LOGO │ V E R         │  ← team logo + driver name
│  1   │ 1 : 2 3       │  ← position + lap time
│ LOGO │ H A M         │
│  2   │ 1 : 2 4       │
...
same as above but just times and position or team logo


45


## End States

| Mode | Left Col | Right 3 Cols |
|------|----------|--------------|
| **1. Position + Initials** | 1–10 | VER, HAM, LEC... |
| **2. Team Color Rows** | Color | Color, Color, Color (matching) |
| **3. Logo + Name / Pos + Time** | Logo/Pos alternating | Name row, Time row (5 drivers, 2 rows each) |
| **4. Logo + Time** | Logo | M:SS (all 10, one per row) |
| **5. Logo + Color Row** | Logo | Team color fills row |

## Animations → End States

| Sequence | Animation | Resolves to |
|----------|-----------|-------------|
| Rainbow → Leaderboard | Rolling rainbow L→R | Position + Initials |
| Team Color → Logo | Team color fill L→R | Logo + Color Row |
car down column 1 
driver standings
constructor standings
finish flag (black and white)

Saved to `/workspace/splitflap-f1/end_states_and_sequences.md`. Here's the summary:

## End States

| # | State | Left Col | Right Cols |
|---|-------|----------|------------|
| 1 | **Driver Standings** | Position 1–10 | Initials (VER, HAM...) |
| 2 | **Constructor Standings** | Team logos | Team abbreviation or points |
| 3 | **Checkered Flag** | ⬛⬜ alternating | ⬜⬛ alternating |
| 4 | **Logo + Color Rows** | Logo | Team color solid |
| 5 | **Race Winner** | Trophy + logo | Winner name + team color |
| 6 | **Sector Times** | Sector color | Lap times |
| 7 | **Lights Out** | All red → all black | Same |
| 8 | **Fastest Lap** | Purple | Driver + time |
| 9 | **Red Flag / Safety Car** | All red | "SAFETY" or red fill |
| 10 | **DRS Enabled** | Green | "DRS" |
| 11 | **Pit Alert** | Logo | "PIT" + lap window |
| 12 | **Off/Sleep** | All black | All black |

## Sequences (Animation → End State)

| Sequence | Transition | Lands on |
|----------|-----------|----------|
| Leaderboard Reveal | Rainbow L→R + Car ↓ | Driver Standings |
| Constructor Reveal | Team color fill L→R | Constructor Standings |
| Race Finish | Checkered wipe | Winner Celebration |
| Lights Out | driver pos → black → Red row (x2) as each light goes on → back to postioins | Sleep |
| Position Change | Car ↓ + team color | Updated Standings |
| Fastest Lap | Purple wave | Fastest Lap display |
| Red Flag | Red flood (instant) | Red Flag state |
| Team Parade | Row-by-row team color | Logo + Color |



- **Gap to Leader** (+0.3, +1.2 in right cols)
- **Tire Compound** (red/yellow/white indicator)
- **Clock Mode** (idle display when no race)
- **Custom Message** (party mode / free text)
- **Championship Clinch** (special gold animation)


Colors needed
Teams - Red, Orange, Teal, Navy, Pink, Green (Aston), Light Blue, White, R Bulls, Gold, Audi (11) 
Flags - blue, yellow, red, green, checkered (5)
Sectors - purple, ,  (3)
Tires - , , , white,  (5)

Red
Orange
Teal
Navy
Pink
Green (Aston)
Light Blue
White
R Bulls
Gold
Audi  
blue
green
Black
purple
yellow
blue

Maybe do sectors and flags and tire (all three lights) as lights

or teams as logo on top and color on bottom



order matters
color should go warms to coldest for rainbow