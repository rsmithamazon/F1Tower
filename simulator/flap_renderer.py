# ===========================================================
# FLAP RENDERER — Draws a single split-flap module on screen
# ===========================================================
# Handles the visual representation of one flap cell:
# colors, numbers, letters, logos (as colored squares + text),
# and the flip animation effect.
# ===========================================================

import pygame

# Flap visual dimensions
FLAP_WIDTH = 120
FLAP_HEIGHT = 100
FLAP_MARGIN = 6
FLAP_RADIUS = 8
SPLIT_GAP = 2  # gap between top/bottom halves

# Colors from flap_config.json
BASE_COLORS = {
    "black":  (0, 0, 0),
    "red":    (220, 0, 0),
    "white":  (255, 255, 255),
    "yellow": (255, 215, 0),
    "green":  (0, 111, 98),
    "blue":   (30, 65, 255),
    "purple": (160, 32, 240),
    "orange": (255, 128, 0),
}

TEAM_COLORS = {
    "teal":      (0, 210, 190),
    "pink":      (255, 135, 188),
    "lightBlue": (0, 90, 255),
    "maroon":    (144, 0, 0),
    "gold":      (200, 169, 81),
}

ALL_COLORS = {**BASE_COLORS, **TEAM_COLORS}

# Team abbreviations for logo display
TEAM_ABBREV = {
    "ferrari": "FER", "mclaren": "MCL", "mercedes": "MER",
    "redbull": "RBR", "astonmartin": "AMR", "alpine": "ALP",
    "williams": "WIL", "haas": "HAS", "racingbulls": "RBU",
    "audi": "AUD", "cadillac": "CAD",
}

# Team primary colors for logo background
TEAM_PRIMARY = {
    "ferrari": "red", "mclaren": "orange", "mercedes": "teal",
    "redbull": "blue", "astonmartin": "green", "alpine": "pink",
    "williams": "lightBlue", "haas": "white", "racingbulls": "purple",
    "audi": "maroon", "cadillac": "gold",
}


def get_text_color(bg_color_tuple):
    """Return black or white text depending on background brightness."""
    r, g, b = bg_color_tuple
    luminance = (0.299 * r + 0.587 * g + 0.114 * b)
    return (0, 0, 0) if luminance > 140 else (255, 255, 255)


def draw_flap(surface, x, y, content, font_large, font_small, flip_progress=0.0):
    """
    Draw a single flap module at position (x, y).

    Args:
        surface: pygame surface to draw on
        x, y: top-left position
        content: dict with {type, value} describing what to show
        font_large: pygame font for main content (numbers, letters)
        font_small: pygame font for small text (logo abbreviations)
        flip_progress: 0.0 = static, 0.0–1.0 = animating flip
    """
    bg_color = (20, 20, 20)  # default dark background
    text = ""
    text_color = (255, 255, 255)

    if content["type"] == "color":
        color_id = content["value"]
        bg_color = ALL_COLORS.get(color_id, (20, 20, 20))
        # No text for solid colors
        text = ""

    elif content["type"] == "number":
        bg_color = (20, 20, 20)
        text = content["value"]
        text_color = (255, 255, 255)

    elif content["type"] == "letter":
        bg_color = (20, 20, 20)
        text = content["value"]
        text_color = (255, 255, 255)

    elif content["type"] == "logo":
        team_id = content["value"]
        color_name = TEAM_PRIMARY.get(team_id, "black")
        bg_color = ALL_COLORS.get(color_name, (20, 20, 20))
        text = TEAM_ABBREV.get(team_id, "???")
        text_color = get_text_color(bg_color)

    elif content["type"] == "icon":
        bg_color = (20, 20, 20)
        if content["value"] == "trophy":
            text = "T"  # Trophy symbol (could use unicode but keeping simple)
            text_color = (255, 215, 0)
        elif content["value"] == "pit":
            text = "P"
            text_color = (255, 100, 100)

    elif content["type"] == "symbol":
        bg_color = (20, 20, 20)
        text = content.get("value", content.get("label", ""))
        text_color = (255, 255, 255)

    elif content["type"] == "animation":
        # Show animation frame as a car icon placeholder
        bg_color = (20, 20, 20)
        text = ">"  # simple car representation
        text_color = (255, 200, 0)

    # Draw the flap body (split in half for realism)
    top_rect = pygame.Rect(x, y, FLAP_WIDTH, FLAP_HEIGHT // 2 - SPLIT_GAP)
    bottom_rect = pygame.Rect(x, y + FLAP_HEIGHT // 2 + SPLIT_GAP, FLAP_WIDTH, FLAP_HEIGHT // 2 - SPLIT_GAP)

    # Apply flip animation (squish the top/bottom during flip)
    if 0 < flip_progress < 1.0:
        scale = abs(1.0 - 2.0 * flip_progress)  # 1→0→1 through the flip
        half_h = int((FLAP_HEIGHT // 2 - SPLIT_GAP) * max(scale, 0.1))
        center_y = y + FLAP_HEIGHT // 2
        top_rect = pygame.Rect(x, center_y - half_h - SPLIT_GAP, FLAP_WIDTH, half_h)
        bottom_rect = pygame.Rect(x, center_y + SPLIT_GAP, FLAP_WIDTH, half_h)

    pygame.draw.rect(surface, bg_color, top_rect, border_radius=FLAP_RADIUS)
    pygame.draw.rect(surface, bg_color, bottom_rect, border_radius=FLAP_RADIUS)

    # Draw text centered on the flap
    if text:
        font = font_small if len(text) > 1 else font_large
        text_surface = font.render(text, True, text_color)
        text_rect = text_surface.get_rect(center=(x + FLAP_WIDTH // 2, y + FLAP_HEIGHT // 2))
        surface.blit(text_surface, text_rect)

    # Draw subtle border
    full_rect = pygame.Rect(x, y, FLAP_WIDTH, FLAP_HEIGHT)
    pygame.draw.rect(surface, (60, 60, 60), full_rect, width=1, border_radius=FLAP_RADIUS)
