# Introduction
The [ESPROS epc901](https://www.espros.com/photonics/epc901/) is a CCD line sensor with a resolution of 1024 monochrome pixels.

Compared to other image sensors the epc901 is very easy to drive, requiring only one voltage, I2C, a few digital I/Os and an ADC. It is also relatively inexpensive
at [$24 in single units from Digi-key](https://www.digikey.com/en/products/detail/espros-photonics-ag/EPC901-CSP32-033/10516882). On the downside, the sensor
comes in a BGA package, the evaluation kit is [very expensive at $1350](https://www.digikey.com/en/products/detail/espros-photonics-ag/EPC901-EVALUATION-KIT-V1/10516848),
and we haven't seen this sensor from any other distributors than Digi-key.

This repository contains design files to build hardware and software to interact with the sensor. 

Except for files imported from the STM32 SDK ([```./software/breakout/Drivers```](https://github.com/astuder/epc901/tree/main/software/breakout/Drivers)), and a few files
auto-generated by STM32CubeIDE, all files in this project are licensed under *TBD*.

# Hardware

All hardware in this project is designed with KiCad version 5.1.6.

## Shared

The folder [```./hardware/shared```](https://github.com/astuder/epc901/tree/main/hardware/shared) contains KiCad symbols, footprints and 3d models that were
created for this project.

## Breakout board

The folder [```./hardware/breakout```](https://github.com/astuder/epc901/tree/main/hardware/breakout) contains the KiCad project for the epc901 breakout board.

The images sensor comes in a 2x16, 0.5 mm pitch BGA package and looks very fragile. With careful handling and placement, we successfully assembled a few boards
using a stainless steel solder stencil and an unmodified toaster oven for reflow. Reflow on a hotplate probably works as well.

In addition to the image sensor and related passives, the breakout board also integrates an ADC. Footprint and firmware are compatible with [Texas Instruments ADCS7476](https://www.ti.com/product/ADCS7476) and [Analog Devices AD7476](https://www.analog.com/en/products/ad7476.html) which are 1 MSPS 12-bit ADCs with SPI-like interface.

### Connector pinout

Pin|Function|Comment
-|-|-
1|GND|digital ground
2|PWR_DOWN|see epc901 datasheet
3|DATA_RDY|see epc901 datasheet
4|CLR_DATA|see epc901 datasheet
5|SHUTTER|see epc901 datasheet
6|CLR_PIX|see epc901 datasheet
7|GNDA|analog ground
8|VIDEO_P|positive video signal for use w/ external ADC, see epc901 datasheet
9|VIDEO_N|negative video signal for use w/ external ADC, see epc901 datasheet
10|GNDA|analog ground
11|GND|digital ground
12|ADC_CLK|SPI clk to on-board ADC
13|ADC_DATA|SPI data from on-board ADC
14|ADC_CS|SPI chip select of on-board ADC
15|GND|digital ground
16|3V3|3.3V power supply
17|GND|digital ground
18|READ|see epc901 datasheet
19|SDA|I2C data
20|SCL|I2C clock

### Jumpers

ID|Function|Comment
-|-|-
JP1|STAR GND|star ground point between GND and GNDA. Close to connect, leave open if GNDA and GND are connected off-board.
JP2|ADC SELECTION|selects whether to use the on-board ADC or an external ADC. External ADC will route VIDEO_P to pin 8 in the connector.

### Config resistor array

These resistors control the configuration of the image sensor on power-up. For details, see sections 4, 7.1 and 12 in the epc901 datasheet.

### Physical dimensions

The size of the board is 61 x 36 mm. The mounting holes in the corners are spaced 55 x 30 mm and have a diameter of 2.2 mm, designed for M2 screws.

The mounting holes for the lens holder are located 29 mm from the left board edge and 8 mm from top and bottom board edge. They have a diameter of 2.2 mm, designed for M2 screws.

![epc901-breakout dimensions](https://github.com/astuder/epc901/blob/main/hardware/breakout/epc901-breakout-dims.png)

### Lens holder & lens

The breakout board has space and mounting holes for a lens holder. The spacing between the mounting holes is 20mm. For full coverage, the lens should support 1/2.3" or larger sensors.

For our prototypes, we chose a [M12 metal lens holder w/ 20mm hole spacing](https://www.m12lenses.com/CNC-Machined-M12-Lens-Holder-Metal-p/pt-lh031m.htm) and a [25mm M12 lens for 1/2" sensors](https://www.m12lenses.com/25mm-F2-4-5MP-CCTV-Lens-p/pt-2524mp5y.htm). In the US, a wide selection of lens holders and lenses can be found at [M12 Lenses Inc](https://www.m12lenses.com).

For use in a spectrograph, a lens may not be required. However we recommend protecting the sensor against accidental physical damage with some sort of bracket or baffel.

## Adapter board

The folder [```./hardware/adapter-shield```](https://github.com/astuder/epc901/tree/main/hardware/adapter-shield) contains the KiCad project for an adapter to mount the 
epc901 breakout board on the NUCLEO evaluation board.

The layout of the adapter board follows the Arduino Shield standard. This means you could mount the adapter and sensor breakout board on any compatible MCU board. Though
you will have to write your own firmware.

# Software

## Firmware

The folder [```./software/breakout```](https://github.com/astuder/epc901/tree/main/software/breakout) contains the firmware to control the epc901 breakout board with an
STM32L4 using the [NUCLEO-L496ZG evaluation board](https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-L496ZG/6697037). This specific MCU and
evaluation board were chosen because we had one laying around.

The firmware is developed and built with the latest version of STM32CubeIDE (version 1.4.2 as of October 2020). To import the project into STM32CubeIDE, clone this
repository, open STM32CubeIDE and import the project with ```File > Open Project From File System..```, and select the folder ```./software/breakout```. Follow the standard
procedure for building the firmware and programming the NUCLEO evaluation board.

## Python

The folder [```./software/python```](https://github.com/astuder/epc901/tree/main/software/python) contains Python library that wraps the serial interface exposed by the
firmware, and examples of Python scripts that use the library.

### epc901camera.py

Python library wrapping the serial interface exposed by the firmware into an API that's easy to use from Python.

### live.py

Python script that displays output of the image sensor in realtime.

### snap.py

Python script that captures one image.

### burst.py

Python script that captures a series of images in a burst.