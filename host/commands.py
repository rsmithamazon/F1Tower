# ===========================================================
# COMMANDS — Build display command messages for ESP32
# ===========================================================
# Helper functions to construct well-formed command dicts.
# These get JSON-serialized and sent over WebSocket.
# ===========================================================

# Command format:
# {
#     "cmd": "display" | "startup" | "shutdown" | "heartbeat" | "query_status",
#     "mode": str,           (display mode context)
#     "endState": str,       (target end state name)
#     "transition": str,     (transition style to use)
#     "data": dict           (end-state-specific payload)
# }


def cmd_display(mode, end_state, transition, data):
    """
    Build a standard display command.
    
    Args:
        mode: "standings", "constructors", "flag", "single_driver", etc.
        end_state: "driver_names", "driver_points", "all_red", etc.
        transition: "instant", "cascade_down", "car_rainbow", etc.
        data: dict with rows/row data specific to the end state
        
    Returns:
        Command dict ready for JSON serialization
    """
    # return {
    #     "cmd": "display",
    #     "mode": mode,
    #     "endState": end_state,
    #     "transition": transition,
    #     "data": data
    # }
    pass


def cmd_row_update(row_index, row_data):
    """
    Build a single-row update (no transition, just change that row).
    Used for race events: pit stops, position swaps.
    
    Args:
        row_index: 0-4
        row_data: {pos, team, text} for the updated row
    """
    # return {
    #     "cmd": "display",
    #     "mode": "standings",
    #     "endState": "driver_names",
    #     "transition": "none",
    #     "data": {"row": row_index, "update": row_data}
    # }
    pass


def cmd_startup():
    """Build startup animation command (black → red rows → white → first mode)."""
    # return {"cmd": "startup"}
    pass


def cmd_shutdown():
    """Build shutdown command (current → all black, motors off)."""
    # return {"cmd": "shutdown"}
    pass


def cmd_heartbeat():
    """Build heartbeat ping."""
    # return {"cmd": "heartbeat", "timestamp": time.time()}
    pass


def cmd_query_status():
    """Request full board state from ESP32."""
    # return {"cmd": "query_status"}
    pass


def cmd_change_setting(key, value):
    """
    Send a setting change to ESP32.
    E.g. change transition preference, scroll delay, etc.
    
    Args:
        key: setting name ("transition.modeChange", "scroll.pageDelay", etc.)
        value: new value
    """
    # return {"cmd": "set", "key": key, "value": value}
    pass
