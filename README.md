# Pololu3piPlus2040-cpp-project

Line-following and demo firmware for the **Pololu 3pi+ 2040** robot, written in C++ using the official Pololu libraries.

## Overview

This project implements multiple operating modes selectable at startup via menu displayed on the OLED. The code is designed for direct deployment on the Pololu 3pi+ 2040 and focuses on real-time motor control, sensor feedback, and user interaction through buttons, LEDs, buzzer, and display.

The primary focus is a high-speed PID-based line follower, with additional experimental and demonstration modes.

## Features

- **OLED menu system** for mode selection  
- **Line sensor calibration** with visual feedback  
- **High-speed PID line following** optimized for sharp turns  
- **Multiple operating modes**
- **RGB LED effects**
- **Buzzer music playback**

## Modes

### Mode 1: Line (Turbo)
- PID-controlled line following using calibrated reflectance sensors
- Dynamic braking based on line error
- Tuned for high-speed cornering
- Blue RGB LED indication during operation

### Mode 2: Maze
- Placeholder for future maze-solving logic

### Mode 3: Woble
- Simple reactive movement based on line position
- Intended as a test or demonstration mode

### Mode 4: Spin (Party Mode)
- Continuous spinning motion
- RGB LED strobe effects
- “Party” display message
- Can be stopped safely using Button A

## Usage

1. Download `Arduino IDE`
2. Go to boards install `Arduino Mbed OS RP2040 Boards`
3. Then go to library install `Pololu3piPlus32U4` and click "Install all"
4. Now go to `https://github.com/adamgreen/pololu-3pi-plus-2040-arduino-library` and install his library over "code" and "Download ZIP"
5. Go to Arduino IDE and under "Sketch", "Include Library" add the ZIP file
6. Install our ZIP and unpack it
7. Now you can open the folder and run the `.ino`
8. Go under "Tools" select boards "Arduino Mbed OS RP2040 Boards" then "Raspberry Pi Pico"
9. Now you can connect your `Pololu 3pi+ 2040` and upload

## Startup Flow

1. Custom OLED characters are loaded  
2. Mode selection menu is shown  
3. Line sensors are calibrated (except in Spin mode)  
4. Live sensor readings can be viewed  
5. Startup sound is played  
6. Main loop begins  

## Hardware Used

- Pololu 3pi+ 2040 Robot
- Built-in:
  - OLED display
  - Line sensors
  - Motors
  - RGB LEDs
  - Buttons A, B, C
  - Buzzer

## Dependencies

- `Pololu3piPlus2040`
- `PololuMenu`

These must be installed via the Arduino Library Manager or from Pololu’s official repositories.

## License

This project is released for **free use**.  
You are allowed to use, modify, and redistribute the code without restriction.  
No warranty is provided.

## Notes

- PID constants (`Kp`, `Kd`) and speed parameters are tuned for aggressive behavior and may require adjustment depending on surface and battery condition.
- Mode 2 is intentionally left unimplemented.
- The code assumes a standard 5-sensor line configuration.

## Acknowledgment
- Project was created by @nikitaniky, @UL-J, @OJ-A, @ElJones44
- Based on adamgreen's LineFollowerSimple.
