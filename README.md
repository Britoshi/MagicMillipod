# MagicMillipod

An interactive proximity-triggered exhibit controller built on a Raspberry Pi Pico (RP2040). A hand sensor triggers a timed audio/visual presentation sequence. Room-exit detection — via a calibrated distance sensor — resets the system automatically when a visitor leaves.

---

## Project Overview

MagicMillipod is a museum exhibit controller. A visitor waves their hand in front of the sensor to start the show. The system plays through an introduction and a full presentation with synchronized audio, UV lighting, and NeoPixel effects. If the visitor leaves mid-presentation, the system detects this and resets to idle. After a full presentation, it waits for the visitor to leave before resetting.

**Key features:**
- Proximity-triggered state machine (no buttons needed for visitors)
- Calibrated room-exit detection using a rolling distance average
- Calibration data persists across power cycles (stored in flash)
- UV light ramp with PWM brightness control
- Three NeoPixel rings with per-state fade effects
- Staff calibration and reset via a single physical button
- Button short press skips to next state when outside Idle
- Persistent visit counter stored in flash (increments each Introduction)
- Simple Mode (DIP switch 0) for venues where calibration is impractical
- Skip Left-Room Check Mode (DIP switch 1) to ignore early visitor exit

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
                    ┌──── person left (avg) ────┐
                    │    (if switch 1 OFF)       ▼
[Idle] ──(hand < 30cm)──► [Introduction] ──(51s)──► [Presentation]
                                │                        │
                    person left │ (if switch 1 OFF)      │ person left
                                ▼                        │ (if switch 1 OFF)
                             [Idle]          ◄───(3min)──┘
                                                  │
                                            [ExitRoom]
                                     (10s AND person gone)
                                                  │
                                               [Idle]

Button short press (Idle)       ──► [Calibration] ──(10s)──► [Idle]
Button short press (other state)──► next state (audio reset first)
Button long press  (any state)  ──► [Idle]
```

---

## Hardware

| Component | Description |
|---|---|
| Raspberry Pi Pico (RP2040) | Microcontroller |
| HC-SR04 | Ultrasonic distance sensor (hand trigger + room exit detection) |
| Adafruit Audio FX Sound Board (16MB) | Three-track audio, GPIO trigger mode |
| TPA3116D2 2×50W Amplifier | Amplifies audio signal to drive speaker |
| Dayton Audio PC105-4 4" Speaker | Full-range audio output |
| NeoPixel Ring ×3 (7 LED each) | Front ring (Ring 1), back/interior ring (Ring 2), sensor box indicator (Ring 3) |
| UV light + N-channel MOSFET | UV LED switched via PWM |
| Momentary push button | Staff reset / calibration / state-skip trigger |
| DIP switch ×4 | Feature configuration |

### Pin Map

| GPIO | Direction | Assignment |
|---|---|---|
| GP2 | Output | SFX Trigger 1 |
| GP3 | Output | SFX Trigger 2 |
| GP4 | Output | SFX Trigger 0 |
| GP5 | Output | SFX Reset |
| GP6 | Input (pullup) | DIP Switch 0 — Simple Mode |
| GP7 | Input (pullup) | DIP Switch 1 — Skip Left-Room Check |
| GP8 | Input (pullup) | DIP Switch 2 — Reserved |
| GP9 | Input (pullup) | DIP Switch 3 — Reserved |
| GP12 | Output | HC-SR04 TRIG |
| GP13 | Input | HC-SR04 ECHO |
| GP15 | Input (pullup) | Reset / Calibration button (active LOW) |
| GP16 | Output | NeoPixel Ring 3 — sensor box indicator |
| GP17 | Output (PWM) | UV light (N-channel MOSFET gate) |
| GP18 | Output | NeoPixel Ring 1 — front/outer |
| GP19 | Output | NeoPixel Ring 2 — back/interior |

### DIP Switches

| Switch | Name | Function |
|---|---|---|
| 0 | Simple Mode | Replaces calibrated exit detection with a fixed 50 cm threshold |
| 1 | Skip Left-Room Check | Intro and Presentation ignore early visitor exit — always run full duration |
| 2 | — | Reserved |
| 3 | — | Reserved |

---

## Key Components

### `StateMachine` / `State` / `StateFactory`
The core FSM. `State` is an abstract base with `OnStateEnter`, `OnStateUpdate`, `OnStateExit`, `CheckSwitchState`, and `InitializeSubState`. `StateFactory` owns all concrete state instances. `StateMachine` holds the active state pointer and calls `Update()` each loop. `ResetToIdle()` forces a transition to Idle from anywhere. `EnterCalibration()` enters calibration. `AdvanceToNextState()` resets audio and jumps to the next state in the sequence (Intro → Presentation → ExitRoom → Idle).

### `IdleState`
Waits for a hand within 30 cm. Ring 1 solid blue, Ring 2 rainbow. Transitions to Introduction on trigger. The 30 cm threshold is fixed and independent of calibration.

### `IntroductionState`
Plays audio track 0. Ring 1 red, Ring 2 rainbow fading out over the last 10 seconds. Runs 51 seconds then transitions to Presentation. If DIP switch 1 is OFF, resets to Idle early if the visitor leaves. Increments the persistent visit counter in flash on every entry.

### `PresentationState`
Plays audio track 1. Ring 1 red, Ring 2 off. UV ramps from 0 to full over the first 80 seconds, then fades back to 0 for the remainder of its 3-minute duration. Transitions to ExitRoom on completion. If DIP switch 1 is OFF, resets to Idle early if the visitor leaves.

### `ExitRoomState`
All three NeoPixel rings and the UV light fade in gradually over 5 seconds:
- **Ring 1 (front/outer)** — fades in red
- **Ring 2 (back/interior)** — fades in white
- **Ring 3 (sensor box)** — fades in red

Waits for **both** 10 seconds to elapse **and** the sensor to confirm the visitor has left. Uses `CalibrationData` in normal mode, or a fixed 50 cm threshold in Simple Mode. Transitions to Idle when both conditions are met.

### `CalibrationState`
Samples the distance sensor for 10 seconds with the room empty. Computes the average (door distance) and half the spread (error range), stores them to `CalibrationData`, and immediately saves to flash. All rings yellow during calibration. Returns to Idle on completion.

### `CalibrationData`
Singleton storing `doorDistance`, `errorRange`, and `isCalibrated`. Provides `IsPersonPresent(dist)` and `HasPersonLeft(dist)`. Falls back to a 60 cm hardcoded threshold if not calibrated. The 1.5× error multiplier provides a grace margin around the calibrated door distance. `Save()` and `Load()` persist data to `/calibration.txt` in LittleFS flash — calibration survives power cycles and only needs to be repeated if the room changes.

### `DistanceSensorManager`
Polls HC-SR04 at 4 Hz. Buffers are pre-filled with 200 cm on boot so the average starts high rather than triggering immediately. Readings below 2 cm (no echo / timeout) are discarded to preserve existing buffer values. Readings above 200 cm are clamped to 100 cm. Maintains two rolling averages:
- `GetAverageDistance()` — 4-sample short average, used for hand detection and exit checks
- `GetLongAverageDistance()` — 12-sample (3s) long average, available for stable exit detection

### `NeoPixelController`
Wraps three `Adafruit_NeoPixel` instances. Provides:
- `SetAll(r, g, b)` — Ring 1 solid color
- `Ring1Fade(r, g, b, t)` — Ring 1 color scaled by t (0.0–1.0), avoids destructive `setBrightness`
- `Ring2Rainbow(brightness)` — non-blocking time-based rainbow with fade support
- `Ring2Fade(r, g, b, t)` — Ring 2 color scaled by t
- `Ring3SetAll(r, g, b)` — Ring 3 solid color
- `Ring3Fade(r, g, b, t)` — Ring 3 color scaled by t
- `Off()`, `Ring2Off()`, `Ring3Off()` — clear and show each ring

All fade methods scale RGB values directly per frame rather than using `setBrightness()`, which is destructive and corrupts stored pixel data.

### `UVLightController`
PWM brightness control via N-channel MOSFET. `SetBrightness(float)` accepts 0.0–1.0. PWM at 1 kHz. Only logs to serial when the duty value changes to avoid flooding the output. UV is only active during `PresentationState`.

### `AdafruitAudio`
Controls the Adafruit Sound FX board in GPIO trigger mode. `PlayTrack(index)` pulls the corresponding pin LOW for 200ms. `Reset()` pulses RST to halt playback. Audio is reset before any button-triggered state skip.

### `SwitchController`
Reads four DIP switches on GP6–9 with internal pull-ups. `IsOn(index)` returns `true` when the switch is closed (connected to GND).

### `VisitCounter`
Singleton backed by LittleFS. Loads the visit count from `/visits.txt` on `Begin()` and writes an updated count back to flash on every `Increment()` call (called in `IntroductionState::OnStateEnter`). Count persists across power cycles and resets. Only cleared by a full flash erase.

### `TimeManager`
Wraps `millis()` into seconds. `GetTime()` returns absolute elapsed time; `GetDeltaTime()` returns per-frame delta. All states use this for timing and smooth value ramps.

---

## Getting Started

### Dependencies
- [Arduino-Pico core](https://github.com/earlephilhower/arduino-pico) — install via Arduino IDE Board Manager using the URL: `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`
- [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) — install via Library Manager

### Flash Size
**Tools → Flash Size** must be set to any option that includes a filesystem partition (e.g. `2MB (Sketch: 1MB, FS: 1MB)`). Do **not** select "No FS" — the visit counter and calibration persistence require LittleFS storage. LittleFS formats itself automatically on first boot.

### Upload
1. Open `MagicMillipod.ino` in Arduino IDE
2. Select board: **Raspberry Pi RP2040 Boards (Earle Philhower) → Raspberry Pi Pico**
3. Set Flash Size to include a FS partition (see above)
4. Upload — hold BOOTSEL on first flash if needed

### Serial Monitor
Baud rate **115200**. All subsystems emit prefixed logs (`[Idle]`, `[Calibration]`, `[Audio]`, `[Distance]`, `[UV]`, `[VisitCounter]`, etc.) useful for tracing state transitions and sensor readings.

---

## Design Notes

- **All hardware controllers are singletons.** Never instantiate them directly — always use `::Instance()`.
- **States use `TimeManager`, not `millis()` directly.** This keeps timing consistent with delta time used for smooth ramps.
- **Distance buffer pre-filled with 200 cm on boot.** Prevents false triggers at startup before real readings arrive.
- **No-echo readings (< 2 cm) are discarded.** When the sensor times out (nothing in range), `pulseIn` returns 0. Discarding these preserves the last valid buffer values rather than driving the average to zero.
- **Fade effects use direct RGB scaling, not `setBrightness()`.** Adafruit NeoPixel's `setBrightness()` is destructive — it permanently scales stored pixel data. Scaling RGB values per frame avoids this.
- **Calibration is for exit detection only.** It has no effect on the 30 cm hand trigger in IdleState.
- **Calibration and visit count persist in LittleFS flash.** Both survive power loss. Calibration only needs repeating if the room layout changes. Visit count only resets on a full flash erase.
- **Simple Mode (DIP switch 0) bypasses calibration for exit detection.** Uses a fixed 50 cm threshold instead.
- **Skip Left-Room Check (DIP switch 1) disables early-exit detection** in Introduction and Presentation. Both states always run their full duration regardless of sensor readings.
- **Audio trigger pins must be HIGH on boot.** The Sound FX board treats a floating LOW as a trigger. All trigger pins are set HIGH as the very first lines of `setup()`.
- **The reset button is dual-purpose.** In Idle, short press (< 1s) enters CalibrationState. In any other state, short press skips to the next state (audio is reset first). Long press (≥ 1s) always calls `ResetToIdle()`.
- **UV MOSFET is N-channel: HIGH = on.** The Pico's 3.3V gate drive is marginal for full enhancement on some devices. If UV brightness seems weak, swap to a logic-level MOSFET (e.g. IRLZ44N).
