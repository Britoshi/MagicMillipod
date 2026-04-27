# MagicMillipod

An interactive proximity-triggered exhibit controller built on a Raspberry Pi Pico (RP2040). A hand sensor triggers a timed audio/visual presentation sequence. Room-exit detection — via a calibrated distance sensor — resets the system automatically when a visitor leaves.

---

## Project Overview

MagicMillipod is a museum exhibit controller. A visitor waves their hand in front of the sensor to start the show. The system plays through an introduction and a full presentation with synchronized audio, UV lighting, and NeoPixel effects. If the visitor leaves mid-presentation, the system detects this and resets to idle. After a full presentation, it waits for the visitor to leave before resetting.

**Key features:**
- Proximity-triggered state machine (no buttons needed for visitors)
- Calibrated room-exit detection using a rolling 3-second distance average
- UV light ramp with PWM brightness control
- Three NeoPixel rings with per-state effects
- Staff calibration and reset via a single physical button
- Simple Mode (DIP switch) for venues where calibration is impractical

---

## System Architecture

The firmware runs on the Arduino framework for RP2040. All hardware subsystems are **singletons** initialized once in `setup()` and accessed globally via `::Instance()`. A **finite state machine** drives all behavior — every state is responsible for its own enter/exit logic, per-frame update, and transition conditions.

```
┌──────────────────────────────────────────────────────────┐
│                        Main Loop                         │
│   TimeManager → DistanceSensorManager → StateMachine     │
└──────────────────────────────────────────────────────────┘
                            │
                ┌───────────▼───────────┐
                │      StateMachine     │
                │  currentState->Update()│
                └───────────────────────┘
                            │
       ┌──────────┬──────────┼───────────┬──────────────┐
       ▼          ▼          ▼           ▼              ▼
   IdleState  Introduction Presentation ExitRoom  Calibration
```

**State transitions:**
```
                    ┌──── person left (3s avg) ────┐
                    │                              ▼
[Idle] ──(hand < 30cm)──► [Introduction] ──(51s)──► [Presentation]
                                │                        │
                    person left │              person left│
                    (3s avg)    │              (3s avg)   │
                                ▼                        │
                             [Idle]          ◄───(3min)──┘
                                                  │
                                            [ExitRoom]
                                     (10s AND person gone)
                                                  │
                                               [Idle]

Button short press (any state) ──► [Calibration] ──(10s)──► [Idle]
Button long press  (any state) ──► [Idle]
```

---

## Hardware

| Component | Description |
|---|---|
| Raspberry Pi Pico (RP2040) | Microcontroller |
| HC-SR04 | Ultrasonic distance sensor (hand trigger + room exit detection) |
| Adafruit Audio FX Sound Board (16MB) | Two-track audio, GPIO trigger mode |
| TPA3116D2 2×50W Amplifier | Amplifies audio signal to drive speaker |
| Dayton Audio PC105-4 4" Speaker | Full-range audio output |
| NeoPixel Ring ×3 (7 LED each) | Front ring, back ring, sensor box indicator |
| UV light + N-channel MOSFET | UV LED switched via PWM |
| Momentary push button | Staff reset / calibration trigger |
| DIP switch ×4 | Feature configuration (Switch 0 = Simple Mode) |

### Pin Map

| GPIO | Assignment |
|---|---|
| GP2 | SFX Trigger 1 |
| GP4 | SFX Trigger 0 |
| GP5 | SFX Reset |
| GP6 | DIP Switch 0 (Simple Mode) |
| GP7 | DIP Switch 1 |
| GP8 | DIP Switch 2 |
| GP9 | DIP Switch 3 |
| GP12 | HC-SR04 TRIG |
| GP13 | HC-SR04 ECHO |
| GP15 | Reset / Calibration button (active LOW, internal pull-up) |
| GP16 | NeoPixel Ring 3 — sensor box indicator |
| GP17 | UV light PWM (N-channel MOSFET gate) |
| GP18 | NeoPixel Ring 1 — front |
| GP19 | NeoPixel Ring 2 — back |

---

## Key Components

### `StateMachine` / `State` / `StateFactory`
The core FSM. `State` is an abstract base with `OnStateEnter`, `OnStateUpdate`, `OnStateExit`, `CheckSwitchState`, and `InitializeSubState`. `StateFactory` owns all concrete state instances. `StateMachine` holds the active state pointer and calls `Update()` each loop. `ResetToIdle()` and `EnterCalibration()` force transitions from anywhere.

### `IdleState`
Waits for a hand within 30 cm. Ring 1 solid blue, Ring 2 rainbow. Transitions to Introduction on trigger. The 30 cm threshold is fixed and independent of calibration.

### `IntroductionState`
Plays audio track 0. Ring 1 red, Ring 2 rainbow fading out over the last 10 seconds. Runs 51 seconds then transitions to Presentation. Resets to Idle early if the 3-second average distance indicates the visitor has left.

### `PresentationState`
Plays audio track 1. Ring 1 red, Ring 2 off. UV ramps from 0 to full over the first 80 seconds, then fades back to 0 for the remainder of its 3-minute duration. Transitions to ExitRoom on completion. Resets to Idle early if the 3-second average distance indicates the visitor has left.

### `ExitRoomState`
Waits for **both** 10 seconds to elapse **and** the sensor to confirm the visitor has left. Uses `CalibrationData` in normal mode, or a fixed 50 cm threshold in Simple Mode. Transitions to Idle when both conditions are met.

### `CalibrationState`
Samples the distance sensor for 10 seconds with the room empty. Computes the average (door distance) and half the spread (error range), then stores both to `CalibrationData`. All rings yellow during calibration. Returns to Idle on completion.

### `CalibrationData`
Singleton storing `doorDistance`, `errorRange`, and `isCalibrated`. Provides `IsPersonPresent(dist)` and `HasPersonLeft(dist)`. If not yet calibrated, falls back to a 30 cm hardcoded threshold. The 1.5× error multiplier provides a grace margin around the calibrated door distance.

### `DistanceSensorManager`
Polls HC-SR04 at 20 Hz. Maintains two rolling averages:
- `GetAverageDistance()` — 4-sample (~200ms), used by IdleState for fast hand detection
- `GetLongAverageDistance()` — 60-sample (3s), used by Introduction/Presentation/ExitRoom for stable exit detection

### `NeoPixelController`
Wraps three `Adafruit_NeoPixel` instances. Ring 1 (front, GP18) and Ring 3 (sensor box, GP16) are controlled with `SetAll` / `Off`. Ring 2 (back, GP19) has `Ring2Rainbow(brightness)` for a non-blocking time-based rainbow with fade support, and `Ring2Off()`.

### `UVLightController`
PWM brightness control via N-channel MOSFET. `SetBrightness(float)` accepts 0.0–1.0. PWM at 1 kHz.

### `AdafruitAudio`
Controls the Adafruit Sound FX board in GPIO trigger mode. `PlayTrack(index)` pulls the corresponding pin LOW for 200ms. `Reset()` pulses RST to halt playback.

### `SwitchController`
Reads four DIP switches on GP6–9 with internal pull-ups. `IsOn(index)` returns `true` when the switch is closed (connected to GND). Switch 0 enables Simple Mode.

### `TimeManager`
Wraps `millis()` into seconds. `GetTime()` returns absolute elapsed time; `GetDeltaTime()` returns per-frame delta. All states use this for timing and smooth value ramps.

---

## Getting Started

### Dependencies
- [Arduino-Pico core](https://github.com/earlephilhower/arduino-pico) — install via Arduino IDE Board Manager
- [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) — install via Library Manager

### Upload
1. Open `MagicMillipod.ino` in Arduino IDE
2. Select board: **Raspberry Pi Pico**
3. Upload — hold BOOTSEL on first flash if needed

### Serial Monitor
Baud rate **115200**. All subsystems emit prefixed logs (`[Idle]`, `[Calibration]`, `[Audio]`, `[Distance]`, `[UV]`, etc.) useful for tracing state transitions and sensor readings.

---

## Design Notes

- **All hardware controllers are singletons.** Never instantiate them directly — always use `::Instance()`.
- **States use `TimeManager`, not `millis()` directly.** This keeps timing consistent with delta time used for smooth ramps.
- **Two separate distance averages exist for a reason.** The 4-sample fast average keeps hand detection snappy in Idle. The 60-sample slow average prevents a brief sensor glitch or momentary obstruction from falsely triggering an exit during the presentation.
- **Calibration is for exit detection only.** It has no effect on the 30 cm hand trigger in IdleState.
- **Simple Mode (DIP switch 0) bypasses calibration entirely.** Introduction, Presentation, and ExitRoom all use a fixed 50 cm threshold when Simple Mode is active. Use this when room geometry makes calibration unreliable.
- **Audio trigger pins must be HIGH on boot.** The Sound FX board treats a floating LOW as a trigger. All trigger pins are set HIGH as the very first lines of `setup()`.
- **The reset button is dual-purpose.** Short press (< 1s) enters CalibrationState. Long press (≥ 1s) calls `ResetToIdle()`. The distinction is handled in `loop()` by tracking press duration on release.
- **UV MOSFET is N-channel: HIGH = on.** The Pico's 3.3V gate drive is marginal for full enhancement on some devices. If UV brightness seems weak, swap to a logic-level MOSFET (e.g. IRLZ44N).
