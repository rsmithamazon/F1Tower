// ===========================================================
// PIO STEPPER — Hardware-precise stepper control via RP2040 PIO
// ===========================================================
// Each motor gets its own PIO state machine.
// PIO generates step signals autonomously — zero CPU jitter.
// CPU just tells PIO "step N times at X interval" and it runs.
// ===========================================================

#ifndef PIO_STEPPER_H
#define PIO_STEPPER_H

#include <stdint.h>

#define NUM_MOTORS       5
#define STEPS_PER_REV    2048    // 28BYJ-48 half-step
#define FLAPS_PER_REV    45
#define STEP_INTERVAL_US 2000   // 2ms between steps (safe for 28BYJ-48 at 5V)

// --- INITIALIZATION ---

void initPIOSteppers();
// Load PIO program for half-step stepper drive.
// Assign one state machine per motor (SM0-SM4 across PIO0 + PIO1).
// Configure GPIO pins for each motor's 4 phase outputs.
//
// Pin assignments:
//   Motor 0 (Col 0): GP6, GP7, GP8, GP9
//   Motor 1 (Col 1): GP10, GP11, GP12, GP13
//   Motor 2 (Col 2): GP14, GP15, GP16, GP17
//   Motor 3 (Col 3): GP18, GP19, GP20, GP21
//   Motor 4 (Col 4): GP22, GP26, GP27, GP28
//
// PIO handles the half-step sequence (8 phases) internally.
// CPU writes number of steps to PIO TX FIFO → PIO executes them.

// --- MOTOR CONTROL ---

void startMotor(uint8_t motor, int16_t steps);
// Command a motor to step N steps forward (positive) or backward (negative).
// Non-blocking: writes step count to PIO FIFO, PIO runs autonomously.
//
// Args:
//   motor: 0-4
//   steps: signed step count. Positive = forward (clockwise), negative = reverse.
//
// Note: For split-flap, always go FORWARD (drums only spin one direction).
//       If target is "behind" current position, go forward through full revolution.

void startMotorToFlap(uint8_t motor, uint8_t targetFlap);
// Higher-level: move motor to a specific flap position.
// Calculates steps needed using lookup table and current position.
// Always moves forward (wraps around if needed).
//
// Args:
//   motor: 0-4
//   targetFlap: 0-44

void startAllMotorsToFlaps(uint8_t targets[NUM_MOTORS]);
// Start all 5 motors moving to their targets simultaneously.
// Each PIO state machine runs independently = true parallel motion.

void startMotorAnimation(uint8_t motor, uint8_t startFlap, uint8_t endFlap, uint8_t intervalMs);
// Play a sequence of flaps on one motor (for car animation).
// Steps through startFlap → startFlap+1 → ... → endFlap with intervalMs between each.
//
// Implemented as a series of single-flap moves queued internally.
// Uses a software timer to trigger each next frame (PIO handles the stepping).

// --- STATUS ---

bool isMotorMoving(uint8_t motor);
// Return true if PIO state machine for this motor is still stepping.
// Checks PIO FIFO empty + step counter.

bool isAnyMotorMoving();
// Return true if any of the 5 motors is still in motion.

bool areAllMotorsIdle();
// Return true if all 5 motors have reached their targets.

// --- STEP LOOKUP TABLE ---
// Pre-computed step positions for each flap (0-44).
// Accounts for fractional steps (2048/45 = 45.51 steps/flap).

extern const uint16_t FLAP_STEP_TABLE[FLAPS_PER_REV];
// FLAP_STEP_TABLE[0]  = 0
// FLAP_STEP_TABLE[1]  = 46
// FLAP_STEP_TABLE[2]  = 91
// FLAP_STEP_TABLE[3]  = 137
// ...
// FLAP_STEP_TABLE[44] = 2003
// Total revolution = 2048 steps

uint16_t getStepsForFlap(uint8_t flap);
// Return absolute step position for a given flap number.

int16_t calculateStepsToTarget(uint8_t currentFlap, uint8_t targetFlap);
// Calculate forward step count from current to target flap.
// Always positive (forward only). Wraps around if target < current.
//
// Formula:
//   if target >= current:
//     steps = FLAP_STEP_TABLE[target] - FLAP_STEP_TABLE[current]
//   else:
//     steps = (2048 - FLAP_STEP_TABLE[current]) + FLAP_STEP_TABLE[target]

#endif
