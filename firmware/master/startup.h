// ===========================================================
// STARTUP — Boot sequence and shutdown logic
// ===========================================================
// Handles the full power-on sequence:
//   1. Load config + stored state
//   2. Home motors (if needed)
//   3. Startup animation (black → red rows → white)
//   4. Transition to first display mode
//
// Also handles clean shutdown (persist state → all black → off).
// ===========================================================

#ifndef STARTUP_H
#define STARTUP_H

// --- STARTUP ---

void runStartupSequence();
// Full startup sequence.
//
// Logic:
//   1. Read stored state from NVS
//   2. Check homeOnStartup config toggle:
//      - If true: home all motors regardless
//      - If false:
//          a. If stored state is valid AND all positions == 0 (black):
//             skip homing (fast boot)
//          b. If stored state corrupt/missing OR positions != 0:
//             home all motors (fallback)
//   3. Board is now confirmed at all-black (position 0)
//   4. Run lights-out animation:
//      - Row 0 → all red, delay
//      - Row 1 → all red, delay
//      - ...
//      - Row 4 → all red, delay
//      - Pause (all red = 5 lights on!)
//      - All 25 → white (LIGHTS OUT)
//   5. Brief pause on all-white
//   6. Transition to first display mode (config.startupTransition)

void homeAllMotors();
// Send HOME command to all 5 Picos.
// Wait for all to report status == idle (homed).
// Timeout after 10 seconds — if any row doesn't respond, log error.
// After homing: all motors at position 0 (black).

void waitForAllHomed();
// Poll each Pico status until all report idle.
// Uses millis() for non-blocking timeout.

// --- SHUTDOWN ---

void runShutdownSequence();
// Clean shutdown sequence.
//
// Logic:
//   1. Send all rows to black (position 0) with configured transition
//   2. Wait for all rows to reach black
//   3. Save current state to NVS (all zeros)
//   4. (Optional) send "power down" signal — or just let user pull power

#endif
