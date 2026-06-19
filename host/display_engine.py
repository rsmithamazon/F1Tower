# ===========================================================
# DISPLAY ENGINE — Decides what to show and when
# ===========================================================
# The brain of the host. Takes F1 data updates, current mode,
# and user preferences → produces display commands for ESP32.
# Handles mode switching, page scrolling, event reactions.
# ===========================================================


class DisplayEngine:
    """
    Processes data changes into display commands.
    Manages current mode, page cycling, event priorities.
    """

    def __init__(self, ws_client, config):
        """
        Args:
            ws_client: WebSocketClient for sending commands
            config: loaded config with display preferences
        """
        # self.ws = ws_client
        # self.config = config
        # self.current_mode = config.display_mode
        # self.current_page = 0
        # self.page_timer = None
        # self.last_command_sent = None
        pass

    def process_update(self, data_update):
        """
        Core logic: given a data change, decide what command to send.
        
        Args:
            data_update: DataUpdate from F1DataManager
            
        Returns:
            Command dict {mode, endState, transition, data} or None
            
        Logic:
            - Flag change? → immediate flag command (highest priority)
            - Position change during race? → update affected rows (no transition)
            - Pit stop? → update that driver's row to "PIT"
            - Session ended? → switch to results mode
            - Page timer expired? → scroll to next page
            - Otherwise: no command needed
        """
        # if data_update.type == "flag":
        #     return self.build_flag_command(data_update.payload)
        # elif data_update.type == "positions":
        #     return self.build_standings_command()
        # elif data_update.type == "pit":
        #     return self.build_pit_command(data_update.payload)
        # ...
        pass

    def build_flag_command(self, flag_data):
        """
        Build a flag display command.
        
        Flags are always instant transition.
        
        Returns:
            {
                "mode": "flag",
                "endState": "all_red" | "all_yellow" | "safety_car" | "checkered" | ...,
                "transition": "instant",
                "data": {}
            }
        """
        pass

    def build_standings_command(self):
        """
        Build a standings display command for current page.
        
        Uses current_page to determine which 5 drivers to show.
        Picks transition based on config preferences.
        
        Returns:
            {
                "mode": "standings",
                "endState": "driver_names" | "driver_points" | "driver_gaps",
                "transition": "cascade_down" | "instant" | ...,
                "data": {
                    "rows": [ {pos, team, text}, ... ]
                }
            }
        """
        pass

    def build_pit_command(self, pit_data):
        """
        Build a single-row pit stop update.
        
        Returns:
            {
                "mode": "standings",
                "endState": "driver_names",
                "transition": "none",
                "data": {
                    "row": <row_index>,
                    "update": {pos, team, text: "PIT"}
                }
            }
        """
        pass

    def build_constructor_command(self):
        """Build constructor standings command."""
        pass

    def build_single_driver_command(self, driver_code):
        """Build single driver focus command."""
        pass

    def next_page(self):
        """
        Advance to next page of standings.
        Called by page scroll timer.
        Wraps around to page 0 after last page.
        """
        # self.current_page = (self.current_page + 1) % total_pages
        # return self.build_standings_command()
        pass

    def change_mode(self, new_mode):
        """
        Switch display mode (user-initiated or session-triggered).
        
        Args:
            new_mode: "standings", "constructors", "single_driver", etc.
        """
        # self.current_mode = new_mode
        # self.current_page = 0
        # build and return appropriate command with mode-change transition
        pass


class PageScroller:
    """
    Handles automatic page cycling with configurable delay.
    """

    def __init__(self, config):
        # self.delay = config.scroll_settings["pageDelay"]
        # self.drivers_to_show = config.scroll_settings["driversToShow"]
        # self.total_pages = drivers_to_show // 5
        # self.loop = config.scroll_settings["loop"]
        pass

    def get_page_drivers(self, page, all_drivers):
        """
        Return the 5 drivers for a given page number.
        
        Args:
            page: int (0-indexed)
            all_drivers: full sorted list of DriverEntry
            
        Returns:
            list of 5 DriverEntry for this page
        """
        # start = page * 5
        # return all_drivers[start:start+5]
        pass

    def should_advance(self, elapsed_ms):
        """Check if page delay has elapsed."""
        # return elapsed_ms >= self.delay
        pass
