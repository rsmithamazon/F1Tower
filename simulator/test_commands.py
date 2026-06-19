# ===========================================================
# TEST COMMANDS — Send test display commands to the simulator
# ===========================================================
# Run this while the simulator is running to test commands.
#
# Usage:
#   python test_commands.py
#
# Or import and call functions from your own test scripts.
# ===========================================================

import asyncio
import json
import websockets
import time

WS_URL = "ws://localhost:81"


async def send_command(command):
    """Send a single command to the simulator."""
    async with websockets.connect(WS_URL) as ws:
        await ws.send(json.dumps(command))
        response = await ws.recv()
        print(f"Response: {response}")
        return json.loads(response)


async def test_standings():
    """Test: Show driver standings with cascade transition."""
    cmd = {
        "cmd": "display",
        "mode": "standings",
        "endState": "driver_names",
        "transition": "cascade_down",
        "data": {
            "rows": [
                {"pos": "1", "team": "mclaren",     "text": "NOR"},
                {"pos": "2", "team": "ferrari",     "text": "LEC"},
                {"pos": "3", "team": "redbull",     "text": "VER"},
                {"pos": "4", "team": "mercedes",    "text": "HAM"},
                {"pos": "5", "team": "williams",    "text": "ALB"},
            ]
        }
    }
    await send_command(cmd)


async def test_red_flag():
    """Test: All red (red flag)."""
    cmd = {
        "cmd": "display",
        "mode": "flag",
        "endState": "all_red",
        "transition": "instant",
        "data": {
            "rows": [
                {"pos": "red", "team": "red", "text": "   "},
                {"pos": "red", "team": "red", "text": "   "},
                {"pos": "red", "team": "red", "text": "   "},
                {"pos": "red", "team": "red", "text": "   "},
                {"pos": "red", "team": "red", "text": "   "},
            ]
        }
    }
    await send_command(cmd)


async def test_checkered():
    """Test: Checkered flag pattern."""
    rows = []
    for r in range(5):
        # Alternate black/white per cell
        pos_color = "black" if r % 2 == 0 else "white"
        team_color = "white" if r % 2 == 0 else "black"
        c2 = "black" if r % 2 == 0 else "white"
        c3 = "white" if r % 2 == 0 else "black"
        c4 = "black" if r % 2 == 0 else "white"
        rows.append({"pos": pos_color, "team": team_color, "text": "   "})

    cmd = {
        "cmd": "display",
        "mode": "flag",
        "endState": "checkered",
        "transition": "instant",
        "data": {"rows": rows}
    }
    await send_command(cmd)


async def test_constructors():
    """Test: Constructor standings."""
    cmd = {
        "cmd": "display",
        "mode": "constructors",
        "endState": "constructor_names",
        "transition": "sweep_lr",
        "data": {
            "rows": [
                {"pos": "1", "team": "mclaren",     "text": "MCL"},
                {"pos": "2", "team": "ferrari",     "text": "FER"},
                {"pos": "3", "team": "redbull",     "text": "RBR"},
                {"pos": "4", "team": "mercedes",    "text": "MER"},
                {"pos": "5", "team": "astonmartin", "text": "AMR"},
            ]
        }
    }
    await send_command(cmd)


async def test_pit_stop():
    """Test: Single row update (driver pits)."""
    cmd = {
        "cmd": "display",
        "mode": "standings",
        "endState": "driver_names",
        "transition": "none",
        "data": {
            "row": 2,
            "update": {"pos": "3", "team": "redbull", "text": "PIT"}
        }
    }
    await send_command(cmd)


async def test_sequence():
    """Run a full test sequence with delays between commands."""
    print("=== Starting test sequence ===\n")

    print("1. Standings (cascade down)")
    await test_standings()
    await asyncio.sleep(3)

    print("\n2. Red flag (instant)")
    await test_red_flag()
    await asyncio.sleep(2)

    print("\n3. Standings again (cascade down)")
    await test_standings()
    await asyncio.sleep(3)

    print("\n4. Pit stop row 2 (instant)")
    await test_pit_stop()
    await asyncio.sleep(2)

    print("\n5. Constructors (sweep L→R)")
    await test_constructors()
    await asyncio.sleep(3)

    print("\n6. Checkered flag")
    await test_checkered()

    print("\n=== Test sequence complete ===")


if __name__ == "__main__":
    print("F1 Split Flap Simulator — Test Commands")
    print("Make sure the simulator (main.py) is running first!\n")
    print("Options:")
    print("  1. Standings")
    print("  2. Red flag")
    print("  3. Checkered")
    print("  4. Constructors")
    print("  5. Pit stop")
    print("  6. Full sequence")
    print()

    choice = input("Pick (1-6): ").strip()

    if choice == "1":
        asyncio.run(test_standings())
    elif choice == "2":
        asyncio.run(test_red_flag())
    elif choice == "3":
        asyncio.run(test_checkered())
    elif choice == "4":
        asyncio.run(test_constructors())
    elif choice == "5":
        asyncio.run(test_pit_stop())
    elif choice == "6":
        asyncio.run(test_sequence())
    else:
        print("Invalid choice")
