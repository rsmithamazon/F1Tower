# Split Flap F1 — Power & Wiring (12V Single Supply)

## Power Architecture

One 12V PSU powers everything. A buck converter steps down to 5V for logic.

```
[12V 5A PSU] ──┬──→ [12V BUS] ──→ 25× ULN2003 motor power
               │
               └──→ [Buck Converter 12V→5V] ──→ [5V BUS] ──→ ESP32 + Picos + Hall sensors

ALL GROUNDS TIED TOGETHER
```

---

## Wiring Diagram

```
[Wall Outlet]
      │
[12V 5A DC PSU]
      │         │
   +12V OUT   GND OUT
      │         │
══════╪═════════╪══════════════════════════════════════════
 12V  │     GND │ (COMMON GROUND BUS)
 BUS  │     BUS │
      │         │
      ├─────────┼──→ ULN2003 #1  VCC (+) / GND (-)
      ├─────────┼──→ ULN2003 #2  VCC (+) / GND (-)
      ├─────────┼──→ ...
      ├─────────┼──→ ULN2003 #25 VCC (+) / GND (-)
      │         │
      │    ┌────┤
      │    │    │
[Buck Converter]│
  IN+ IN-  │    │
  OUT+ OUT- │    │
      │    │    │
   +5V OUT │    │
      │    GND (same bus)
      │         │
══════╪═════════╪══════════════════════════════════════════
  5V  │     GND │
  BUS │     BUS │ (same ground as 12V)
      │         │
      ├─────────┼──→ ESP32 VIN / GND
      ├─────────┼──→ Pico 1 VSYS (pin 39) / GND (pin 38)
      ├─────────┼──→ Pico 2 VSYS / GND
      ├─────────┼──→ Pico 3 VSYS / GND
      ├─────────┼──→ Pico 4 VSYS / GND
      └─────────┼──→ Pico 5 VSYS / GND
                │
════════════════╪══════════════════════════════════════════
```

---

## Per Row Wiring (×5 identical)

```
12V BUS ──┬──→ ULN2003 #1 VCC (motor power +)
           ├──→ ULN2003 #2 VCC
           ├──→ ULN2003 #3 VCC
           ├──→ ULN2003 #4 VCC
           └──→ ULN2003 #5 VCC

5V BUS  ──────→ Pico VSYS (pin 39)

GND BUS ──┬──→ Pico GND (pin 38)
           ├──→ ULN2003 #1 GND
           ├──→ ULN2003 #2 GND
           ├──→ ULN2003 #3 GND
           ├──→ ULN2003 #4 GND
           └──→ ULN2003 #5 GND
```

---

## I2C Bus (ESP32 → All Picos)

```
ESP32 GPIO21 (SDA) ──┬── Pico1 GP0 ── Pico2 GP0 ── Pico3 GP0 ── Pico4 GP0 ── Pico5 GP0
                     │
                [4.7KΩ pull-up to 3.3V]

ESP32 GPIO22 (SCL) ──┬── Pico1 GP1 ── Pico2 GP1 ── Pico3 GP1 ── Pico4 GP1 ── Pico5 GP1
                     │
                [4.7KΩ pull-up to 3.3V]
```

I2C Addresses:
- Pico 1 (Row 1): 0x10
- Pico 2 (Row 2): 0x11
- Pico 3 (Row 3): 0x12
- Pico 4 (Row 4): 0x13
- Pico 5 (Row 5): 0x14

---

## Hall Sensors (per Pico)

```
Pico 3V3 OUT ──┬──→ Hall 1 VCC
               ├──→ Hall 2 VCC
               ├──→ Hall 3 VCC
               ├──→ Hall 4 VCC
               └──→ Hall 5 VCC

Hall 1 OUT ──→ Pico GP2
Hall 2 OUT ──→ Pico GP3
Hall 3 OUT ──→ Pico GP4
Hall 4 OUT ──→ Pico GP5
Hall 5 OUT ──→ Pico GP6

Hall GND ──→ GND BUS
```

---

## Motor Pins (per Pico, 5 motors × 4 pins = 20 GPIO)

```
Motor 1: GP7,  GP8,  GP9,  GP10  → ULN2003 #1 IN1–IN4
Motor 2: GP11, GP12, GP13, GP14  → ULN2003 #2 IN1–IN4
Motor 3: GP15, GP16, GP17, GP18  → ULN2003 #3 IN1–IN4
Motor 4: GP19, GP20, GP21, GP22  → ULN2003 #4 IN1–IN4
Motor 5: GP26, GP27, GP28, GP22  → ULN2003 #5 IN1–IN4
```

---

## Power Budget

### 12V Rail (motors)
- 28BYJ-48 12V (~100Ω): ~120mA per motor moving
- Worst case (all 25): 25 × 120mA = 3.0A
- **PSU provides 5A** — plenty of headroom

### 5V Rail (logic, via buck converter)
- ESP32: ~250mA
- Pi Pico × 5: ~500mA
- Hall sensors × 25: ~250mA
- Total: ~1.0A
- **Buck converter rated 3-5A** — plenty

### Total from PSU
- Motors: 3.0A × 12V = 36W
- Logic: 1.0A × 12V input (buck is ~90% efficient) ≈ 6W
- Total: ~42W peak
- **12V 5A = 60W** — covered

---

## FULL PARTS LIST

### Electronics

| # | Part | Qty | Cost | Search |
|---|------|-----|------|--------|
| 1 | ESP32 dev board | 1 | $5 | "ESP32 dev board USB-C" |
| 2 | Raspberry Pi Pico | 5 | $20 | "Raspberry Pi Pico" |
| 3 | **28BYJ-48 12V + ULN2003 combo** | 25 | $50 | AliExpress SeasBoat Store "12V", or Bezek Labs Etsy |
| 4 | Hall effect sensor (A3144) | 25 | $8 | "A3144 hall effect sensor 10 pack" (×3) |
| 5 | Neodymium magnets (3×1mm) | 25 | $3 | "3x1mm neodymium magnet 50 pack" |
| 6 | 4.7KΩ resistors | 2 | $0.10 | "4.7K ohm resistor" |

### Power (single supply)

| # | Part | Qty | Cost | Search |
|---|------|-----|------|--------|
| 7 | **12V 5A DC power supply** | 1 | $12 | "12V 5A power supply barrel jack" |
| 8 | **Barrel jack to screw terminal** | 1 | $2 | "DC 5.5x2.1 barrel jack screw terminal adapter" |
| 9 | **12V to 5V buck converter** | 1 | $4 | "LM2596 buck converter module" or "12V to 5V 3A step down" |

### Wire & Connectors

| # | Part | Qty | Cost | Search |
|---|------|-----|------|--------|
| 10 | 18 AWG silicone wire (red + black) | 2m each | $4 | "18 AWG silicone wire red black" |
| 11 | 22 AWG hookup wire (assorted) | 5m | $3 | "22 AWG solid hookup wire kit" |
| 12 | Dupont jumper wires (F-F) | 40 pack | $3 | "dupont jumper wires female to female" |

### Mechanical

| # | Part | Qty | Cost |
|---|------|-----|------|
| 13 | 3D printed frames + drums | 25 sets | ~$30 filament |
| 14 | Flap material (cardstock/PVC) | 1,125 flaps | $20 |

### Total: ~$164

---

## Physical Hookup (step by step)

```
1. PSU barrel jack → barrel-to-screw adapter
2. Screw terminal +12V → 12V bus wire (18 AWG red)
3. Screw terminal GND → GND bus wire (18 AWG black)
4. 12V bus → all 25 ULN2003 VCC pins
5. GND bus → all ULN2003 GND + buck converter IN-
6. 12V bus → buck converter IN+
7. Buck converter OUT+ → 5V bus wire (18 AWG red, separate run)
8. Buck converter OUT- → GND bus (same ground)
9. 5V bus → ESP32 VIN + all 5 Pico VSYS
10. GND bus → ESP32 GND + all 5 Pico GND
```

---

## Summary

```
[Wall] → [12V 5A PSU] → [barrel jack adapter]
                              │
                    ┌─────────┴─────────┐
                    │                   │
               [12V BUS]          [Buck 12→5V]
                    │                   │
            25× ULN2003           [5V BUS]
            (motor power)              │
                    │            ESP32 + 5 Picos
                    │                   │
                    └───── [COMMON GND] ─┘
```

# Split Flap F1 — Order Now (Committed Parts)

## Order List

| # | Part | Qty | ~Cost | Search |
|---|------|-----|-------|---------------|
| 1 | Raspberry Pi Pico | 5 | $20 | [Amazon](https://www.amazon.com/s?k=Raspberry+Pi+Pico) |
| 2 | Pico screw terminal breakout board | 5 | $25 | [Amazon](https://www.amazon.com/s?k=Raspberry+Pi+Pico+screw+terminal+breakout+board) |
| 3 | ESP32 dev board (USB-C) | 1 | $5 | [Amazon](https://www.amazon.com/s?k=ESP32+dev+board+USB-C) |
| 4 | ESP32 screw terminal breakout board | 1 | $8 | [Amazon](https://www.amazon.com/s?k=ESP32+screw+terminal+breakout+board) |
| 5 | 28BYJ-48 **12V** + ULN2003 combo | 30 | $60 | [AliExpress SeasBoat "12V"](https://www.aliexpress.us/item/2255800976774653.html) — 6× qty 5 |
| 6 | A3144 Hall effect sensor | 30 | $8 | [Amazon](https://www.amazon.com/s?k=A3144+hall+effect+sensor) |
| 7 | 12V 5A DC power supply (barrel jack) | 1 | $12 | [Amazon](https://www.amazon.com/s?k=12V+5A+DC+power+supply+barrel+jack) |
| 8 | Barrel jack to screw terminal adapter | 1 | $2 | [Amazon](https://www.amazon.com/s?k=5.5x2.1+DC+barrel+jack+screw+terminal+adapter) |
| 9 | LM2596 buck converter (12V→5V) | 1 | $4 | [Amazon](https://www.amazon.com/s?k=LM2596+buck+converter+module) |
| 10 | Digital multimeter | 1 | $15 | [Amazon](https://www.amazon.com/s?k=digital+multimeter) |
| 11 | 18 AWG silicone wire (red + black) | 2m each | $4 | [Amazon](https://www.amazon.com/s?k=18+AWG+silicone+wire+red+black) |
| 12 | 22 AWG solid hookup wire (assorted) | 5m | $3 | [Amazon](https://www.amazon.com/s?k=22+AWG+solid+hookup+wire+kit) |
| 13 | 4.7KΩ resistors | 10 pack | $1 | [Amazon](https://www.amazon.com/s?k=4.7K+ohm+resistor) |

## Optional (decide later)

| # | Part | Qty | ~Cost | Notes |
|---|------|-----|-------|-------|
| 14 | Neodymium magnets 3×1mm | 50 pack | $3 | For hall sensor — may already have |
| 15 | Wago 221 lever connectors (5-port) | 10 pack | $8 | Cleaner bus splits |

---

## Total: ~$167

## Not ordering yet (decide after testing)

- Flap material (cardstock vs PVC)
- Extra motors beyond 30
- Final frame filament

---

## Google Searches (copy-paste)

```
Raspberry Pi Pico
Raspberry Pi Pico screw terminal breakout board
ESP32 dev board USB-C
ESP32 screw terminal breakout board
28BYJ-48 12V ULN2003 5 pack
A3144 hall effect sensor 10 pack
12V 5A DC power supply barrel jack
DC 5.5x2.1 barrel jack screw terminal adapter
LM2596 buck converter module
digital multimeter
18 AWG silicone wire red black
22 AWG solid hookup wire assorted
4.7K ohm resistor pack
```
