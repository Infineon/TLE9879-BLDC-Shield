# TLE9879-BLDC-Shield

<img src="https://github.com/Infineon/Assets/blob/master/Pictures/TLE9879_BLDC_shield.jpg" width="400">



Arduino library for Infineon's [BLDC Shield with TLE9879](https://www.infineon.com/cms/en/product/evaluation-boards/bldc_shield_tle9879/).

## Summary
The BLDC shield is a small evaluation board equipped with the TLE9879 to use with Arduino. Up to 4 boards can be stacked ontop of each other to control 4 BLDC motors indepedently. Each shield can supply up to 10 A load current. The implemented integrated [TLE9879](https://www.infineon.com/cms/en/product/microcontroller/embedded-power-ics-system-on-chip-/3-phase-bridge-driver-integrated-arm-cortex-m3/)
is a inteligent B6 MOSFET driver designed especially for automotive motion control applications such as wiper, engine cooling fan or Fuel pump.

## Key Features and Benefits
* 32-bit Arm® Cortex® -M3 core at up to 40 MHz
* Single power supply from 5.5 V to 28 V
* FLASH up to 128 KB, RAM up to 6 KB
* 1x LIN transceiver
* 2x UART, 2x SSC
* 3-phase bridge driver with charge pump and PWM generator
* 1x low-side shunt current sense amplifier
* 1x 10-bit ADC/8 ch and 1x 8-bit ADC/10 ch
* 10x 16-bit timer, 1x 24 bit timer
* Temperature Range-GRADE1 TJ: -40 °C up to 150 °C
* Temperature Range-GRADE0 TJ: -40 °C up to 175 °C
* Ultra compact application footprint with VQFN-48 packages


## Target Applications:
* Wiper
* Engine cooling fan
* Auxillary water pump
* Fuel pump
* Oil pump

## Examples
Three examples are available to be used with the TLE9879 BLDC shield.

* [Blinky](examples/bldc_shield_blinky_test/bldc_shield_blinky_test.ino)
* [Single motor](examples/bldc_shield_single_motor_test/bldc_shield_single_motor_test.ino)
* [Multiple motor](examples/bldc_shield_multiple_motor_test/bldc_shield_multiple_motor_test.ino)

A documentation of the API for Arduino can be found in chapter 4 of the [user manual](Infineon-BLDC_Shield_with_TLE9879QXA40_for_Arduino-UM.pdf).





