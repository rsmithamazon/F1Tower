// ===========================================================
// MOTOR DRIVER — High-level motor management
// ===========================================================
// Wraps PIO stepper with position tracking, animation sequencing,
// and stall detection. The "brain" layer above raw PIO control.
// ===========================================================

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

#define NUM_MOTORS       5
#define STALL_TIMEOUT_MS 5000   // If motor hasn't finished in 5 sec, flag error

// --- INITIALIZATION ---

void initMotorDriver();
// Initialize motor state tracking for all 5 motors.
// Sets all to "unknown position" until homed or loaded from flash.

// --- MOVEMENT COMMANDS ---

void moveToFlap(uint8_t motor, uint8_t targetFlap);
// Move a single motor to target flap position.
// Calculates steps via PIO stepper, starts motion.
// Updates target tracking.
//
// Args:
//   motor: 0-4
//   targetFlap: 0-44

void moveAllToFlaps(uint8_t targets[NUM_MOTORS]);
// Move all 5 motors to their respective targets.
// All start simultaneously (PIO parallel execution).
// Each finishes independently based on distance.

void playAnimation(uint8_t motor, uint8_t startFlap, uint8_t endFlap, uint8_t intervalMs);
// Queue a frame-by-frame animation on one motor.
// Steps through consecutive flap positions with delay between each.
//
// Implementation:
//   - Store animation params
//   - Set animating flag for this motor
//   - In updateMotors(): advance to next frame when interval elapsed

// --- STATUS ---

void updateMotors();
// Called every loop iteration. Services all 5 motors:
//   - Check if PIO finished stepping → update current position
//   - Check animation sequences → trigger next frame if interval elapsed
//   - Check stall timeout → flag error if exceeded
//
// This is the main "tick" function for motor management.

bool isMotorAtTarget(uint8_t motor);
// Return true if motor has reached its commanded target.

bool isMotorError(uint8_t motor);
// Return true if motor has stall/timeout error.

uint8_t getMotorStatus();
// Return aggregate status byte for all 5 motors:
//   STATUS_IDLE if all at target
//   STATUS_MOVING if any in motion
//   STATUS_ERROR if any has error

// --- POSITION ---

uint8_t getCurrentFlap(uint8_t motor);
// Return current flap position of a motor (0-44 or 0xFF if unknown).

void getAllPositions(uint8_t positions[NUM_MOTORS]);
// Fill array with current positions of all 5 motors.

void setCurrentFlap(uint8_t motor, uint8_t flap);
// Manually set a motor's known position (after homing or loading from flash).

// --- ERROR HANDLING ---

void clearMotorError(uint8_t motor);
// Clear error flag for a motor. Called after successful re-home.

void clearAllErrors();
// Clear all motor errors.

#endif
