# ===========================================================
# BOARD STATE — Simulator's internal state of the 5×5 board
# ===========================================================
# Tracks current content per cell, handles transitions
# (animating from one state to another with timing).
# ===========================================================

import time
import json
import os

ROWS = 5
COLS = 5

# Load flap_config for position lookups
CONFIG_PATH = os.path.join(os.path.dirname(__file__), "..", "flap_config.json")


def load_flap_config():
    """Load flap_config.json and build lookup tables."""
    with open(CONFIG_PATH, "r") as f:
        return json.load(f)


def content_for_position(col, flap_pos, config):
    """
    Given a column (0-4) and flap position (0-44), return content dict.

    Returns:
        {"type": "color"|"number"|"letter"|"logo"|..., "value": "..."}
    """
    # Determine drum type
    if col == 0:
        drum = config["col0"]["flaps"]
    elif col == 1:
        drum = config["col1"]["flaps"]
    else:
        drum = config["col2_3_4"]["flaps"]

    # Find the flap entry at this position
    for flap in drum:
        if flap["pos"] == flap_pos:
            if flap["type"] == "color":
                return {"type": "color", "value": flap["id"]}
            elif flap["type"] == "number":
                return {"type": "number", "value": flap["value"]}
            elif flap["type"] == "letter":
                return {"type": "letter", "value": flap["value"]}
            elif flap["type"] == "logo":
                return {"type": "logo", "value": flap["id"]}
            elif flap["type"] == "icon":
                return {"type": "icon", "value": flap["id"]}
            elif flap["type"] == "symbol":
                return {"type": "symbol", "value": flap.get("label", flap["id"])}
            elif flap["type"] == "animation":
                return {"type": "animation", "value": flap["id"]}
            elif flap["type"] == "spare":
                return {"type": "color", "value": "black"}

    # Fallback
    return {"type": "color", "value": "black"}


class BoardState:
    """
    Maintains the current and target state of all 25 cells.
    Handles transition animation timing.
    """

    def __init__(self):
        self.config = load_flap_config()

        # Current displayed content (what's visible NOW)
        self.current = [[{"type": "color", "value": "black"} for _ in range(COLS)] for _ in range(ROWS)]

        # Target content (what we're transitioning TO)
        self.target = [[{"type": "color", "value": "black"} for _ in range(COLS)] for _ in range(ROWS)]

        # Flip animation state per cell (0.0 = static, >0 = animating)
        self.flip_progress = [[0.0 for _ in range(COLS)] for _ in range(ROWS)]

        # Transition queue
        self.transition_type = "instant"
        self.transition_active = False
        self.transition_step = 0
        self.transition_last_step_time = 0
        self.transition_row_delay = 0.4  # seconds between rows (cascade)
        self.transition_col_delay = 0.2  # seconds between cols (sweep)

    def set_all_color(self, color_id):
        """Set entire board to a single color (instant)."""
        for r in range(ROWS):
            for c in range(COLS):
                self.current[r][c] = {"type": "color", "value": color_id}
                self.target[r][c] = {"type": "color", "value": color_id}
        self.transition_active = False

    def set_target_from_command(self, command):
        """
        Parse a display command and set up transition.

        Args:
            command: dict with {cmd, mode, endState, transition, data}
        """
        data = command.get("data", {})
        transition = command.get("transition", "instant")

        if "rows" in data:
            # Full board update (5 rows)
            for row_idx, row_data in enumerate(data["rows"]):
                if row_idx >= ROWS:
                    break
                self._set_row_target(row_idx, row_data)
        elif "row" in data and "update" in data:
            # Single row update
            row_idx = data["row"]
            self._set_row_target(row_idx, data["update"])
            transition = "instant"  # single row updates are always instant

        self._start_transition(transition)

    def _set_row_target(self, row_idx, row_data):
        """Convert a row data dict to target cell contents."""
        pos = row_data.get("pos", "")
        team = row_data.get("team", "")
        text = row_data.get("text", "")

        # Col 0: position number or icon
        if pos.isdigit():
            self.target[row_idx][0] = {"type": "number", "value": pos}
        elif pos == "trophy":
            self.target[row_idx][0] = {"type": "icon", "value": "trophy"}
        elif pos in ("red", "yellow", "green", "blue", "purple", "orange", "black", "white"):
            self.target[row_idx][0] = {"type": "color", "value": pos}
        else:
            self.target[row_idx][0] = {"type": "number", "value": pos}

        # Col 1: team logo or color
        if team in ("red", "yellow", "green", "blue", "purple", "orange", "black", "white",
                    "teal", "pink", "lightBlue", "maroon", "gold"):
            self.target[row_idx][1] = {"type": "color", "value": team}
        else:
            self.target[row_idx][1] = {"type": "logo", "value": team}

        # Cols 2-4: text characters
        for i, char in enumerate(text[:3]):
            col = 2 + i
            if char.isdigit():
                self.target[row_idx][col] = {"type": "number", "value": char}
            elif char == "-":
                self.target[row_idx][col] = {"type": "symbol", "value": "-"}
            elif char == ".":
                self.target[row_idx][col] = {"type": "symbol", "value": "."}
            elif char == " ":
                self.target[row_idx][col] = {"type": "color", "value": "black"}
            else:
                self.target[row_idx][col] = {"type": "letter", "value": char.upper()}

    def _start_transition(self, transition_type):
        """Begin a transition animation."""
        self.transition_type = transition_type
        self.transition_step = 0
        self.transition_last_step_time = time.time()

        if transition_type == "instant" or transition_type == "none":
            # Apply immediately
            for r in range(ROWS):
                for c in range(COLS):
                    self.current[r][c] = self.target[r][c].copy()
            self.transition_active = False
        else:
            self.transition_active = True

    def update(self, dt):
        """
        Advance transition animation. Call every frame.

        Args:
            dt: time since last frame in seconds
        """
        if not self.transition_active:
            return

        now = time.time()
        elapsed = now - self.transition_last_step_time

        if self.transition_type == "cascade_down":
            # Reveal one row at a time, top to bottom
            if self.transition_step < ROWS:
                if elapsed >= self.transition_row_delay:
                    self._reveal_row(self.transition_step)
                    self.transition_step += 1
                    self.transition_last_step_time = now
            else:
                self.transition_active = False

        elif self.transition_type == "cascade_up":
            # Reveal one row at a time, bottom to top
            if self.transition_step < ROWS:
                if elapsed >= self.transition_row_delay:
                    row = ROWS - 1 - self.transition_step
                    self._reveal_row(row)
                    self.transition_step += 1
                    self.transition_last_step_time = now
            else:
                self.transition_active = False

        elif self.transition_type == "sweep_lr":
            # Reveal one column at a time, left to right
            if self.transition_step < COLS:
                if elapsed >= self.transition_col_delay:
                    self._reveal_col(self.transition_step)
                    self.transition_step += 1
                    self.transition_last_step_time = now
            else:
                self.transition_active = False

        elif self.transition_type == "in_place":
            # Reveal one row at a time with shorter delay
            if self.transition_step < ROWS:
                if elapsed >= 0.2:
                    self._reveal_row(self.transition_step)
                    self.transition_step += 1
                    self.transition_last_step_time = now
            else:
                self.transition_active = False

        elif self.transition_type in ("car_down", "car_rainbow", "car_color", "rainbow", "lights_out"):
            # Complex animations — simplified for simulator
            # For now: cascade with visual flair
            if self.transition_step < ROWS:
                if elapsed >= self.transition_row_delay:
                    self._reveal_row(self.transition_step)
                    self.transition_step += 1
                    self.transition_last_step_time = now
            else:
                self.transition_active = False

        else:
            # Unknown transition: instant fallback
            for r in range(ROWS):
                for c in range(COLS):
                    self.current[r][c] = self.target[r][c].copy()
            self.transition_active = False

        # Update flip animations
        for r in range(ROWS):
            for c in range(COLS):
                if self.flip_progress[r][c] > 0:
                    self.flip_progress[r][c] += dt * 4.0  # flip speed
                    if self.flip_progress[r][c] >= 1.0:
                        self.flip_progress[r][c] = 0.0

    def _reveal_row(self, row):
        """Instantly resolve a row to its target (with flip trigger)."""
        for c in range(COLS):
            if self.current[row][c] != self.target[row][c]:
                self.current[row][c] = self.target[row][c].copy()
                self.flip_progress[row][c] = 0.01  # trigger flip animation

    def _reveal_col(self, col):
        """Instantly resolve a column to its target (with flip trigger)."""
        for r in range(ROWS):
            if self.current[r][col] != self.target[r][col]:
                self.current[r][col] = self.target[r][col].copy()
                self.flip_progress[r][col] = 0.01
