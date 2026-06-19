# ===========================================================
# F1 DATA — Manages live/mock F1 timing data
# ===========================================================
# Connects to F1 data source (OpenF1 API, MultiViewer, file replay).
# Normalizes all data into a consistent internal format.
# Emits events when race state changes.
# ===========================================================


class F1DataManager:
    """
    Ingests F1 timing data and maintains current session state.
    """

    def __init__(self, data_source):
        """
        Args:
            data_source: "openf1", "mock", or filepath for replay
        """
        # self.source = data_source
        # self.session_state = SessionState()
        # self.last_update_time = None
        pass

    async def connect(self):
        """
        Connect to the data source.
        - "openf1": HTTP polling or SSE to api.openf1.org
        - "mock": load mock data, simulate updates on timer
        - filepath: load recorded session, replay at real speed or accelerated
        """
        pass

    async def poll(self):
        """
        Check for new data. Returns a change event or None.
        
        Returns:
            DataUpdate object if something changed, else None.
            DataUpdate contains:
                - type: "positions", "flag", "pit", "tire", "sector", "session_status"
                - payload: varies by type
        """
        # fetch new data from source
        # diff against self.session_state
        # if changed: update state, return DataUpdate
        # else: return None
        pass

    def get_standings(self):
        """
        Return current driver standings as list of DriverEntry.
        
        Each DriverEntry:
            - position: int (1-20)
            - driver_code: str ("NOR", "VER", etc.)
            - team: str ("ferrari", "mclaren", etc.)
            - gap: str ("+0.059", "LAP", etc.)
            - tire: str ("soft", "medium", "hard", "inter", "wet")
            - in_pit: bool
            - sectors: [color, color, color]  ("green", "purple", "yellow")
            - fastest_lap: bool
        """
        pass

    def get_session_info(self):
        """
        Return current session metadata.
        
        Returns:
            - session_type: "practice1/2/3", "qualifying", "sprint", "race"
            - status: "active", "finished", "red_flagged", "not_started"
            - current_flag: None, "red", "yellow", "green", "checkered"
            - safety_car: None, "sc", "vsc"
        """
        pass


class SessionState:
    """
    Internal state of the current F1 session.
    Updated incrementally as data arrives.
    """

    def __init__(self):
        # self.drivers = {}       # dict of driver_code → DriverEntry
        # self.flag = None
        # self.safety_car = None
        # self.session_type = None
        # self.session_status = None
        pass

    def update_positions(self, position_data):
        """Update driver positions from incoming data."""
        pass

    def update_flag(self, flag_data):
        """Update flag status."""
        pass

    def update_pit(self, pit_data):
        """Update pit stop status for a driver."""
        pass

    def update_tire(self, tire_data):
        """Update tire compound for a driver."""
        pass
