#  Technical Walkthrough – Digital Multimeter Project

##  Design Challenges Faced
- Accurate ADC readings with noisy signals
- Scaling analog input ranges (0–5V) to real-world voltages
- Ensuring protection against over-voltage input
- Switching between modes (voltmeter, ammeter, ohmmeter) safely

##  Core Concepts Used
- Ohm's Law calculations
- Use of voltage dividers and shunt resistors
- ADC readings via ATmega-16
- Proteus simulation techniques

##  Testing Procedure
- Used simulated sources in Proteus to provide inputs
- Verified values on LCD against calculated reference
- Introduced noise and verified filtering

##  Future Improvements
- Add auto-ranging
- Support for capacitance measurement
- Shift to STM32 or ARM Cortex for more precision
