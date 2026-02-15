

# ArduinoMega2560_ServoSG90

This repository contains code templates, configurations, and documentation designed for projects based on the Arduino Mega 2560 microcontroller (ATmega2560).  
It serves as a starting point or foundation for developers who want to build modular, optimized, and well‑structured applications on the Mega2560 platform.

The goal of this repository is to provide a ready‑to‑use base project that simplifies the setup of new developments.  
Instead of starting from scratch, you can clone this repository and immediately have:
- A clean project structure compatible with PlatformIO and Arduino IDE.
- Preconfigured build flags for memory optimization and performance.
- Example modules for diagnostics, GPIO, PWM, UART, and EEPROM.
- Documentation templates (README, diagrams, configuration notes) to keep your project organized.




## Table of Contents

-- [Introduction](#introduction)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Debug](#debug)
- [Functions](#functions)
- [Arduino Mega 2560 Pinout](#arduino-mega-2560-pinout)
- [PlatformIO Configuration](#platformio-configuration)
- [Servo SG90 — Technical Deep Dive](#servo-sg90--technical-deep-dive)
- [License](#license)

## Introduction

This project serves as a foundation for various Arduino Mega 2560-based applications. It leverages the PlatformIO IDE for streamlined development, building, and deployment.

## Getting Started

These instructions will guide you through setting up the project on your local machine for development and deployment.

### Prerequisites

Before you begin, ensure you have the following installed:

-   [PlatformIO IDE](https://platformio.org/platformio-ide) (Installation guide available on the PlatformIO website)

### Installation

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/edujimser/ArduinoMega2560.git
    cd ArduinoMega2560
    ```

2.  **Open the project in PlatformIO IDE:**

    -   Launch PlatformIO IDE.
    -   Click "Open Project" and select the cloned `ArduinoMega2560` directory.

3.  **Build the project:**

    -   In the PlatformIO IDE, click the "PlatformIO" icon in the bottom toolbar.
    -   Select "Build" to compile the code.

4.  **Upload the code to your Arduino Mega 2560:**

    -   Connect your Arduino Mega 2560 to your computer via USB.
    -   In the PlatformIO IDE, select "Upload" to flash the compiled code to your board.

## Project Structure

The project structure is organized as follows:

```
ArduinoMega2560/
├── .gitignore              # Specifies intentionally untracked files that Git should ignore
├── .vscode/                # Contains VS Code specific settings
├── include/                # Header files for the project
├── lib/                    # Libraries used in the project
│   └── avr-debugger/       # Example library (if applicable)
├── platformio.ini          # PlatformIO configuration file
├── src/                    # Source code files
│   ├── main.cpp            # Main application file
│   └── system/             # System-related source files (if applicable)
└── README.md             # Project documentation
```

## Usage

The `src/main.cpp` file contains the main application logic. You can modify this file to implement your desired functionality.  Refer to the comments within `main.cpp` for guidance on how to use the code.

Example (from `src/main.cpp`):

```cpp
#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello, Arduino!");
  delay(1000);
}
```

## Debug
This project includes a full debugging system for the Arduino Mega 2560 using **avr-stub**, **GDB**, and an **FT232BL** USB–Serial adapter.  
This enables professional-level firmware debugging on a microcontroller that does not support hardware debugging natively.

---

###  Requirements

To use the debug mode, you will need:

- Arduino **Mega 2560**
- **FT232BL** USB–Serial adapter
- **PlatformIO** (VSCode)
- The `avr-stub` library (already included in the project)
- Basic wiring between the FT232BL and the Mega

---

###  FT232BL Wiring

Connect the FT232BL to the Mega 2560 as follows:

| FT232BL | Mega 2560 |
|---------|-----------|
| TXD     | RX0 (0)   |
| RXD     | TX0 (1)   |
| GND     | GND       |

> The FT232BL is used by GDB to communicate with the microcontroller.

---

###  PlatformIO Configuration

The debug environment is already defined in `platformio.ini`:

```ini
[env:debug]
build_type = debug
debug_tool = custom
debug_port = /dev/ttyUSB0

build_flags =
    -DDEBUG_MODE
    -Og
    -g

lib_deps =
    jdolinay/avr-debugger

```

## Functions

Here's a description of the key functions used in the `src/main.cpp` file:

*   **`void setup()`**: This function is called once at the beginning of the program. It is typically used to initialize variables, set pin modes, start serial communication, and perform other setup tasks.

    *   `Serial.begin(9600)`: Initializes serial communication at a baud rate of 9600. This allows the Arduino to send data to the serial monitor for debugging and monitoring.

*   **`void loop()`**: This function is called repeatedly in an infinite loop. It contains the main logic of the program, such as reading sensor data, controlling actuators, and responding to user input.

    *   `Serial.println("Hello, Arduino!")`: Prints the text "Hello, Arduino!" to the serial monitor.
    *   `delay(1000)`: Pauses the program for 1000 milliseconds (1 second).

## Arduino Mega 2560 Pinout

The Arduino Mega 2560 has a variety of pins that can be used for different purposes:

*   **Digital Pins (0-53):** These pins can be configured as either inputs or outputs. Some digital pins have special functions, such as PWM (Pulse Width Modulation) for controlling the brightness of LEDs or the speed of motors.
*   **Analog Input Pins (A0-A15):** These pins can read analog signals from sensors, such as potentiometers or temperature sensors.
*   **PWM Pins:** Marked with a "~" symbol, these pins can be used for PWM output.
*   **Serial Communication Pins (TX, RX):** Used for serial communication with other devices.
*   **SPI Pins:** Used for SPI (Serial Peripheral Interface) communication.
*   **I2C Pins (SDA, SCL):** Used for I2C (Inter-Integrated Circuit) communication.
*   **AREF:** Analog Reference pin for analog inputs.
*   **GND:** Ground pins.

## PlatformIO Configuration

The `platformio.ini` file configures the PlatformIO build environment for this project. Here's a breakdown of the key settings:

```ini
; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed, and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit the documentation for other options and examples:
; https://docs.platformio.org/page/projectconf.html

;----------------------------------------------------------------------------------------------------------------------------------------------------------------
;------ Project Configuration ------
[env:megaatmega2560]
platform = atmelavr           ; Specifies the development platform (Atmel AVR)
board = megaatmega2560       ; Specifies the board (Arduino Mega 2560)
framework = arduino            ; Specifies the framework (Arduino)
upload_port = COM3             ; Port where your board is connected (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux/Mac)
monitor_port = COM3          ; Must match the physical port of your board (same as debug_port normally)
                             ; To check the port in CMD: connect and disconnect the board, then run `mode` in CMD
;----------------------------------------------------------------------------------------------------------------------------------------------------------------
;------ Standard dependencies and extra configurations ------
; Adapting the C++ version for the project: removing GNU++11 standard to insert GNU++17
; https://docs.platformio.org/en/latest/projectconf/advanced_scripting.html#build-unflags-and-build-flags
build_unflags = 
    -std=gnu++11              ; Remove the C++11 standard to avoid conflicts with the C++17 standard
build_flags = 
    -std=gnu++17              ; Use the C++17 standard with GNU extensions (more flexible than the pure standard)
    -DAVR8_UART_NUMBER=3      ; Define a macro to indicate that UART number 3 will be used on AVR microcontrollers
    -DEBUG_MODE=0             ; Activate debug mode to include debugging code
    -Os                       ; Change the optimization level here:
                                ; -O0  → no optimization (debugging)
                                ; -O1  → light optimization
                                ; -O2  → balanced optimization
                                ; -O3  → maximum speed
                                ; -Os  → optimize for size
    -flto                     ; Enable Link Time Optimization (LTO) for better optimization across files
    -fno-exceptions           ; Disable exceptions to reduce code size and improve performance

;----------------------------------------------------------------------------------------------------------------------------------------------------------------
;------ Artificial Debugging Dependencies ------
; Notes:
;   - Configure build_flags (DAVR8_UART_NUMBER, DEBUG_MODE) to activate debug mode
; Activate simulated debugging system with avr-stub
debug_tool = avr-stub         ; Uses the 'avr-debugger' library to simulate debugging on AVR chips
; Virtual serial port for debugging
debug_port = COM6             ; Port where your board is connected (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux/Mac)
                             ; To check the port in CMD: connect and disconnect the board, then run `mode` in CMD
; Serial monitor communication speed
monitor_speed = 57600         ; Baud rate. Must match the one used in Serial.begin() in your code

;----------------------------------------------------------------------------------------------------------------------------------------------------------------
;------ RFID-RC522 Dependencies ------
; The following line tells PlatformIO to automatically download and include 
; the MFRC522 library by Miguel Balboa from PlatformIO's official registry.
; This library allows controlling the RFID RC522 reader module for reading/writing tags and cards.
; lib_deps = /lib/Ard - opcional descomentar si se quiere usar la librería de Arduino por descarga

[platformio]
description = Sensors
```

### Explanation of Settings:

*   **`[env:megaatmega2560]`**: This section defines the environment for the Arduino Mega 2560 board.  `env` stands for environment. You can have multiple environments for different boards or configurations.

    *   `platform = atmelavr`: Specifies that the Atmel AVR platform will be used.
    *   `board = megaatmega2560`:  Specifies that the target board is the Arduino Mega 2560.
    *   `framework = arduino`: Specifies that the Arduino framework will be used.
    *   `upload_port = COM3`: Defines the serial port used for uploading the compiled code to the board.  **Important:**  You need to change this to the correct port for your system (e.g., `/dev/ttyUSB0` on Linux, `COM3` or similar on Windows).
    *   `monitor_port = COM3`: Defines the serial port used for the serial monitor. This should typically match the `upload_port`.

*   **`build_unflags`**: These settings remove default compiler flags.

    *   `-std=gnu++11`:  Removes the default C++11 standard flag. This is done to allow the use of C++17.

*   **`build_flags`**: These settings add custom compiler flags.

    *   `-std=gnu++17`:  Specifies that the C++17 standard should be used for compilation. C++17 provides more modern language features.
    *   `-DAVR8_UART_NUMBER=3`: Defines a macro that might be used in the code to specify

   
   ---
# Servo SG90 — Technical Deep Dive

This section documents the complete internal architecture of the SG90 servo control system implemented in this project.  
It includes timer configuration, PWM generation, register‑level behavior, and the internal design of the `Timmer` and `ServoMotor` classes.

---
## Supported Pins

The SG90 servo module in this project only works on hardware PWM pins
connected to Output Compare (OC) channels of 16‑bit timers.

Supported pins:

| Pin | Timer | Channel | Register |
|-----|--------|---------|----------|
| 2   | Timer3 | OC3B    | OCR3B    |
| 3   | Timer3 | OC3C    | OCR3C    |
| 5   | Timer3 | OC3A    | OCR3A    |
| 6   | Timer4 | OC4A    | OCR4A    |
| 7   | Timer4 | OC4B    | OCR4B    |
| 8   | Timer4 | OC4C    | OCR4C    |
| 11  | Timer1 | OC1A    | OCR1A    |
| 12  | Timer1 | OC1B    | OCR1B    |

These pins match the internal list:

PINES_VALIDOS_SERVO[] = { 2, 3, 5, 6, 7, 10, 11, 12 };

---
## Timer Architecture

The SG90 servo requires a 50 Hz PWM signal:

- Period: 20 ms  
- Pulse width: 1–2 ms  
- Resolution: high precision

To achieve this, the project uses **16‑bit timers**:

- Timer1  
- Timer3  
- Timer4  

All configured in:

**Fast PWM mode with TOP = ICRn**

This mode allows:

- Custom period via ICRn  
- High‑resolution pulse width via OCRnx  
- Stable servo control independent of Arduino’s default PWM

### Waveform Generation Mode (WGM33:WGM30)

To control the SG90 servo, the timers are configured in:

**Fast PWM mode with TOP = ICRn**

This corresponds to:

WGM33 = 1  
WGM32 = 1  
WGM31 = 1  
WGM30 = 0  

This mode allows:

- Custom PWM period using ICRn  
- High‑resolution duty cycle using OCRnx  
- Stable 50 Hz servo signal independent of Arduino’s default PWM

---
## PWM Generation for Servos

The SG90 servo requires a 50 Hz PWM signal:

- Period: 20 ms  
- Pulse width: 1–2 ms  
- Neutral position: 1.5 ms  

To achieve this, the timers are configured with:

- Prescaler = 8  
- Timer clock = 2 MHz  
- Tick duration = 0.5 µs  

The PWM period is defined by:

ICRn = 40000 ticks  
40000 × 0.5 µs = 20 ms

### Pulse Widths

The SG90 interprets the PWM pulse width as the target angle:

- 1.0 ms → 0°
- 1.5 ms → 90° (center)
- 2.0 ms → 180°

With a 0.5 µs timer tick:

1 ms  = 2000 ticks  
1.5 ms = 3000 ticks  
2 ms  = 4000 ticks  

These values are written into the corresponding OCRnx register.

---
## Timmer Class Internals

The `Timmer` class is responsible for configuring the correct hardware timer
based on the selected pin.

Its responsibilities include:

- Detecting the Output Compare channel (OCnx)
- Selecting the correct timer (Timer1, Timer3, Timer4)
- Configuring WGM bits for Fast PWM with TOP = ICRn
- Configuring COM bits for non‑inverted PWM
- Setting the prescaler to 8
- Setting ICRn = 40000 (20 ms period)
- Initializing OCRnx = 3000 (1.5 ms neutral pulse)

---
## Timmer Class Internals

The `Timmer` class is responsible for configuring the correct hardware timer
based on the selected pin.

Its responsibilities include:

- Detecting the Output Compare channel (OCnx)
- Selecting the correct timer (Timer1, Timer3, Timer4)
- Configuring WGM bits for Fast PWM with TOP = ICRn
- Configuring COM bits for non‑inverted PWM
- Setting the prescaler to 8
- Setting ICRn = 40000 (20 ms period)
- Initializing OCRnx = 3000 (1.5 ms neutral pulse)

### Register Configuration

Each timer is configured at register level:

- TCCRnA → waveform + compare output mode  
- TCCRnB → prescaler + high‑order WGM bits  
- ICRn   → defines the PWM period (TOP)  
- OCRnx  → defines the pulse width  
- DDRx   → sets the pin as output  

Example (Timer3):

TCCR3A |= (1 << WGM31);
TCCR3B |= (1 << WGM33) | (1 << WGM32);
TCCR3B |= (1 << CS31);   // prescaler = 8

ICR3 = 40000;            // 20 ms period
OCR3B = 3000;            // 1.5 ms neutral pulse

### Output Compare Channel Selection

Each valid servo pin maps to a specific OC channel:

- Pin 2  → OC3B  
- Pin 3  → OC3C  
- Pin 5  → OC3A  
- Pin 6  → OC4A  
- Pin 7  → OC4B  
- Pin 8  → OC4C  
- Pin 11 → OC1A  
- Pin 12 → OC1B  

The `Timmer` class automatically:

1. Detects the pin  
2. Selects the correct timer  
3. Selects the correct OCRnx register  
4. Enables the correct COMnx1 bit for PWM output

### Initialization Flow

When a servo pin is selected, the `Timmer` class performs:

1. Validate pin is in PINES_VALIDOS_SERVO[]
2. Identify timer (1, 3, or 4)
3. Identify OC channel (A, B, or C)
4. Configure DDRx to set pin as output
5. Configure WGM bits for Fast PWM (TOP = ICRn)
6. Configure COMnx1 for non‑inverted PWM
7. Set prescaler = 8
8. Set ICRn = 40000 (20 ms)
9. Set OCRnx = 3000 (neutral 1.5 ms)

---
## ServoMotor Class Internals

The `ServoMotor` class provides the high‑level interface for controlling
a servo using the underlying `Timmer` instance.

Responsibilities:

- Validate the selected pin
- Create and configure a `Timmer` object
- Convert angle (0–180°) into pulse width (ticks)
- Write the pulse width into OCRnx
- Provide smooth and safe angle updates

### Angle to Pulse Conversion

The SG90 expects:

- 0°  → 1.0 ms  → 2000 ticks
- 90° → 1.5 ms  → 3000 ticks
- 180° → 2.0 ms → 4000 ticks

The `ServoMotor` class computes:

ticks = map(angle, 0, 180, 2000, 4000)

Then writes:

OCRnx = ticks

This directly updates the PWM pulse width generated by the timer.

### Safety and Limits

The `ServoMotor` class enforces:

- Angle clamping between 0° and 180°
- Minimum pulse = 2000 ticks (1.0 ms)
- Maximum pulse = 4000 ticks (2.0 ms)
- Neutral pulse = 3000 ticks (1.5 ms)

If an invalid angle is provided:

- It is automatically corrected
- The servo is never driven beyond safe limits

### Write & Update Flow

When `setAngle()` is called, the `ServoMotor` class:

1. Clamps the angle to 0–180°
2. Converts the angle to ticks (2000–4000)
3. Writes the value into OCRnx
4. The timer hardware updates the PWM pulse on the next cycle
5. The servo receives the new pulse width and moves accordingly

No delays or blocking loops are used; updates are instantaneous.

### Neutral & Startup Behavior

On creation, every `ServoMotor` instance:

- Initializes the timer through `Timmer`
- Sets the PWM pulse to 1.5 ms (3000 ticks)
- Ensures the servo starts in a safe neutral position
- Avoids sudden jumps or mechanical stress

This guarantees predictable and safe startup behavior for all servos.

### Detach & Stop Behavior

The `ServoMotor` class supports disabling the PWM output:

- COMnx1 bit is cleared
- OCRnx stops driving the pin
- The pin becomes a normal digital output
- The servo stops receiving pulses and holds its last position

This is useful for:
- Power saving
- Releasing the servo when not needed
- Avoiding jitter when idle

### Multi‑Servo Independence

Each `ServoMotor` instance:

- Uses its own `Timmer` object
- Configures a dedicated hardware timer channel (OCnx)
- Operates fully independently from other servos
- Does not block or interfere with other PWM outputs

This allows:

- Multiple SG90 servos running simultaneously
- Precise timing for each channel
- No jitter caused by shared timers or software PWM

### Timing Stability

The servo control system guarantees:

- No use of delay() or blocking loops
- No dependence on Arduino’s default PWM
- No interference from millis() or micros()
- Hardware‑driven timing at 2 MHz
- Deterministic 20 ms period (ICRn = 40000)
- Deterministic pulse width resolution of 0.5 µs

This ensures extremely stable and jitter‑free servo movement.

### Performance & Efficiency

The servo control system is designed for maximum efficiency:

- No CPU time spent generating PWM
- All timing handled by hardware timers
- Updates to OCRnx take only a few CPU cycles
- Zero jitter thanks to deterministic hardware timing
- Minimal memory footprint (no dynamic allocation)

This allows precise multi‑servo control without impacting the main program.

### Resolution & Precision

With a 2 MHz timer clock and 0.5 µs tick:

- Pulse width resolution = 0.5 µs
- Angle resolution ≈ 0.03°
- Smooth and highly precise positioning
- No rounding artifacts in OCRnx updates

This level of precision exceeds the mechanical resolution of the SG90,
ensuring extremely smooth motion.

### Non‑Blocking Design

The entire servo control system is fully non‑blocking:

- No use of delay()
- No waiting loops
- No timer resets during operation
- OCRnx updates happen instantly
- The main loop remains 100% free for user code

This allows complex robotics logic to run without being slowed down by servo control.

### Robustness & Error Handling

The `ServoMotor` class includes defensive mechanisms:

- Rejects invalid pins (not in PINES_VALIDOS_SERVO[])
- Prevents reinitializing an already configured timer
- Ensures OCRnx writes are always within safe bounds
- Avoids undefined behavior by validating timer/channel mapping
- Provides predictable behavior even with unexpected inputs

This guarantees stable operation in all supported scenarios.


### Abstraction Layer

The `ServoMotor` class acts as a clean abstraction layer:

- Hides all low‑level timer configuration
- Exposes a simple API: setAngle(), detach(), attach()
- Ensures safe defaults (neutral position, valid ranges)
- Allows the user to control servos without touching registers
- Keeps hardware‑specific logic encapsulated in `Timmer`

This separation makes the system modular, reusable, and easy to maintain.

### Extensibility

The architecture is designed to be easily extended:

- New servo models can redefine pulse limits
- Additional helper methods (e.g., sweep, easing) can be added
- Timer configuration is isolated in `Timmer`
- High‑level behavior is isolated in `ServoMotor`
- The system can scale to more servo types or timer modes

This modularity ensures long‑term maintainability and flexibility.

### Summary & Architecture Map

The servo control system is composed of two layers:

1. **Timmer (low‑level)**
   - Configures hardware timers
   - Sets PWM mode, prescaler, TOP, and OCRnx
   - Manages pin → timer → channel mapping

2. **ServoMotor (high‑level)**
   - Converts angles to pulse widths
   - Applies safety limits
   - Provides simple API for user code
   - Ensures safe startup and clean detach

Together, they form a robust, modular, and precise servo control architecture.

---
## High‑Level API Overview

The library exposes a clean and simple interface for controlling SG90 servos:

- No need to configure timers manually
- No need to write to registers
- No need to calculate pulse widths

The user only interacts with the `ServoMotor` class:

### Basic usage:

ServoMotor servo(9);   // Attach servo to pin 9
servo.setAngle(90);    // Move to 90°
servo.detach();        // Stop PWM output

All low‑level details are handled internally by the library.

---
## Creating a ServoMotor Instance

To control a servo, simply create a `ServoMotor` object:

ServoMotor servo(9);

This performs:

1. Validation of pin 9 as a valid servo pin
2. Automatic timer configuration through `Timmer`
3. Automatic PWM activation on the correct OCnx channel
4. Startup at neutral position (1.5 ms pulse)

No extra setup or initialization is required.

---
## Setting the Servo Angle

To move the servo, simply call:

servo.setAngle(90);

This performs:

1. Angle validation (0°–180°)
2. Conversion to pulse width (2000–4000 ticks)
3. Hardware update of OCRnx
4. Immediate PWM refresh on the next cycle

The movement is smooth, precise, and non‑blocking.

---
## Detaching the Servo

To stop sending PWM to the servo:

servo.detach();

This performs:

1. Disabling the COMnx1 bit (PWM output off)
2. Releasing the pin as a normal digital output
3. Stopping the servo from receiving pulses
4. Holding the last mechanical position without jitter

Useful for:
- Saving power
- Releasing the servo when not needed
- Avoiding unnecessary noise or vibration

---
## Reattaching the Servo

If a servo was previously detached, you can re-enable PWM:

servo.attach();

This performs:

1. Reconfiguration of the timer channel if needed
2. Re‑enabling the COMnx1 bit (PWM output on)
3. Restoring the last known angle
4. Resuming stable 50 Hz servo pulses

Useful when:
- You temporarily disabled the servo
- You want to resume movement after a pause

---
## Angle Limits & Safety

The library enforces safe angle limits:

servo.setAngle(200);   // Automatically clamped to 180°
servo.setAngle(-30);   // Automatically clamped to 0°

This prevents:

- Overdriving the servo
- Mechanical stress or damage
- Invalid pulse widths reaching the hardware

The user can always assume the servo will operate within safe bounds.

---
## Controlling Multiple Servos

Each servo is fully independent:

ServoMotor s1(9);
ServoMotor s2(10);
ServoMotor s3(11);

You can move them simultaneously:

s1.setAngle(45);
s2.setAngle(90);
s3.setAngle(135);

Thanks to hardware timers:
- No blocking
- No interference between channels
- No jitter even with several servos running

---
## Smooth & Non‑Blocking Motion

All servo movements are non‑blocking:

s1.setAngle(45);
s2.setAngle(120);

The calls return immediately.

Thanks to hardware timers:

- No delay() is used
- No busy‑waiting loops
- No CPU time wasted generating PWM
- The main loop remains fully available

This allows complex logic, sensors, and communication to run without interruption.

---
## Main Loop Compatibility

The library is designed to work seamlessly inside loop():

void loop() {
    servo1.setAngle(90);
    servo2.setAngle(45);
}

Because all PWM generation is hardware‑based:

- No blocking occurs
- No CPU time is consumed by the servo system
- The loop can run at full speed
- Sensor reading, communication, and logic remain unaffected

This makes the library ideal for robotics and real‑time applications.

---
## Compatibility with User Delays & Timers

The servo system works even if the user adds delays:

delay(500);
servo.setAngle(120);

Because PWM is fully hardware‑driven:

- Delays do NOT interrupt servo pulses
- Timers used by the user do NOT interfere
- The servo continues receiving stable 50 Hz signals

This ensures predictable behavior in mixed‑timing applications.

---
## Using Servos in Larger Projects

The library integrates cleanly into complex architectures:

- Inside functions:
    void mover() { servo.setAngle(30); }

- Inside classes:
    class Robot {
        ServoMotor brazo;
    public:
        Robot() : brazo(9) {}
        void saludar() { brazo.setAngle(90); }
    };

- In multi‑file projects:
    - No global state required
    - No shared timers to manage manually
    - Each servo manages its own configuration

This keeps large codebases clean, modular, and easy to maintain.

---
## Practical Usage Tips

To get the best results in real projects:

- Mount the servo firmly to avoid vibration
- Use an external power supply if driving multiple servos
- Share grounds between Arduino and servo power
- Avoid forcing the servo beyond its mechanical limits
- Detach the servo when holding torque is not required

These practices improve stability, reduce noise, and extend servo lifespan.

---
## Full Basic Example

A minimal working example:

#include "ServoMotor.h"

ServoMotor servo(9);

void setup() {
    servo.setAngle(90);   // Move to center
}

void loop() {
    servo.setAngle(0);
    delay(1000);

    servo.setAngle(180);
    delay(1000);
}

This demonstrates:

- Simple instantiation
- Immediate

---
## Multi‑Servo Example

#include "ServoMotor.h"

ServoMotor base(9);
ServoMotor brazo(10);
ServoMotor pinza(11);

void setup() {
    base.setAngle10);
    pinza.setAngle(0);
    delay(1000);
}

This demonstrates:
- Independent control of each servo
- No blocking or interference
- Clean, readable high‑level code

---
## Detach/Attach Example (Power Saving)

#include "ServoMotor.h"

ServoMotor servo(9);

void setup() {
    servo.setAngle(90);
}

void loop() {
    servo.setAngle(0);
    delay(500);

    servo.detach();      // Stop PWM, save power
    delay(1000);

    servo.attach();      // Resume PWM
    servo.setAngle(180);
    delay(500);
}

This demonstrates:
- How to temporarily disable the servo
- How to resume control cleanly
- Reduced noise and power consumption




    ### License
    
    This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
    
    ```
    MIT License
    
    Copyright (c) [Año] [Nombre del titular del copyright]
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    ```