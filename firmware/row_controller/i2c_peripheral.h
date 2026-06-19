// ===========================================================
// I2C PERIPHERAL — Receives commands from ESP32 master
// ===========================================================
// Pico acts as I2C peripheral (row controller). Receives motor
// commands, responds to status/position reads from master.
// ===========================================================

#ifndef I2C_PERIPHERAL_H
#define I2C_PERIPHERAL_H

#include <stdint.h>

// --- COMMAND BYTES (must match master's i2c_master.h) ---
#define CMD_GOTO      0x01    // [CMD, col0_target, col1_target, col2_target, col3_target, col4_target]
#define CMD_HOME      0x02    // [CMD] — home all 5 motors
#define CMD_ANIMATE   0x03    // [CMD, col, startFlap, endFlap, intervalMs]
#define CMD_REPORT    0x04    // [CMD] — master requesting position dump

// --- STATUS BYTES ---
#define STATUS_IDLE       0x00   // All motors at target
#define STATUS_MOVING     0x01   // At least one motor in motion
#define STATUS_ERROR      0x02   // Motor stall or timeout detected
#define STATUS_NOT_HOMED  0xFF   // Hasn't been homed since power on

// --- INITIALIZATION ---

void initI2CPeripheral(uint8_t address);
// Configure I2C peripheral at given address.
// Register receive handler and request handler.
// Uses I2C0 on GP4 (SDA) and GP5 (SCL) — configurable.

// --- RECEIVE HANDLER ---

void onI2CReceive(int numBytes);
// Called when master sends data. Parses command byte and payload.
//
// CMD_GOTO:
//   Read 5 target positions. Store as pending targets.
//   Signal motor driver to begin moving all 5 motors to targets.
//
// CMD_HOME:
//   Trigger homing sequence for all 5 motors.
//
// CMD_ANIMATE:
//   Read col, startFlap, endFlap, intervalMs.
//   Signal motor driver to play frame sequence on specified col.
//
// CMD_REPORT:
//   No payload. Sets flag for request handler to send full position dump.

// --- REQUEST HANDLER ---

void onI2CRequest();
// Called when master reads from this slave.
//
// Default response (1 byte): current status byte.
//
// If CMD_REPORT was received: respond with 6 bytes:
//   [status, col0_pos, col1_pos, col2_pos, col3_pos, col4_pos]

// --- COMMAND PROCESSING ---

void processI2CCommand();
// Called in loop(). Checks if a new command was received (set by interrupt handler).
// Executes the command (non-blocking — starts motor motion, doesn't wait).

#endif
