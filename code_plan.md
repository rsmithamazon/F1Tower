# F1 Split Flap Tower — Code Architecture

## Three Codebases

```
F1 Tower/
├── host/                    ← Python (PC/Raspberry Pi) — the "brain"
├── firmware/                ← ESP32 (Arduino/PlatformIO) — display controller
└── row_controller/           ← Raspberry Pi Pico (Arduino/PlatformIO) — motor drivers
```

---

## 1. HOST (Python)

The host connects to F1 live timing, maintains race state, decides what the board should display, and sends commands to the ESP32 over WebSocket.

```
host/
├── main.py
├── config.py
├── models.py
├── f1_client.py
├── race_state.py
├── display_engine.py
├── renderers/
│   ├── __init__.py
│   ├── standings.py
│   ├── constructors.py
│   ├── single_driver.py
│   ├── flags.py
│   ├── ceremonies.py
│   └── base.py
├── board_protocol.py
└── ws_client.py
```

### main.py — Entry Point

```pseudo
function main():
    load_config()
    connect_to_esp32()           // WebSocket client → ESP32 server
    connect_to_f1_api()          // OpenF1 or timing stream
    
    loop:
        receive_f1_update()
        update_race_state()
        new_frame = display_engine.render_current_mode()
        if new_frame != last_sent_frame:
            send_to_esp32(new_frame)
        handle_events()          // flags, pits, overtakes → trigger transitions
```

### config.py — User Settings

```pseudo
CONFIG:
    esp32_ip: "192.168.1.x"
    esp32_port: 81
    f1_api_url: "..."
    
    display_mode: "standings_cycle"
    scroll_delay_ms: 5000
    drivers_to_show: 10
    points_mode: "alternate_per_page"
    
    transitions:
        page_scroll: "cascade_down"
        mode_change: "car_rainbow"
        flag_return: "in_place"
        startup: "rainbow"
        shutdown: "cascade_down"
        car_color: "team"
```

### models.py — Data Structures

```pseudo
class Driver:
    position: int
    name: str               // "NOR"
    team_id: str            // "mclaren"
    gap: str                // "+1.234"
    tire: str               // "soft"
    in_pit: bool
    sectors: [SectorTime]
    points: int

class SectorTime:
    value: str              // "28.3"
    status: str             // "personal_best" | "overall_best" | "slower"

class Team:
    id: str
    name: str               // "McLaren"
    abbreviation: str       // "MCL"
    color_id: str           // "orange"
    points: int

class BoardFrame:
    cells: [5][5] CellContent
    transition: str | None  // requested transition style
    priority: int           // flag=100, normal=1

class CellContent:
    type: str               // "number" | "letter" | "color" | "logo" | "icon" | "animation"
    value: str              // "5" | "N" | "red" | "ferrari" | "trophy" | "car_frame_01"
```

### f1_client.py — F1 Data Connection

```pseudo
class F1Client:
    function connect(url):
        // WebSocket or HTTP stream to F1 timing source
    
    function on_position_change(callback):
        // driver changed position
    
    function on_flag_change(callback):
        // flag status changed (none, yellow, red, etc.)
    
    function on_pit_event(callback):
        // driver entered/exited pit
    
    function on_sector_time(callback):
        // new sector time posted
    
    function on_session_change(callback):
        // practice → quali → race
    
    function get_current_standings() -> [Driver]:
        // full current state
```

### race_state.py — Live State Tracker

```pseudo
class RaceState:
    drivers: dict[str, Driver]      // keyed by driver ID
    teams: dict[str, Team]
    session_type: str               // "practice" | "qualifying" | "race"
    flag_status: str                // "none" | "yellow" | "red" | "sc" | "vsc"
    fastest_lap: Driver | None
    quali_round: int                // 1, 2, 3
    eliminated: [str]               // driver IDs eliminated this round
    
    function update_from_event(event):
        // apply incoming data to state
    
    function get_sorted_drivers() -> [Driver]:
        // by position
    
    function get_constructors_sorted() -> [Team]:
        // by points
    
    function detect_position_swap(old, new) -> [(driver_a, driver_b)]:
        // what changed since last update
    
    function detect_flag_change(old, new) -> FlagEvent | None:
        // did flag status change
```

### display_engine.py — Mode Manager

```pseudo
class DisplayEngine:
    current_mode: str
    current_page: int
    page_timer: Timer
    race_state: RaceState
    renderers: dict[str, BaseRenderer]
    
    function render_current_mode() -> BoardFrame:
        renderer = renderers[current_mode]
        return renderer.render(race_state, current_page)
    
    function handle_event(event) -> (BoardFrame, transition):
        // events override current display
        if event.type == "flag":
            return (flags_renderer.render(event), "instant")
        if event.type == "pit_in":
            return (modify_single_row(event.driver), None)
        if event.type == "fastest_lap":
            return (purple_flash(), "instant")
    
    function advance_page():
        current_page += 1
        if current_page >= total_pages:
            current_page = 0
    
    function change_mode(new_mode, transition):
        current_mode = new_mode
        current_page = 0
```

### renderers/base.py — Renderer Interface

```pseudo
class BaseRenderer:
    function render(state: RaceState, page: int) -> BoardFrame:
        raise NotImplemented
    
    function get_page_count(state: RaceState) -> int:
        raise NotImplemented
```

### renderers/standings.py — Driver/Points Rendering

```pseudo
class StandingsRenderer(BaseRenderer):
    function render(state, page) -> BoardFrame:
        drivers = state.get_sorted_drivers()
        page_drivers = drivers[page*5 : (page+1)*5]
        frame = BoardFrame()
        for row, driver in enumerate(page_drivers):
            frame.cells[row] = render_driver_row(driver)
        return frame
    
    function render_driver_row(driver) -> [CellContent]:
        return [
            CellContent("number", str(driver.position)),
            CellContent("logo", driver.team_id),
            CellContent("letter", driver.name[0]),
            CellContent("letter", driver.name[1]),
            CellContent("letter", driver.name[2])
        ]
    
    function render_points_row(driver) -> [CellContent]:
        pts = str(driver.points).rjust(3)
        return [
            CellContent("number", str(driver.position)),
            CellContent("logo", driver.team_id),
            CellContent("number", pts[0]),
            CellContent("number", pts[1]),
            CellContent("number", pts[2])
        ]
```

### renderers/flags.py — Flag States

```pseudo
class FlagRenderer(BaseRenderer):
    function render_full_color(color_id) -> BoardFrame:
        // all 25 cells = same color
        frame = BoardFrame()
        for row in 0..4:
            for col in 0..4:
                frame.cells[row][col] = CellContent("color", color_id)
        return frame
    
    function render_safety_car() -> BoardFrame:
        // yellow background + "SAFETY CAR" text
        frame = render_full_color("yellow")
        frame.cells[1][1] = CellContent("letter", "S")
        frame.cells[1][2] = CellContent("letter", "A")
        frame.cells[1][3] = CellContent("letter", "F")
        frame.cells[1][4] = CellContent("letter", "E")
        // ... etc
        return frame
    
    function render_checkered() -> BoardFrame:
        frame = BoardFrame()
        for row in 0..4:
            for col in 0..4:
                color = "black" if (row + col) % 2 == 0 else "white"
                frame.cells[row][col] = CellContent("color", color)
        return frame
```

### renderers/ceremonies.py — Race Start, Winner, etc.

```pseudo
class CeremonyRenderer(BaseRenderer):
    function render_lights_out(step: int) -> BoardFrame:
        // step 1-5: progressive red fill, step 6: all white
        frame = BoardFrame()
        if step <= 5:
            for row in 0..step-1:
                fill_row(frame, row, "red")
        else:
            fill_all(frame, "white")
        return frame
    
    function render_winner(winner: Driver, podium: [Driver]) -> BoardFrame:
        frame = BoardFrame()
        frame.cells[0] = [icon("trophy"), logo(winner.team_id), color(team_color), color(team_color), color(team_color)]
        frame.cells[1] = render_driver_row(winner)
        frame.cells[2] = [color("black"), color("black"), color("black"), color("black"), color("black")]
        frame.cells[3] = render_driver_row(podium[1])
        frame.cells[4] = render_driver_row(podium[2])
        return frame
```

### board_protocol.py — Command Serialization

```pseudo
// Defines the message format sent from host → ESP32

function encode_full_frame(frame: BoardFrame) -> dict:
    return {
        "cmd": "frame",
        "cells": [[cell.to_dict() for cell in row] for row in frame.cells],
        "transition": frame.transition,
        "priority": frame.priority
    }

function encode_single_row(row: int, cells: [CellContent], transition: str) -> dict:
    return {
        "cmd": "row",
        "row": row,
        "cells": [cell.to_dict() for cell in cells],
        "transition": transition
    }

function encode_single_cell(row: int, col: int, cell: CellContent) -> dict:
    return {
        "cmd": "cell",
        "row": row,
        "col": col,
        "cell": cell.to_dict()
    }

function encode_animation(animation_id: str, params: dict) -> dict:
    return {
        "cmd": "animate",
        "id": animation_id,      // "car_rainbow", "lights_out", etc.
        "params": params
    }

function encode_config_update(settings: dict) -> dict:
    return {
        "cmd": "config",
        "settings": settings
    }
```

### ws_client.py — WebSocket Connection to ESP32

```pseudo
class WebSocketClient:
    function connect(ip, port):
        // establish WebSocket connection to ESP32
    
    function send(message: dict):
        // JSON serialize and send
    
    function on_status(callback):
        // ESP32 sends back status (ready, busy, error)
    
    function reconnect():
        // auto-reconnect with backoff
    
    function is_connected() -> bool
```

---

## 2. FIRMWARE — ESP32 (Display Controller)

The ESP32 receives display commands over WiFi/WebSocket and orchestrates the physical display via I2C to Pico row controllers.

```
firmware/
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── config.h
│   ├── wifi_manager.h / .cpp
│   ├── ws_server.h / .cpp
│   ├── command_parser.h / .cpp
│   ├── board_state.h / .cpp
│   ├── flap_lookup.h / .cpp
│   ├── transition_engine.h / .cpp
│   ├── animation_player.h / .cpp
│   ├── scheduler.h / .cpp
│   └── i2c_master.h / .cpp
```

### main.cpp — Setup + Loop

```pseudo
function setup():
    init_serial_debug()
    init_i2c_bus()
    load_config()
    wifi_manager.connect()
    ws_server.start()
    board_state.init_all_black()
    home_all_modules()             // tell all Picos to home their motors

function loop():
    wifi_manager.check_connection()
    ws_server.poll()               // check for incoming messages
    scheduler.tick()               // advance timers
    transition_engine.tick()       // advance any running transition
    animation_player.tick()        // advance any running animation
```

### config.h — Hardware Constants

```pseudo
constants:
    I2C_SDA_PIN = 21
    I2C_SCL_PIN = 22
    I2C_CLOCK = 400000             // 400kHz fast mode
    
    NUM_ROWS = 5
    NUM_COLS = 5
    NUM_ROW_CONTROLLERS = 5
    MOTORS_PER_ROW = [5, 5, 5, 5, 5]   // 5 motors per Pico (one per row)
    
    ROW_ADDRESSES = [0x10, 0x11, 0x12, 0x13, 0x14]
    
    // Module-to-row mapping: module[row][col] → (row_addr, motor_index)
    MODULE_MAP = [
        [(0x10, 0), (0x10, 1), (0x10, 2), (0x10, 3), (0x10, 4)],  // row 0
        [(0x11, 0), (0x11, 1), (0x11, 2), (0x11, 3), (0x11, 4)],  // row 1
        [(0x12, 0), (0x12, 1), (0x12, 2), (0x12, 3)],             // row 2 (partial)
        ...
    ]
    
    WIFI_SSID = "..."
    WIFI_PASS = "..."
    WS_PORT = 81
```

### wifi_manager.h — WiFi Connection

```pseudo
class WiFiManager:
    function connect():
        // connect to configured SSID
        // retry with backoff on failure
    
    function check_connection():
        // if disconnected, attempt reconnect
    
    function get_ip() -> string
    
    function is_connected() -> bool
```

### ws_server.h — WebSocket Server

```pseudo
class WSServer:
    function start(port):
        // start WebSocket server on given port
    
    function poll():
        // check for new connections + incoming messages
        // on message → command_parser.parse(message)
    
    function send_status(status: string):
        // send status back to host ("ready", "busy", "transition_complete")
    
    function on_message(handler_callback):
        // register handler for incoming commands
```

### command_parser.h — Parse Incoming Commands

```pseudo
class CommandParser:
    function parse(json_string) -> Command:
        // deserialize JSON into command struct
    
    function execute(command: Command):
        switch command.type:
            case "frame":
                handle_full_frame(command.cells, command.transition, command.priority)
            case "row":
                handle_row_update(command.row, command.cells, command.transition)
            case "cell":
                handle_cell_update(command.row, command.col, command.cell)
            case "animate":
                handle_animation(command.id, command.params)
            case "config":
                handle_config(command.settings)
            case "home":
                home_all_modules()
            case "sleep":
                transition_to_black()
    
    function handle_full_frame(cells, transition, priority):
        target = board_state.cells_to_flap_positions(cells)
        if priority > current_priority OR not transition_engine.is_running():
            transition_engine.start(target, transition)
```

### board_state.h — Current Display State

```pseudo
class BoardState:
    current_positions: [5][5] int     // current flap position per module (0-44)
    target_positions: [5][5] int      // where we want to be
    
    function init_all_black():
        // set all to flap position 0 (black)
    
    function get_current(row, col) -> int:
        return current_positions[row][col]
    
    function set_current(row, col, position):
        current_positions[row][col] = position
    
    function cells_to_flap_positions(cells: [5][5] CellContent) -> [5][5] int:
        // use flap_lookup to convert content → flap position
        for row in 0..4:
            for col in 0..4:
                positions[row][col] = flap_lookup.resolve(col, cells[row][col])
        return positions
    
    function get_delta(target) -> [(row, col, from_pos, to_pos)]:
        // which modules need to move
        changes = []
        for row in 0..4:
            for col in 0..4:
                if current[row][col] != target[row][col]:
                    changes.append((row, col, current[row][col], target[row][col]))
        return changes
```

### flap_lookup.h — Content → Flap Position Mapping

```pseudo
// Built from flap_config.json at compile time (or stored in PROGMEM)

class FlapLookup:
    function resolve(col: int, content: CellContent) -> int:
        // given a column (0-4) and content, return the flap position
        drum_type = get_drum_type(col)   // col0, col1, or col2_3_4
        
        switch content.type:
            case "color":
                return color_to_position(drum_type, content.value)
            case "number":
                return number_to_position(content.value)   // same on all: pos 8-17
            case "letter":
                return letter_to_position(content.value)   // col2_3_4 only: pos 18-43
            case "logo":
                return logo_to_position(content.value)     // col1 only: pos 26-36
            case "icon":
                return icon_to_position(content.value)     // col0 only: pos 26-27
            case "animation":
                return animation_to_position(content.value) // col0 only: pos 30-44
            case "symbol":
                return symbol_to_position(drum_type, content.value)
    
    function get_drum_type(col) -> str:
        if col == 0: return "col0"
        if col == 1: return "col1"
        return "col2_3_4"
    
    function color_to_position(drum_type, color_id) -> int:
        // base colors: pos 0-7 on all drums
        // team colors: pos 18-22 on col0/col1 only
        return COLOR_MAP[color_id]
    
    function number_to_position(digit: char) -> int:
        return 8 + (digit - '0')    // pos 8-17
    
    function letter_to_position(letter: char) -> int:
        return 18 + (letter - 'A')  // pos 18-43
```

### transition_engine.h — Transition Orchestration

```pseudo
class TransitionEngine:
    is_running: bool
    current_transition: Transition
    step_index: int
    step_timer: unsigned long
    
    function start(target_positions: [5][5] int, style: string):
        is_running = true
        steps = compute_steps(target_positions, style)
        step_index = 0
        execute_step(steps[0])
    
    function tick():
        if not is_running: return
        if millis() - step_timer >= current_step.delay:
            step_index++
            if step_index >= steps.length:
                finish()
            else:
                execute_step(steps[step_index])
    
    function compute_steps(target, style) -> [TransitionStep]:
        delta = board_state.get_delta(target)
        switch style:
            case "instant":
                return [TransitionStep(all delta modules, delay=0)]
            case "cascade_down":
                return group_by_row(delta, delay_between=300ms)
            case "cascade_up":
                return group_by_row_reversed(delta, delay_between=300ms)
            case "sweep_lr":
                return group_by_col(delta, delay_between=200ms)
            case "in_place":
                return one_by_one(delta, delay_between=200ms)
            case "car_rainbow":
                return animation_player.build_car_rainbow_steps(target)
    
    function execute_step(step: TransitionStep):
        for (row, col, target_pos) in step.moves:
            i2c_master.move_module(row, col, target_pos)
            board_state.set_current(row, col, target_pos)
        step_timer = millis()
    
    function finish():
        is_running = false
        ws_server.send_status("ready")
    
    function abort():
        // stop mid-transition (for flag interrupts)
        is_running = false
    
    function is_interruptible() -> bool:
        // flags can interrupt, normal transitions cannot
        return current_transition.priority < FLAG_PRIORITY

struct TransitionStep:
    moves: [(row, col, target_pos)]
    delay: int                        // ms to wait after this step
```

### animation_player.h — Complex Multi-Step Animations

```pseudo
class AnimationPlayer:
    is_playing: bool
    frame_index: int
    frame_timer: unsigned long
    
    function play(animation_id: string, params: dict):
        switch animation_id:
            case "car_down":
                play_car_down(params.color)
            case "car_rainbow":
                play_car_rainbow()
            case "lights_out":
                play_lights_out()
            case "rainbow":
                play_rainbow()
    
    function tick():
        if not is_playing: return
        if millis() - frame_timer >= current_frame_delay:
            advance_frame()
    
    function play_car_down(fill_color):
        // car moves down col 0, each row fills with color behind it
        frames = []
        for row in 0..4:
            frames.append(CarFrame(row, fill_color))
        load_frames(frames, delay=400ms)
    
    function play_car_rainbow():
        // car descends col 0
        // each row fills L→R with rainbow colors after car passes
        // complex: multiple cells per frame
        frames = build_car_rainbow_frames()
        load_frames(frames, delay=150ms)
    
    function play_lights_out():
        // 5 steps: row by row goes red
        // step 6: all white
        // then resolve to target
        frames = []
        for step in 1..5:
            frames.append(LightsFrame(rows_lit=step, color="red", delay=800ms))
        frames.append(LightsFrame(rows_lit=5, color="white", delay=0))  // GO!
        load_frames(frames)
    
    function build_car_rainbow_frames() -> [AnimFrame]:
        // row 0: car appears → rainbow fills behind → car to row 1
        // row 1: car appears → rainbow fills behind → car to row 2
        // ...
        colors = ["red", "orange", "yellow", "green", "blue", "purple", "white"]
        frames = []
        for row in 0..4:
            // car animation frames on col 0
            for car_frame in 30..34:  // first 5 animation positions
                frames.append({col0: car_frame, cols1-4: rainbow_step})
        return frames
```

### scheduler.h — Timing and Delays

```pseudo
class Scheduler:
    timers: [ScheduledTask]
    
    function tick():
        for task in timers:
            if task.is_due():
                task.execute()
                if task.repeating:
                    task.reschedule()
                else:
                    remove(task)
    
    function schedule_once(delay_ms, callback):
        timers.append(ScheduledTask(millis() + delay_ms, callback, repeating=false))
    
    function schedule_repeating(interval_ms, callback) -> task_id:
        timers.append(ScheduledTask(millis() + interval_ms, callback, repeating=true))
        return task.id
    
    function cancel(task_id):
        remove task by id
```

### i2c_master.h — I2C Communication to Slaves

```pseudo
class I2CMaster:
    function init(sda, scl, clock_speed):
        Wire.begin(sda, scl)
        Wire.setClock(clock_speed)
    
    function move_module(row: int, col: int, target_position: int):
        (slave_addr, motor_index) = MODULE_MAP[row][col]
        send_command(slave_addr, CMD_MOVE, motor_index, target_position)
    
    function home_module(row: int, col: int):
        (slave_addr, motor_index) = MODULE_MAP[row][col]
        send_command(slave_addr, CMD_HOME, motor_index, 0)
    
    function home_all():
        for addr in SLAVE_ADDRESSES:
            send_command(addr, CMD_HOME_ALL, 0, 0)
    
    function send_command(addr: byte, cmd: byte, motor: byte, value: byte):
        Wire.beginTransmission(addr)
        Wire.write(cmd)
        Wire.write(motor)
        Wire.write(value)
        Wire.endTransmission()
    
    function request_status(addr) -> byte:
        Wire.requestFrom(addr, 1)
        return Wire.read()

// I2C Command bytes
CMD_MOVE = 0x01        // move motor X to position Y
CMD_HOME = 0x02        // home motor X
CMD_HOME_ALL = 0x03    // home all motors on this slave
CMD_STOP = 0x04        // emergency stop
CMD_STATUS = 0x05      // request status byte
```

---

## 3. SLAVE — Raspberry Pi Pico (Motor Driver)

Each Pico drives 5 stepper motors (one full row). It listens for I2C commands and moves motors using PIO for hardware-precise timing.

```
slave/
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── config.h
│   ├── i2c_handler.h / .cpp
│   ├── stepper_driver.h / .cpp
│   └── homing.h / .cpp
```

### main.cpp

```pseudo
function setup():
    set_i2c_address(MY_ADDRESS)      // set via jumper/solder bridge
    init_i2c_slave(on_receive, on_request)
    init_motors()
    init_hall_sensors()

function loop():
    for each motor:
        if motor.has_target():
            motor.step_toward_target()
    // motors run concurrently (round-robin stepping)
```

### config.h

```pseudo
constants:
    MY_ADDRESS = 0x10              // unique per slave (set by jumper)
    NUM_MOTORS = 5                 // how many this slave controls
    FLAPS_PER_DRUM = 45
    STEPS_PER_FLAP = 64           // 28BYJ-48 steps between flaps (2048 steps / 45 ≈ 45.5)
    
    MOTOR_PINS = [
        [2, 3, 4, 5],             // motor 0: IN1-IN4
        [6, 7, 8, 9],             // motor 1
        [10, 11, 12, 13],         // motor 2
        [A0, A1, A2, A3],         // motor 3
        [A4, A5, ?, ?]            // motor 4 (if applicable)
    ]
    
    HALL_PINS = [?, ?, ?, ?, ?]   // one per motor, for homing
```

### i2c_handler.h

```pseudo
function on_receive(num_bytes):
    cmd = Wire.read()
    motor_index = Wire.read()
    value = Wire.read()
    
    switch cmd:
        case CMD_MOVE:
            motors[motor_index].set_target(value)
        case CMD_HOME:
            motors[motor_index].start_homing()
        case CMD_HOME_ALL:
            for motor in motors:
                motor.start_homing()
        case CMD_STOP:
            for motor in motors:
                motor.stop()

function on_request():
    // master asking for status
    status = compute_status_byte()   // bit per motor: 0=idle, 1=moving
    Wire.write(status)
```

### stepper_driver.h

```pseudo
class StepperDriver:
    current_position: int          // 0-44 (which flap is showing)
    target_position: int
    step_count: int                // 0-2047 (absolute step within drum)
    is_moving: bool
    
    function set_target(flap_position: int):
        target_position = flap_position
        is_moving = true
    
    function step_toward_target():
        if current_position == target_position:
            is_moving = false
            power_off_coils()      // save power when idle
            return
        
        // always step FORWARD (split-flap only goes one direction)
        do_single_step()
        step_count++
        
        if step_count >= STEPS_PER_FLAP * (next_flap_offset()):
            current_position = (current_position + 1) % FLAPS_PER_DRUM
    
    function do_single_step():
        // half-step sequence for 28BYJ-48
        advance_phase()
        write_coils(PHASE_TABLE[current_phase])
        delay_microseconds(STEP_DELAY_US)
    
    function power_off_coils():
        // all pins LOW to prevent heating
    
    function get_steps_remaining() -> int:
        // calculate forward distance (always forward on split-flap)
        if target_position >= current_position:
            flaps_to_go = target_position - current_position
        else:
            flaps_to_go = (FLAPS_PER_DRUM - current_position) + target_position
        return flaps_to_go * STEPS_PER_FLAP
```

### homing.h

```pseudo
class HomingController:
    function start_homing(motor: StepperDriver):
        // step forward until hall sensor triggers
        // hall sensor = magnet on one specific flap (flap 0 = black)
        while not hall_triggered(motor.hall_pin):
            motor.do_single_step()
            motor.step_count++
            if motor.step_count > MAX_STEPS_FULL_REV:
                // error: hall never triggered — hardware problem
                flag_error(motor)
                return
        
        // found home
        motor.current_position = 0
        motor.step_count = 0
        motor.target_position = 0
    
    function hall_triggered(pin) -> bool:
        return digitalRead(pin) == LOW   // hall sensors are active-low typically
```

---

## MESSAGE PROTOCOL (Host ↔ ESP32)

### Host → ESP32 (commands)

```json
// Full frame update
{"cmd": "frame", "cells": [[...]], "transition": "cascade_down", "priority": 1}

// Single row update (pit stop, position change)
{"cmd": "row", "row": 2, "cells": [...], "transition": null}

// Single cell update (tire color flash)
{"cmd": "cell", "row": 1, "col": 0, "cell": {"type": "color", "value": "red"}}

// Trigger animation (car down, lights out, etc.)
{"cmd": "animate", "id": "car_rainbow", "params": {"then": "frame", "target": [[...]]}}

// System commands
{"cmd": "home"}
{"cmd": "sleep"}
{"cmd": "wake"}
{"cmd": "config", "settings": {"transition_speed": "fast"}}
```

### ESP32 → Host (status)

```json
{"status": "ready"}
{"status": "busy", "transition": "cascade_down", "progress": 60}
{"status": "error", "message": "slave 0x12 not responding"}
{"status": "homing", "progress": 80}
```

---

## KEY DESIGN DECISIONS

### 1. ESP32 Dual Core Usage
- **Core 0**: WiFi stack + WebSocket (runs automatically)
- **Core 1**: Main loop — transition engine, animation, I2C commands

### 2. Interrupt Priority
- Flags (red/yellow) override ANYTHING — abort current transition, instant flood
- Pit/tire events are single-cell, non-blocking
- Page scrolls are lowest priority, interruptible

### 3. Forward-Only Drums
Split-flap drums only spin forward. To go from flap 40 to flap 5, you go forward through 41→42→43→44→0→1→2→3→4→5 (10 flaps). The transition engine does NOT need to worry about direction — always forward.

### 4. Motor De-energize
Stepper coils are powered off when idle to prevent heat buildup. 25 motors × 4 coils = 100 coil-pairs. All off when not moving.

### 5. Homing on Startup Only
Motors home once at power-on. After that, position is tracked in software. If a motor loses steps (jam), it drifts permanently until next home cycle.
