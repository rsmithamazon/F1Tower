# ===========================================================
# CONFIG — Load and manage host application settings
# ===========================================================

def load_config(filepath):
    """
    Load config from JSON file.
    
    Returns config object with:
        - esp32_ip: str           (IP address of ESP32 on local network)
        - esp32_port: int         (WebSocket port, default 81)
        - data_source: str        (API URL, "mock", or file path for replay)
        - log_level: str          ("debug", "info", "warn", "error")
        - session_type: str       ("practice", "qualifying", "race", "manual")
        - display_mode: str       (default display mode on startup)
        - scroll_settings: dict   (page delay, drivers to show, loop)
        - transition_prefs: dict  (preferred transitions per event type)
    """
    # json.load(open(filepath))
    # validate required fields
    # return Config object
    pass


def save_config(filepath, config):
    """
    Save updated config back to file.
    Used when user changes settings at runtime.
    """
    pass
