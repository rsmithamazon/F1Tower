// ===========================================================
// F1 SPLIT FLAP TOWER — PICO ROW CONTROLLER FIRMWARE
// ===========================================================
// Runs on Raspberry Pi Pico (RP2040). One per row (5 total).
// Receives flap target commands from ESP32 over I2C.
// Drives 5 stepper motors using PIO for hardware-precise timing.
// Handles homing via hall effect sensors.
// ===========================================================

#include "i2c_peripheral.h"
#include "motor_driver.h"
#include "homing.h"
#include "position_tracker.h"
#include "pio_stepper.h"

// --- CONFIGURATION ---
// Set per-unit before flashing (or via EEPROM)

#define MY_ROW_INDEX     0          // 0-4, determines I2C address
#define I2C_ADDRESS      (0x10 + MY_ROW_INDEX)
#define COLS_PER_ROW     5

// --- SETUP ---

void setup() {
    // Serial.begin(115200)          — debug output
    // initPositionTracker()          — load stored positions from flash
    // initPIOSteppers()              — configure 5 PIO state machines for motors
    // initHallSensors()              — configure 5 hall effect sensor pins
    // initI2CPeripheral(I2C_ADDRESS)  — join I2C bus at configured address
    // reportReady()                  — set status to idle (or not-homed if no stored state)
}

// --- MAIN LOOP ---

void loop() {
    // processI2CCommand()            — check if new command received, execute it
    // updateMotors()                 — service PIO (check completion, update positions)
    // updateStatus()                 — refresh status byte based on motor states
}
