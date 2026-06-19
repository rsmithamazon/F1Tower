# ===========================================================
# WS SERVER — WebSocket server mimicking ESP32
# ===========================================================
# Accepts connections from the host application (or manual test
# scripts) on the same port/protocol the real ESP32 would use.
# Routes commands to the board state.
# ===========================================================

import asyncio
import json
import websockets


class SimulatorWSServer:
    """
    WebSocket server that mimics ESP32 behavior.
    Accepts display commands, returns status.
    """

    def __init__(self, board_state, port=81):
        self.board = board_state
        self.port = port
        self.server = None
        self.clients = set()
        self.command_queue = asyncio.Queue()

    async def start(self):
        """Start the WebSocket server."""
        self.server = await websockets.serve(
            self._handle_client,
            "0.0.0.0",
            self.port
        )
        print(f"[SIM] WebSocket server listening on ws://localhost:{self.port}")

    async def _handle_client(self, websocket):
        """Handle a single client connection."""
        self.clients.add(websocket)
        print(f"[SIM] Host connected from {websocket.remote_address}")

        try:
            async for message in websocket:
                await self._process_message(message, websocket)
        except websockets.exceptions.ConnectionClosed:
            print("[SIM] Host disconnected")
        finally:
            self.clients.discard(websocket)

    async def _process_message(self, message, websocket):
        """Parse and route incoming command."""
        try:
            command = json.loads(message)
        except json.JSONDecodeError:
            await websocket.send(json.dumps({
                "event": "error", "code": "ESP_400", "msg": "Invalid JSON"
            }))
            return

        cmd_type = command.get("cmd", "")
        print(f"[SIM] Received: {cmd_type} | {json.dumps(command, indent=None)[:80]}")

        if cmd_type == "display":
            # Queue for the main loop to process
            await self.command_queue.put(command)
            await websocket.send(json.dumps({"event": "status", "status": "ok"}))

        elif cmd_type == "heartbeat":
            await websocket.send(json.dumps({"event": "heartbeat_ack"}))

        elif cmd_type == "query_status":
            status = self._build_status()
            await websocket.send(json.dumps(status))

        elif cmd_type == "startup":
            await self.command_queue.put({"cmd": "startup"})
            await websocket.send(json.dumps({"event": "status", "status": "ok"}))

        elif cmd_type == "shutdown":
            await self.command_queue.put({"cmd": "shutdown"})
            await websocket.send(json.dumps({"event": "status", "status": "ok"}))

        else:
            await websocket.send(json.dumps({
                "event": "error", "code": "ESP_401", "msg": f"Unknown cmd: {cmd_type}"
            }))

    def _build_status(self):
        """Build board status response."""
        return {
            "event": "status",
            "status": "idle" if not self.board.transition_active else "transitioning",
            "transition": self.board.transition_type if self.board.transition_active else None
        }

    def get_pending_command(self):
        """Non-blocking check for queued commands (called from game loop)."""
        try:
            return self.command_queue.get_nowait()
        except asyncio.QueueEmpty:
            return None
