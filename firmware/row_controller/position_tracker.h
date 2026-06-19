// ===========================================================
// POSITION TRACKER — Persistent flap position storage
// ===========================================================
// Stores current flap positions in Pico's flash memory.
// Survives power cycles. Used to skip homing on clean restart.
// Also provides position reporting to master via I2C reads.
// ===========================================================

#ifndef POSITION_TRACKER_H
#define POSITION_TRACKER_H

#include <stdint.h>

#define NUM_MOTORS  5
#define POS_UNKNOWN 0xFF   // Position not known (needs homing)

// --- INITIALIZATION ---

void initPositionTracker();
// Load stored positions from flash.
// Validate: all values must be 0-44 or 0xFF.
// If corrupt: set all to POS_UNKNOWN.

// --- POSITION MANAGEMENT ---

uint8_t getStoredPosition(uint8_t motor);
// Get last known position for a motor.
// Returns 0-44 if known, POS_UNKNOWN (0xFF) if not.

void setPosition(uint8_t motor, uint8_t flap);
// Update stored position for a motor in RAM.
// Called whenever a motor reaches its target.

void getAllPositions(uint8_t positions[NUM_MOTORS]);
// Fill array with all 5 current positions.

// --- PERSISTENCE ---

void savePositionsToFlash();
// Write current positions to Pico flash.
// Called on clean shutdown or periodically.
//
// Note: Pico flash write is page-based. Use a small reserved
// section. Write sparingly to preserve flash life.
// Strategy: only save when master sends shutdown command.

void loadPositionsFromFlash();
// Read positions from flash into RAM.

bool areStoredPositionsValid();
// Return true if all stored positions are in valid range (0-44).
// Used by startup logic to decide whether to trust stored state.

bool areAllAtBlack();
// Return true if all stored positions are 0 (black).
// Indicates a clean shutdown happened.

// --- STATUS BYTE ---

uint8_t getStatusByte();
// Build the status byte for I2C response to master.
//
// Logic:
//   if any motor has error → STATUS_ERROR
//   if any motor is moving → STATUS_MOVING
//   if any motor not homed and no stored pos → STATUS_NOT_HOMED
//   else → STATUS_IDLE

#endif
