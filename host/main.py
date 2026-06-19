# ===========================================================
# F1 SPLIT FLAP TOWER — HOST APPLICATION (Python)
# ===========================================================
# Runs on PC/Raspberry Pi. Connects to F1 data source,
# processes race state, sends display commands to ESP32 via WebSocket.
# ===========================================================

import asyncio
from config import load_config
from connection import WebSocketClient
from f1_data import F1DataManager
from display_engine import DisplayEngine
from logger import Logger


async def main():
    """
    Entry point. Boots all systems, runs main event loop.
    
    Flow:
        1. Load config (display settings, WiFi target, mode prefs)
        2. Connect to ESP32 over WebSocket
        3. Connect to F1 data source (or mock/file)
        4. Start display engine (decides what to show based on session state)
        5. Run event loop: data changes → engine decides → sends commands
    """
    # config = load_config("config.json")
    # logger = Logger(config.log_level)
    # ws_client = WebSocketClient(config.esp32_ip, config.esp32_port)
    # f1_data = F1DataManager(config.data_source)
    # display = DisplayEngine(ws_client, config)
    
    # await ws_client.connect()
    # await f1_data.connect()
    
    # Run main loop
    # while True:
    #     new_data = await f1_data.poll()
    #     if new_data:
    #         command = display.process_update(new_data)
    #         if command:
    #             await ws_client.send(command)
    #     await asyncio.sleep(0.1)
    pass


if __name__ == "__main__":
    asyncio.run(main())
