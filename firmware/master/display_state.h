// ===========================================================
// DISPLAY STATE — Tracks what the board is currently showing
// ===========================================================
// Maintains a 5×5 grid of current flap positions.
// Used to calculate deltas (what needs to change) and
// persist state to NVS for clean restart.
// ===========================================================

#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include <stdint.h>

#define ROWS 5
#define COLS 5

// --- STATE MANAGEMENT ---

void initDisplayState();
// Load last known board state from NVS (flash).
// If corrupt or missing: mark as "unknown" (will force homing).

void updateCellState(uint8_t row, uint8_t col, uint8_t flapPos);
// Update a single cell's tracked position.
// Called after sending a target to a row controller.

void updateRowState(uint8_t row, uint8_t positions[COLS]);
// Update all 5 cells in a row at once.

uint8_t getCellState(uint8_t row, uint8_t col);
// Get the current (believed) flap position of a cell.

void getAllState(uint8_t state[ROWS][COLS]);
// Get full 5×5 board state.

bool isStateValid();
// Check if stored state is valid (not corrupt, all values 0-44).

bool isBoardAtBlack();
// Return true if all 25 cells are at position 0 (black).
// Used for startup logic (skip homing if already at black).

// --- PERSISTENCE ---

void saveStateToNVS();
// Write current 5×5 state to ESP32 NVS flash.
// Called on clean shutdown only (not every flip — preserves flash life).

void loadStateFromNVS();
// Read stored state from NVS. Validate integrity.
// If invalid: set all to 0xFF (unknown).

void clearStoredState();
// Wipe stored state. Forces homing on next boot.

#endif
