# Ultrasonic Distance Sensor System – Abstandssensor


## Usage Notice

This project was developed as part of the module “Information Technology / Networked Systems” in the Bachelor's program in Information and Communication Engineering.

- Repository is archived and read-only.
- Not licensed for reuse, redistribution, or modification.
- For demonstration and educational reference only.
- Licensed under **UNLICENSED**.

> Developed by **Fbarquez** - HTW Berlin, 2021

---

## About the Project

This project is a **microcontroller-based distance measuring system** designed to support social distancing during the COVID-19 pandemic. The sensor system detects nearby objects using **ultrasound technology** and triggers **visual and acoustic alerts** when the safety distance is not maintained.

> **Main goal:** Help individuals monitor and maintain the recommended 1.5–2 meters distance to prevent virus transmission.

---

## Media Preview

### [▶ Watch Demo Video](https://www.dropbox.com/scl/fi/pl3xh61fnlcbmjqz21ddf/Abstandssensor_video_FB.mp4?rlkey=nnty2kcn74rb1pb28xz7tbtho&st=4qxa9l1h&raw=1)

> A live demo showing the behavior of the sensor system in real conditions.

---

## Project Objectives

- Measure distances in real-time using the **HC-SR04 ultrasonic sensor**
- Visually indicate proximity using **LEDs** (green = safe, red = warning)
- Trigger **buzzer** alert when distance is dangerously short
- Display distance on a **20x4 LCD display**
- **Transmit data via Bluetooth** to a mobile app
- Provide a **portable, reliable, and affordable** prevention tool

---

## Hardware Overview

| Component              | Description                               |
|------------------------|-------------------------------------------|
| PIC16F1936             | 8-bit microcontroller (main control unit) |
| LCD 20x4               | Visual distance output                    |
| HC-SR04                | Ultrasonic distance sensor                |
| LEDs                   | Red (short), Green (safe) alerts          |
| Buzzer                 | Audio alert for danger distance           |
| HC-05 Bluetooth Module | Wireless transmission to Android          |
| External Power Supply  | 5V regulated                              |

> Schematics and PCB were designed using **EAGLE** software.

---

## System Logic

```text
              +--------------------+
              |    Ultrasonic      |
              |     Sensor         |
              +--------+-----------+
                       |
             Measures Time of Echo
                       |
                       v
        +--------------+--------------+
        |     PIC16F1936 MCU          |
        | (Distance Calculation Logic)|
        +-------+----------+----------+
                |          |         
         +------v--+  +----v-----+ 
         |   LCD    |  | Bluetooth| --> Smartphone App
         +----------+  +----------+
             |
     +-------+--------+
     |      LEDs      |
     |   Buzzer Alarm |
     +----------------+
````

The MCU uses **Timer1** to measure the echo pulse from the sensor. Based on the measured time, it calculates distance using:

```
distance_cm = duration / 58
```

Depending on the result:

* **> 50 cm** → Green LED ON (safe)
* **30–50 cm** → Red LED ON (warning)
* **< 30 cm** → Red LED + Buzzer ON (danger)

---

## How It Works

1. **Startup**: LCD displays intro text → system initializes
2. **Sensor trigger**: Sends 10μs pulse to HC-SR04 TRIG
3. **Echo measured**: Timer1 starts on echo rise, stops on fall
4. **Distance calculation**: Converts time into cm
5. **LED + Buzzer logic**: Executes based on thresholds
6. **LCD display**: Shows distance in cm
7. **Bluetooth transmission**: Sends data to Android app

---

## Project Structure

```
Abstandsssensor.X/
├── main.c                   # Main program logic
├── lcd 20x4_1.h             # Custom LCD display driver
├── mcc_generated_files/     # Microchip Code Configurator auto-generated code
├── MyConfig.mc3             # MCC configuration file
├── nbproject/               # MPLAB X project configuration
├── Makefile                 # Project build rules
├── .clang-format            # Code formatting configuration
├── build/, dist/, debug/    # Build artifacts (can be ignored or gitignored)
├── eagle/
│   ├── Diseño .brd          # PCB layout
│   └── Diseño .sch          # Schematic diagram
```

---

## Features

* Real-time distance display on LCD
* Visual LED indication based on proximity
* Acoustic alarm for dangerous range
* Bluetooth communication with Android device
* Fully coded in C using **XC8** and **MPLAB X IDE**
* Clean code style with `.clang-format`
* Version-controlled with Git

---

## How to Build & Run

1. Open the project in **MPLAB X IDE**
2. Ensure you have:

   * XC8 compiler installed
   * MCC plugin enabled
3. Configure your hardware:

   * Connect HC-SR04 to RA0 (TRIG) and RA1 (ECHO)
   * Connect LEDs and Buzzer to PORTC
4. Program the PIC16F1936 using SNAP or PICKIT
5. Power on — the system is ready to operate

---

## Android App Integration

* The HC-05 Bluetooth module sends messages like:

  * `"DISTANCE: 045cm"`
  * `"ALARM: DANGEROUS"`
* App GUI made with **Bluetooth Electronics App**
* Enables real-time distance visualization on your phone

---

## Eagle Design Files

Schematic (`.sch`) and Board Layout (`.brd`) are included in the `eagle/` directory for PCB fabrication and review.

---

## Requirements

To build, program, or extend this project, you will need a set of development tools such as the MPLAB X IDE and the XC8 compiler.

Please refer to the [requirements.md](requirements.md) file for full details on the required software, versions, and links to official downloads.

---

## References

* [PIC16F1936 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/41364E.pdf)
* [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
* [HC-05 Bluetooth](https://components101.com/sites/default/files/component_datasheet/HC-05)
* [HD44780 LCD](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

---

For usage restrictions and legal information, see [NOTICE.md](NOTICE.md).

---


