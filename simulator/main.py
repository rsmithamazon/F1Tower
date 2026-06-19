# ===========================================================
# F1 SPLIT FLAP TOWER — VISUAL SIMULATOR
# ===========================================================
# Pygame window that renders the 5×5 split-flap board.
# Accepts WebSocket commands on port 81 (same as real ESP32).
# Use this to test the host application without hardware.
#
# Usage:
#   pip install -r requirements.txt
#   python main.py
#
# Then connect the host application to ws://localhost:81
# Or send manual test commands (see test_commands.py)
# ===========================================================

import asyncio
import sys
import os

import pygame

from board_state import BoardState
from flap_renderer import (
    draw_flap, FLAP_WIDTH, FLAP_HEIGHT, FLAP_MARGIN
)
from ws_server import SimulatorWSServer

# --- DISPLAY CONSTANTS ---
ROWS = 5
COLS = 5
WINDOW_PADDING = 30
HEADER_HEIGHT = 50

WINDOW_WIDTH = WINDOW_PADDING * 2 + COLS * (FLAP_WIDTH + FLAP_MARGIN) - FLAP_MARGIN
WINDOW_HEIGHT = HEADER_HEIGHT + WINDOW_PADDING * 2 + ROWS * (FLAP_HEIGHT + FLAP_MARGIN) - FLAP_MARGIN

BG_COLOR = (30, 30, 35)
FRAME_COLOR = (50, 50, 55)
HEADER_COLOR = (220, 220, 220)

FPS = 60
WS_PORT = 81


async def main():
    """Main entry: start pygame + WebSocket server, run game loop."""

    # --- PYGAME INIT ---
    pygame.init()
    screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    pygame.display.set_caption("F1 Split Flap Tower — Simulator")
    clock = pygame.time.Clock()

    font_large = pygame.font.SysFont("Consolas", 48, bold=True)
    font_small = pygame.font.SysFont("Consolas", 28, bold=True)
    font_header = pygame.font.SysFont("Segoe UI", 18)

    # --- STATE ---
    board = BoardState()
    ws_server = SimulatorWSServer(board, port=WS_PORT)

    # --- START WEBSOCKET ---
    await ws_server.start()

    # --- GAME LOOP ---
    running = True
    last_time = pygame.time.get_ticks() / 1000.0

    while running:
        # Calculate delta time
        now = pygame.time.get_ticks() / 1000.0
        dt = now - last_time
        last_time = now

        # --- EVENTS ---
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                elif event.key == pygame.K_r:
                    # R key = red flag test
                    board.set_all_color("red")
                elif event.key == pygame.K_g:
                    # G key = green (all clear)
                    board.set_all_color("green")
                elif event.key == pygame.K_b:
                    # B key = black (blank)
                    board.set_all_color("black")
                elif event.key == pygame.K_w:
                    # W key = white
                    board.set_all_color("white")
                elif event.key == pygame.K_t:
                    # T key = test standings
                    _send_test_standings(board)

        # --- PROCESS WEBSOCKET COMMANDS ---
        command = ws_server.get_pending_command()
        if command:
            cmd_type = command.get("cmd", "")
            if cmd_type == "display":
                board.set_target_from_command(command)
            elif cmd_type == "startup":
                # Simulate startup: black → red rows → white
                board.set_all_color("black")
                # Queue red cascade then white (simplified)
                board.target = [[{"type": "color", "value": "red"} for _ in range(COLS)] for _ in range(ROWS)]
                board._start_transition("cascade_down")
            elif cmd_type == "shutdown":
                board.set_all_color("black")

        # --- UPDATE ---
        board.update(dt)

        # --- DRAW ---
        screen.fill(BG_COLOR)

        # Header
        status_text = "TRANSITIONING" if board.transition_active else "IDLE"
        header = font_header.render(
            f"F1 Split Flap Simulator  |  ws://localhost:{WS_PORT}  |  {status_text}  |  Keys: R/G/B/W/T/Esc",
            True, HEADER_COLOR
        )
        screen.blit(header, (WINDOW_PADDING, 15))

        # Draw board frame
        board_x = WINDOW_PADDING
        board_y = HEADER_HEIGHT + WINDOW_PADDING
        frame_rect = pygame.Rect(
            board_x - 10, board_y - 10,
            COLS * (FLAP_WIDTH + FLAP_MARGIN) + 10,
            ROWS * (FLAP_HEIGHT + FLAP_MARGIN) + 10
        )
        pygame.draw.rect(screen, FRAME_COLOR, frame_rect, border_radius=12)

        # Draw each flap
        for row in range(ROWS):
            for col in range(COLS):
                x = board_x + col * (FLAP_WIDTH + FLAP_MARGIN)
                y = board_y + row * (FLAP_HEIGHT + FLAP_MARGIN)
                content = board.current[row][col]
                flip = board.flip_progress[row][col]
                draw_flap(screen, x, y, content, font_large, font_small, flip)

        # Column labels
        col_labels = ["Col 0", "Col 1", "Col 2", "Col 3", "Col 4"]
        for col in range(COLS):
            x = board_x + col * (FLAP_WIDTH + FLAP_MARGIN) + FLAP_WIDTH // 2
            label = font_header.render(col_labels[col], True, (100, 100, 100))
            label_rect = label.get_rect(center=(x, board_y - 18))
            screen.blit(label, label_rect)

        pygame.display.flip()
        clock.tick(FPS)

        # Yield to asyncio (allows WebSocket to process)
        await asyncio.sleep(0)

    pygame.quit()


def _send_test_standings(board):
    """Send a test standings display (keyboard shortcut T)."""
    test_command = {
        "cmd": "display",
        "mode": "standings",
        "endState": "driver_names",
        "transition": "cascade_down",
        "data": {
            "rows": [
                {"pos": "1", "team": "mclaren",    "text": "NOR"},
                {"pos": "2", "team": "ferrari",    "text": "LEC"},
                {"pos": "3", "team": "redbull",    "text": "VER"},
                {"pos": "4", "team": "mercedes",   "text": "HAM"},
                {"pos": "5", "team": "astonmartin", "text": "ALO"},
            ]
        }
    }
    board.set_target_from_command(test_command)


if __name__ == "__main__":
    asyncio.run(main())
