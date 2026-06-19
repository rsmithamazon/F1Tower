// ===========================================================
// FLAP LOOKUP — Convert content to flap positions
// ===========================================================
// Given a piece of content (team name, letter, color, etc.),
// returns the flap position for the appropriate column type.
// This is the firmware-side equivalent of flap_config.json.
// ===========================================================

#ifndef FLAP_LOOKUP_H
#define FLAP_LOOKUP_H

#include <stdint.h>

// Column types (drum types)
#define DRUM_COL0    0  // Position / Status / Animation
#define DRUM_COL1    1  // Logo / Digits / Colors / Symbols
#define DRUM_COL234  2  // Text / Numbers / Colors (identical)

// --- CONTENT → FLAP POSITION ---

uint8_t lookupColor(const char* colorId);
// Get flap position for a color. Same across all columns (0-7 base, 18-22 team).
//
// Args:
//   colorId: "black", "red", "white", "yellow", "green", "blue", "purple", "orange"
//            "teal", "pink", "lightBlue", "maroon", "gold" (col 0+1 only)
// Returns:
//   Flap position (0-25)

uint8_t lookupNumber(char digit);
// Get flap position for a digit 0-9. Same across all columns.
//
// Args:
//   digit: '0' through '9'
// Returns:
//   Flap position (8-17)

uint8_t lookupLetter(char letter);
// Get flap position for a letter A-Z. Cols 2-4 only.
//
// Args:
//   letter: 'A' through 'Z' (uppercase)
// Returns:
//   Flap position (18-43)

uint8_t lookupLogo(const char* teamId);
// Get flap position for a team logo. Col 1 only.
//
// Args:
//   teamId: "ferrari", "mclaren", "mercedes", "redbull", "astonmartin",
//           "alpine", "williams", "haas", "racingbulls", "audi", "cadillac"
// Returns:
//   Flap position (26-36)

uint8_t lookupIcon(const char* iconId);
// Get flap position for an icon. Col 0 only.
//
// Args:
//   iconId: "trophy", "pit"
// Returns:
//   Flap position (26-27)

uint8_t lookupSymbol(const char* symbolId, uint8_t drumType);
// Get flap position for a symbol. Varies by column type.
//
// Args:
//   symbolId: "plus", "dash", "period"
//   drumType: DRUM_COL0, DRUM_COL1, or DRUM_COL234
// Returns:
//   Flap position

uint8_t lookupAnimationFrame(uint8_t frame);
// Get flap position for a car animation frame. Col 0 only.
//
// Args:
//   frame: 1-15
// Returns:
//   Flap position (30-44)

// --- HIGH-LEVEL ROW CONVERSION ---

void convertRowToFlaps(const char* pos, const char* team, const char* text,
                       uint8_t output[5]);
// Convert a semantic row (position + team + 3-char text) into 5 flap positions.
//
// Args:
//   pos: position number as string ("1"-"20"), or icon ("trophy", "pit"), or color
//   team: team ID for logo column ("ferrari", etc.) or color name
//   text: 3-character string for cols 2-4 (e.g. "NOR", "PIT", "321")
//   output: filled with 5 flap positions [col0, col1, col2, col3, col4]
//
// Logic:
//   col0: if pos is numeric → lookupNumber. if icon → lookupIcon. if color → lookupColor.
//   col1: lookupLogo(team) or lookupColor(team) for color fills
//   col2-4: per character — if letter → lookupLetter. if digit → lookupNumber.
//           if '-' → lookupSymbol("dash"). if color → lookupColor.

void convertFullBoardToFlaps(/* parsed command data */, uint8_t output[5][5]);
// Convert a full display command (5 rows of semantic data) into 5×5 flap positions.
// Calls convertRowToFlaps for each row.

// --- COLOR FILLS ---

void fillAllColor(const char* colorId, uint8_t output[5][5]);
// Fill entire 5×5 board with one color.
// Used for flag states (all red, all yellow, etc.)

void fillRowColor(uint8_t row, const char* colorId, uint8_t output[5]);
// Fill one row with a single color.

#endif
