// ===========================================================
// TRANSITION ENGINE — Manages how the board changes state
// ===========================================================
// Takes a target end state + transition type, breaks it into
// timed steps, sends I2C commands at the right moments.
// Handles "prepare to black" for animated transitions.
// Supports priority interrupts (flags override in-progress).
// ===========================================================

#ifndef TRANSITION_ENGINE_H
#define TRANSITION_ENGINE_H

#include <stdint.h>

// --- TRANSITION TYPES ---
// Matches transition IDs from transitions.md

#define TRANS_NONE           0   // No transition, direct update
#define TRANS_INSTANT        1   // All 25 units flip at once
#define TRANS_CASCADE_DOWN   2   // Row 0 → 1 → 2 → 3 → 4
#define TRANS_CASCADE_UP     3   // Row 4 → 3 → 2 → 1 → 0
#define TRANS_SWEEP_LR       4   // Col 0 → 1 → 2 → 3 → 4
#define TRANS_IN_PLACE       5   // Pos 1 resolves, then pos 2, then pos 3...
#define TRANS_CAR_DOWN       6   // Car animates down col 0
#define TRANS_CAR_RAINBOW    7   // Car + rainbow fill
#define TRANS_CAR_COLOR      8   // Car + solid color fill
#define TRANS_RAINBOW        9   // Rolling rainbow wave
#define TRANS_RANDOM         10  // Random offset starts
#define TRANS_LIGHTS_OUT     11  // Red row-by-row → all white (startup)

// --- PRIORITY LEVELS ---
#define PRIORITY_LOW         0   // Normal mode changes, page scrolls
#define PRIORITY_MEDIUM      1   // User-initiated changes
#define PRIORITY_HIGH        2   // Safety car, session events
#define PRIORITY_CRITICAL    3   // Red/yellow flag — interrupts everything

// --- TRANSITION PROPERTIES ---

typedef struct {
    uint8_t type;
    bool needsCleanStart;       // Must go to black before starting
    uint8_t priority;
} TransitionConfig;

// --- ENGINE STATE ---

void initTransitionEngine();
// Reset engine state. No active transition.

void startTransition(uint8_t transType, uint8_t targetState[5][5], uint8_t priority);
// Begin a new transition.
//
// Args:
//   transType: TRANS_* constant
//   targetState: 5×5 array of target flap positions
//   priority: PRIORITY_* level
//
// Logic:
//   1. If current transition in progress AND new priority > current:
//      abort current, start new immediately.
//   2. If needsCleanStart: send all to black, wait for ready, then begin.
//   3. Set up timing state machine for the transition type.
//   4. First step executes immediately.

void updateTransitionEngine();
// Called every loop(). Advances the transition state machine.
//
// Checks:
//   - Is it time for the next step? (millis-based timing)
//   - Has the current row finished? (poll slave status if needed)
//   - If transition complete: mark done, update display state.

bool isTransitionActive();
// Return true if a transition is currently in progress.

void abortTransition();
// Cancel current transition immediately.
// Used when a higher-priority command arrives.

uint8_t getTransitionProgress();
// Return 0-100 percentage of transition completion.
// Used for status reporting to host.

// --- SPECIFIC TRANSITION LOGIC ---
// Each is a state machine called by updateTransitionEngine()

void stepCascadeDown();
// State: which row are we on? (0-4)
// Each step: send targets to next row, advance after delay.
// Delay between rows: configurable (default 400ms).

void stepCascadeUp();
// Same as cascade down but row 4 → 0.

void stepSweepLR();
// State: which column? (0-4)
// Send that column's target to ALL rows at once.
// Delay between columns: configurable (default 200ms).

void stepCarDown();
// State: which row is the car on? (0-4)
// Per step:
//   - Send animation command to current row Col 0 (play frames)
//   - When frames done: advance car to next row
//   - Previous row resolves to final state

void stepCarRainbow();
// State: which row is the car on + which color column?
// Interleaves car animation with rainbow fill per row.

void stepRainbow();
// State: which row, which color?
// Fills row-by-row with cycling colors.

void stepLightsOut();
// State: which row has been turned red? (0-4)
// Per step: send row to all-red, delay ~800ms.
// After all 5 red: pause 1 sec, then all white.

void stepRandom();
// Send all targets at once but with random per-unit start delays.
// Implemented: send each unit with a random delay offset (0-500ms).

// --- HELPER ---

bool doesTransitionNeedCleanStart(uint8_t transType);
// Lookup whether a transition type requires all-black before starting.
// car_down, car_rainbow, car_color, rainbow, lights_out → true
// instant, cascade, sweep, in_place, random, none → false

#endif
