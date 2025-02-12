# Microcontrollers

## Overview
This repository contains microcontroller projects and experiments conducted using the PIC24F Curiosity Development Board. The projects cover various aspects of microcontroller programming, including hardware setup, timers, PWM control, and sensor integration.

## Projects

### 1. **Counter Implementation**
- Set up an I2C LCD display with the PIC24F Curiosity Board.
- Created a counter program displaying values in hexadecimal format.
- Modified the counter to display decimal values.

### 2. **Egg Timer**
- Implemented a countdown timer initialized at 2:00 (min:sec).
- Used a timer interrupt for countdown functionality.
- S2 starts the countdown, and S1 resets the timer.

### 3. **LED Blinking using Timer**
- Utilized a 32.768 kHz clock crystal.
- Programmed TIMER2 interrupt to blink an onboard LED at 1 Hz.

### 4. **LED Dimming using PWM**
- Used ADC to sample input from a potentiometer every 500ms.
- Controlled the brightness of an RGB LED’s red component based on input.
- Implemented linear dimming from off to full brightness.

### 5. **Altimeter using BMP180**
- Integrated the BMP180 pressure sensor with the microcontroller.
- Displayed temperature, static air pressure, and altitude.
- Implemented real-time data updates from the sensor.

## Documentation
The repository also includes the following reference documents:
- **BMP180.pdf** - Datasheet for the BMP180 digital pressure sensor.
- **dsPIC Programmers Reference 70157C.pdf** - Reference manual for dsPIC30F microcontrollers.
- **PIC24FJ128GA204.pdf** - Datasheet for the PIC24FJ128GA204 microcontroller.

## Setup Instructions
1. **Hardware Requirements:**
   - PIC24F Curiosity Development Board
   - I2C LCD Display
   - BMP180 Sensor
   - RGB LED and Potentiometer
   - USB connection to MPLAB IDE

2. **Software Requirements:**
   - MPLAB X IDE
   - XC16 Compiler
   - Microchip Programmer

3. **Compilation and Uploading:**
   - Open MPLAB X and create a new project.
   - Add the required source and header files.
   - Compile the project and upload it to the board.

## Author
Miray Dicle

## License
This project is licensed under the MIT License.
