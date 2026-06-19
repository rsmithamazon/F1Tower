// ===========================================================
// I2C MASTER — Communication with Pico row controllers
// ===========================================================
// ESP32 is I2C master. 5 Pico row controllers (one per row).
// Sends flap target commands, reads status/position reports.
// ===========================================================

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdint.h>

// --- I2C ADDRESSES ---
// Row 0: 0x10
// Row 1: 0x11
// Row 2: 0x12
// Row 3: 0x13
// Row 4: 0x14

#define ROW_COUNT        5
#define COLS_PER_ROW     5
#define I2C_BASE_ADDR    0x10

// --- INITIALIZATION ---

void initI2C();
// Setup I2C bus as master.
// SDA = GPIO 21, SCL = GPIO 22 (default ESP32 pins).
// Clock: 400kHz (fast mode).

// --- SENDING COMMANDS ---

void sendRowTargets(uint8_t row, uint8_t targets[COLS_PER_ROW]);
// Send 5 flap position targets to a single Pico.
//
// Args:
//   row: 0-4 (maps to I2C address 0x10-0x14)
//   targets: array of 5 flap positions (0-44)
//
// I2C write format: [CMD_GOTO, col0_pos, col1_pos, col2_pos, col3_pos, col4_pos]

void sendRowHome(uint8_t row);
// Command a row's Pico to home all its motors.
// I2C write format: [CMD_HOME]

void sendRowAnimation(uint8_t row, uint8_t col, uint8_t startFlap, uint8_t endFlap, uint8_t intervalMs);
// Command a specific motor to play a sequence of flaps.
// Used for car animation on Col 0.
//
// Args:
//   row: which row (0-4)
//   col: which column on that row (0-4)
//   startFlap: first frame position
//   endFlap: last frame position
//   intervalMs: delay between frames
//
// I2C write format: [CMD_ANIMATE, col, startFlap, endFlap, intervalMs]

void sendAllHome();
// Home all 5 rows simultaneously.

void sendAllTargets(uint8_t allTargets[ROW_COUNT][COLS_PER_ROW]);
// Send targets to all 5 rows.

// --- READING STATUS ---

uint8_t readRowStatus(uint8_t row);
// Read status byte from a Pico.
//
// Returns:
//   0x00 = idle (all motors at target)
//   0x01 = moving (at least one motor in motion)
//   0x02 = error (motor stall or timeout)
//   0xFF = not homed
//
// I2C read: request 1 byte from row controller address.

void readRowPositions(uint8_t row, uint8_t positions[COLS_PER_ROW]);
// Read current flap positions from a Pico.
//
// Args:
//   row: 0-4
//   positions: output array, filled with 5 current flap positions
//
// I2C read: request 6 bytes [status, col0, col1, col2, col3, col4].

bool isRowReady(uint8_t row);
// Check if a row has finished moving (status == 0x00).

bool areAllRowsReady();
// Check if ALL rows are idle. Used before starting animated transitions.

// --- I2C COMMAND BYTES ---
#define CMD_GOTO      0x01
#define CMD_HOME      0x02
#define CMD_ANIMATE   0x03
#define CMD_REPORT    0x04

#endif
