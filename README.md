# Digital-Multimeter
A full-featured Digital Multimeter using ATmega-16: measures voltage, current, resistance
#  Digital Multimeter (DMM) – Embedded Systems Project

>  Duration: Feb 2024 – Present  
>  Final-Year Project | Faculty of Engineering, Cairo University

---

##  Overview

A complete **Digital Multimeter (DMM)** built using **ATmega-16**, capable of measuring:

-  AC & DC Voltage
-  Current (Amps)
-  Resistance (Ohms)

The system uses analog-to-digital conversion with signal conditioning and switching to select measurement modes.

---

##  Technologies Used

- **Microcontroller**: ATmega-16
- **Language**: Embedded C
- **Simulation**: Proteus
- **PCB Design**: (Assumed Eagle/Altium)
- **Tools**: Multimeter, Oscilloscope, Breadboard, LCD, Potentiometer

---

##  Real Hardware Setup

![DMM Hardware](images/dmm_real_setup.jpg)

---

##  Circuit Design

The system is divided into several circuits:

- Voltmeter
- Ammeter
- Ohmmeter
- AC-to-DC Conversion
- Power Supply

 You can find all schematics in the `/schematics/` folder.

-  [Circuit Diagram PDF](schematics/Circuit_Diagram.pdf)
-  [Proteus Simulation File](schematics/Embedded_Project.pdsprj)

---

##  Features

- Accurate measurements displayed on LCD
- Integrated mode-switching logic
- Input protection and calibration
- Simulated and tested on hardware

---

##  Folder Structure

```bash
/firmware/       → Embedded C source code (coming soon)
/schematics/     → Circuit diagram + Proteus project
/images/         → Project photos
README.md
