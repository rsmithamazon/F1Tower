// Split Flap F1 Display — Animation & End State Modes
// 4 columns × 10 rows
// Col 0: Left column (numbers, logos, car frames, colors)
// Cols 1-3: Right columns (letters, numbers, colors)

// =============================================================
// FLAP DEFINITIONS
// =============================================================

// Left column flap positions (50 flaps)
#define FLAP_NUM_0      0   // Numbers 0-9
#define FLAP_NUM_9      9
#define FLAP_LOGO_START 10  // Team logos 10-19
#define FLAP_CAR_START  20  // Car animation frames 20-29
#define FLAP_COL_START  30  // Colors 30-39
#define FLAP_EXTRA      40  // Extras 40-49 (blank/black/flag)
#define FLAP_BLACK      40
#define FLAP_BLANK      41

// Right columns flap positions (50 flaps)
#define FLAP_A          0   // Letters A-Z: 0-25
#define FLAP_Z          25
#define FLAP_R_NUM_0    26  // Numbers 0-9: 26-35
#define FLAP_R_NUM_9    35
#define FLAP_R_COL_START 36 // Colors: 36-45
#define FLAP_R_COLON    46  // Extras: 46-49
#define FLAP_R_DASH     47
#define FLAP_R_DOT      48
#define FLAP_R_BLANK    49

// Color indices (offset from COL_START on left, R_COL_START on right)
#define COL_RED     0
#define COL_ORANGE  1
#define COL_YELLOW  2
#define COL_GREEN   3
#define COL_BLUE    4
#define COL_PURPLE  5
#define COL_PINK    6
#define COL_WHITE   7
#define COL_CYAN    8
#define COL_BLACK   9

// Team logo indices (offset from LOGO_START)
#define TEAM_REDBULL   0
#define TEAM_FERRARI   1
#define TEAM_MERCEDES  2
#define TEAM_MCLAREN   3
#define TEAM_ASTONM    4
#define TEAM_ALPINE    5
#define TEAM_WILLIAMS  6
#define TEAM_HAAS      7
#define TEAM_ALPHATAURI 8
#define TEAM_SAUBER    9

// Team colors mapped to color indices
const byte TEAM_COLORS[10] = {
    COL_BLUE,    // Red Bull (dark blue)
    COL_RED,     // Ferrari
    COL_CYAN,    // Mercedes (teal/cyan)
    COL_ORANGE,  // McLaren
    COL_GREEN,   // Aston Martin
    COL_BLUE,    // Alpine
    COL_BLUE,    // Williams
    COL_WHITE,   // Haas
    COL_BLUE,    // AlphaTauri
    COL_RED      // Sauber
};

const byte RAINBOW[10] = {COL_RED, COL_ORANGE, COL_YELLOW, COL_GREEN, COL_BLUE, COL_PURPLE, COL_PINK, COL_WHITE, COL_CYAN, COL_BLACK};

#define NUM_ROWS 10
#define NUM_COLS 4

// =============================================================
// DRIVER DATA
// =============================================================

struct Driver {
    byte position;      // 1-10
    char initials[4];   // "VER", "HAM", etc.
    byte team;          // TEAM_REDBULL, etc.
    char lapTime[4];    // "123" → displayed as 1:23
};

// Example data
Driver drivers[10] = {
    {1,  "VER", TEAM_REDBULL,  "123"},
    {2,  "HAM", TEAM_MERCEDES, "124"},
    {3,  "LEC", TEAM_FERRARI,  "124"},
    {4,  "NOR", TEAM_MCLAREN,  "125"},
    {5,  "SAI", TEAM_FERRARI,  "125"},
    {6,  "ALO", TEAM_ASTONM,   "126"},
    {7,  "RUS", TEAM_MERCEDES, "126"},
    {8,  "PIA", TEAM_MCLAREN,  "127"},
    {9,  "STR", TEAM_ASTONM,   "127"},
    {10, "GAS", TEAM_ALPINE,   "128"}
};

// =============================================================
// HELPER: Set a flap position
// =============================================================

void setFlap(int col, int row, byte flapPosition);  // implemented by hardware layer

byte charToFlap(char c) {
    if (c >= 'A' && c <= 'Z') return FLAP_A + (c - 'A');
    if (c >= '0' && c <= '9') return FLAP_R_NUM_0 + (c - '0');
    if (c == ':') return FLAP_R_COLON;
    if (c == '-') return FLAP_R_DASH;
    if (c == '.') return FLAP_R_DOT;
    return FLAP_R_BLANK;
}

byte leftColor(byte colorIdx) { return FLAP_COL_START + colorIdx; }
byte rightColor(byte colorIdx) { return FLAP_R_COL_START + colorIdx; }
byte leftNum(byte n) { return FLAP_NUM_0 + n; }
byte leftLogo(byte team) { return FLAP_LOGO_START + team; }

// =============================================================
// END STATE 1: Position + Driver Initials
// =============================================================
// Layout:
//   [ 1] [V] [E] [R]
//   [ 2] [H] [A] [M]
//   [ 3] [L] [E] [C]
//   ...
//   [10] [G] [A] [S]

void endState_PositionAndInitials() {
    for (int row = 0; row < NUM_ROWS; row++) {
        // Left col: position number (0-9 for 1-10, using 0 for 10)
        setFlap(0, row, leftNum(drivers[row].position % 10));

        // Right cols: driver initials
        for (int col = 1; col < 4; col++) {
            setFlap(col, row, charToFlap(drivers[row].initials[col - 1]));
        }
    }
}

// =============================================================
// END STATE 2: Rows of matching team colors
// =============================================================
// Layout:
//   [RED ] [RED ] [RED ] [RED ]   ← Ferrari
//   [BLUE] [BLUE] [BLUE] [BLUE]  ← Red Bull
//   [CYAN] [CYAN] [CYAN] [CYAN]  ← Mercedes
//   ...

void endState_TeamColorRows() {
    for (int row = 0; row < NUM_ROWS; row++) {
        byte color = TEAM_COLORS[drivers[row].team];
        setFlap(0, row, leftColor(color));
        for (int col = 1; col < 4; col++) {
            setFlap(col, row, rightColor(color));
        }
    }
}

// =============================================================
// END STATE 3: Logo + Name / Position + Lap Time (2 rows per driver)
// =============================================================
// Layout:
//   [LOGO] [V] [E] [R]   ← team logo + driver name
//   [  1 ] [1] [:] [23]  ← position + lap time
//   [LOGO] [H] [A] [M]
//   [  2 ] [1] [:] [24]
//   ...
// Fits 5 drivers in 10 rows

void endState_LogoNameTime() {
    for (int i = 0; i < 5; i++) {
        int topRow = i * 2;
        int botRow = i * 2 + 1;

        // Top row: logo + initials
        setFlap(0, topRow, leftLogo(drivers[i].team));
        for (int col = 1; col < 4; col++) {
            setFlap(col, topRow, charToFlap(drivers[i].initials[col - 1]));
        }

        // Bottom row: position + lap time (e.g. "123" → 1:23)
        setFlap(0, botRow, leftNum(drivers[i].position % 10));
        setFlap(1, botRow, charToFlap(drivers[i].lapTime[0]));
        setFlap(2, botRow, FLAP_R_COLON);
        setFlap(3, botRow, charToFlap(drivers[i].lapTime[1]));
        // Note: only showing M:SS — adjust if you need M:SS.s
    }
}

// =============================================================
// END STATE 4: Position + Lap Time only (logo on left)
// =============================================================
// Layout:
//   [LOGO] [1] [:] [2]   ← logo + time
//   [LOGO] [1] [:] [2]
//   ...
// All 10 drivers, one row each

void endState_LogoAndTime() {
    for (int row = 0; row < NUM_ROWS; row++) {
        setFlap(0, row, leftLogo(drivers[row].team));
        setFlap(1, row, charToFlap(drivers[row].lapTime[0]));
        setFlap(2, row, FLAP_R_COLON);
        setFlap(3, row, charToFlap(drivers[row].lapTime[1]));
    }
}

// =============================================================
// END STATE 5: Logo + Color Row
// =============================================================
// Layout:
//   [LOGO] [RED ] [RED ] [RED ]   ← Ferrari logo + red
//   [LOGO] [BLUE] [BLUE] [BLUE]  ← Red Bull logo + blue
//   [LOGO] [CYAN] [CYAN] [CYAN]  ← Mercedes logo + teal
//   ...

void endState_LogoAndColor() {
    for (int row = 0; row < NUM_ROWS; row++) {
        byte color = TEAM_COLORS[drivers[row].team];
        setFlap(0, row, leftLogo(drivers[row].team));
        for (int col = 1; col < 4; col++) {
            setFlap(col, row, rightColor(color));
        }
    }
}

// =============================================================
// ANIMATION: Rolling Rainbow (color pushes left → right)
// =============================================================
// Each new color enters col 1, previous colors shift right
// Creates diagonal color stripe moving down the board

void anim_RollingRainbow(void (*resolveRow)(int)) {
    byte colState[4] = {COL_BLACK, COL_BLACK, COL_BLACK, COL_BLACK};
    int currentRow = 0;

    for (int step = 0; step < 12; step++) {  // 10 colors + 2 to flush
        // Shift right: col3 gets col2's color, col2 gets col1's
        colState[3] = colState[2];
        colState[2] = colState[1];
        colState[1] = (step < 10) ? RAINBOW[step] : COL_BLACK;

        // Car position on left column
        if (step < 10) {
            setFlap(0, step, FLAP_CAR_START + (step % 10));
        }

        // Update columns — each column shows its current color on its respective row
        // Col 1 is on current row, col 2 is 1 row behind, col 3 is 2 behind
        if (step < 10) {
            setFlap(1, step, rightColor(colState[1]));
        }
        if (step - 1 >= 0 && step - 1 < 10) {
            setFlap(2, step - 1, rightColor(colState[2]));
        }
        if (step - 2 >= 0 && step - 2 < 10) {
            setFlap(3, step - 2, rightColor(colState[3]));
        }

        // Resolve rows that are fully colored (3 steps behind)
        if (step >= 3 && resolveRow != NULL) {
            resolveRow(step - 3);
        }

        delay(300);
    }

    // Resolve remaining rows
    if (resolveRow != NULL) {
        for (int row = 7; row < 10; row++) {
            resolveRow(row);
            delay(300);
        }
    }
}

// =============================================================
// ANIMATION: Team Color Fill (same color per row, left → right)
// =============================================================

void anim_TeamColorFill(void (*resolveRow)(int)) {
    for (int row = 0; row < NUM_ROWS; row++) {
        byte color = TEAM_COLORS[drivers[row].team];

        // Car drives down
        setFlap(0, row, FLAP_CAR_START + row);

        // Fill row with team color left → right
        for (int col = 1; col < 4; col++) {
            setFlap(col, row, rightColor(color));
            delay(300);
        }

        // Resolve previous row
        if (row > 0 && resolveRow != NULL) {
            resolveRow(row - 1);
        }
    }

    // Resolve last row
    if (resolveRow != NULL) {
        resolveRow(9);
    }
}

// =============================================================
// RESOLVE HELPERS (called by animations to set final state)
// =============================================================

void resolveToInitials(int row) {
    setFlap(0, row, leftNum(drivers[row].position % 10));
    for (int col = 1; col < 4; col++) {
        setFlap(col, row, charToFlap(drivers[row].initials[col - 1]));
    }
}

void resolveToLogoAndColor(int row) {
    byte color = TEAM_COLORS[drivers[row].team];
    setFlap(0, row, leftLogo(drivers[row].team));
    for (int col = 1; col < 4; col++) {
        setFlap(col, row, rightColor(color));
    }
}

// =============================================================
// MAIN SEQUENCES (combine animation + end state)
// =============================================================

// Rainbow cascade → resolve to position + initials
void sequence_RainbowToLeaderboard() {
    anim_RollingRainbow(resolveToInitials);
}

// Team color fill → resolve to logo + color
void sequence_TeamColorToLogo() {
    anim_TeamColorFill(resolveToLogoAndColor);
}
