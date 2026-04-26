# MagicMillipod

An interactive installation built on a Raspberry Pi Pico (RP2040). A distance sensor detects visitors and drives a timed sequence of audio, UV lighting, and LED effects across three states.

---

## Project Overview

MagicMillipod is a proximity-triggered exhibit controller. When a visitor approaches, it transitions through an introduction and presentation sequence with synchronized audio, UV light ramp-up, and NeoPixel ring effects. It returns to idle automatically after the presentation ends, or immediately via a physical reset button.

---

## System Architecture

The firmware is structured around a **finite state machine** running on the Arduino framework for RP2040. All hardware subsystems are managed as **singletons** : initialized once in `setup()` and accessed globally via `Instance()`.

```
┌─────────────────────────────────────────────────────┐
│                    Main Loop                        │
│  TimeManager → DistanceSensorManager → StateMachine │
└─────────────────────────────────────────────────────┘
                          │
              ┌───────────▼───────────┐
              │      StateMachine     │
              │  currentState->Update()│
              └───────────────────────┘
                          │
         ┌────────────────┼────────────────┐
         ▼                ▼                ▼
    IdleState     IntroductionState  PresentationState
```

**State transitions:**
```
[Idle] ──(sensor < 30cm)──► [Introduction] ──(2 min)──► [Presentation] ──(3 min)──► [Idle]

Reset button pressed from any state ──► [Idle]
```

---

## Hardware

| Component | Description |
|---|---|
| Raspberry Pi Pico (RP2040) | Microcontroller |
| HC-SR04 | Ultrasonic distance sensor |
| Adafruit Sound FX Mini | Audio board, GPIO trigger mode |
| WS2812B ring x2 | NeoPixel LED rings (7 LEDs each) |
| UV light + N-channel MOSFET | UV LED strip switched via PWM (RFP30N06LE) |
| Momentary push button | Hardware reset to idle |
| Sensor indicator LED | On while in idle state |

### Pin Map

| GPIO | Assignment |
|---|---|
| GP2 | SFX Trigger 1 |
| GP4 | SFX Trigger 0 |
| GP5 | SFX Reset |
| GP12 | HC-SR04 TRIG |
| GP13 | HC-SR04 ECHO |
| GP15 | Reset button (active LOW, internal pull-up) |
| GP16 | Sensor indicator LED |
| GP17 | UV light PWM (N-channel MOSFET gate) |
| GP18 | NeoPixel Ring 1 data |
| GP19 | NeoPixel Ring 2 data |

---

## Key Components

### `StateMachine` / `State` / `StateFactory`
The core state machine. `State` is an abstract base class with `OnStateEnter`, `OnStateUpdate`, `OnStateExit`, and `CheckSwitchState`. `StateFactory` owns all concrete state instances. `StateMachine` holds the current state and calls `Update()` each loop. `ResetToIdle()` forces an immediate transition to idle from anywhere.

### `IdleState`
Waits for a visitor. Ring 1 is blue, Ring 2 cycles rainbow. Includes suppression logic: if the sensor is already triggered on entry, it waits for 5 continuous seconds of clear readings before arming detection (prevents stuck-trigger false starts).

### `IntroductionState`
Plays audio track 0, Ring 1 red. Ring 2 rainbow fades out over the last 15 seconds. UV stays off. Runs for ~51 seconds then transitions to Presentation.

### `PresentationState`
Plays audio track 1, Ring 1 red, Ring 2 off. UV ramps up from 0 to full over ~10 seconds, then fades back out starting at the 1:20 mark. Runs for 3 minutes then returns to Idle.

### `DistanceSensorManager`
Polls the HC-SR04 at 20 Hz. Maintains a rolling 4-sample average. Exposes `distance` (latest reading) and `GetAverageDistance()`.

### `NeoPixelController`
Singleton wrapping two `Adafruit_NeoPixel` instances. `SetAll(r,g,b)` controls Ring 1. `Ring2Rainbow(brightness)` drives a time-based non-blocking rainbow on Ring 2 with optional brightness scale (0.0–1.0 for fade effects). `Ring2Off()` clears Ring 2.

### `UVLightController`
PWM-based brightness control via an N-channel MOSFET. `SetBrightness(float)` takes 0.0–1.0. PWM frequency is 1 kHz.

### `AdafruitAudio`
Controls the Adafruit Sound FX Mini in GPIO trigger mode. Tracks are triggered by pulling a pin LOW for 200ms. `Reset()` pulses the RST pin to stop playback.

### `TimeManager`
Wraps `millis()` into seconds. Provides `GetTime()` (absolute) and `GetDeltaTime()` (frame delta) used by states for timing and smooth ramps.

---

## Getting Started

### Dependencies
- [Arduino-Pico core](https://github.com/earlephilhower/arduino-pico) : install via Arduino IDE Board Manager
- [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) : install via Library Manager

### Upload
1. Open `MagicMillipod.ino` in Arduino IDE
2. Select board: **Raspberry Pi Pico**
3. Upload : hold BOOTSEL on first flash if needed

### Serial Monitor
Set baud rate to **115200**. All subsystems log prefixed output (`[Idle]`, `[Audio]`, `[Distance]`, `[UV]`, etc.) useful for debugging state transitions and sensor readings.

---

## Design Notes

- **All hardware controllers are singletons** : do not instantiate them directly, always use `::Instance()`.
- **States do not own timing logic** : use `TimeManager::GetTime()` and `GetDeltaTime()` rather than `millis()` directly, so timing stays consistent with the rest of the system.
- **`begin()` is a no-op on RP2040 for NeoPixels** : the PIO state machine starts on the first `show()` call. Always call `show()` during `Begin()` to initialize the PIO before the state machine starts.
- **Audio trigger pins must be HIGH on boot** : the Sound FX Mini treats a floating LOW as a trigger. `pinMode` + `digitalWrite HIGH` for all trigger pins are the very first lines of `setup()` for this reason.
- **Ring 2 is powered at 3.3V** : the Pico's 3.3V data line is marginal against a 5V-powered WS2812B (threshold is 3.5V). Running Ring 2 from the 3.3V rail drops the threshold to ~2.3V and makes data reliable without a level shifter.
- **UV MOSFET** : uses an N-channel MOSFET (RFP30N06LE). HIGH = on. 12V supply on the drain side is within the 60V rating. The Pico's 3.3V gate drive is marginal for full enhancement on this device : if UV brightness seems weak, a logic-level MOSFET (e.g. IRLZ44N) is a drop-in improvement.
