# Introduction
The [ESPROS epc901](https://www.espros.com/photonics/epc901/) is a CCD line sensor with a resolution of 1024 monochrome pixels.

Compared to other image sensors the epc901 is very easy to drive, requiring only one voltage, I2C, a few digital I/Os and an ADC. It is also relatively inexpensive
at [$24 in single units from Digi-key](https://www.digikey.com/en/products/detail/espros-photonics-ag/EPC901-CSP32-033/10516882). On the downside, the sensor
comes in a BGA package, the evaluation kit is [very expensive at $1350](https://www.digikey.com/en/products/detail/espros-photonics-ag/EPC901-EVALUATION-KIT-V1/10516848),
and we haven't seen this sensor from any other distributors than Digi-key.

This repository contains design files to build hardware and software to interact with the sensor. 

## Table of contents

[Hardware](#hardware)
* [Breakout board](#breakout-board)
  * [Connector pinout](#connector-pinout)
  * [Jumpers](#jumpers)
  * [Config resistor array](#config-resistor-array)
  * [Physical dimensions](#physical-dimensions)
  * [Lens holder & lens](#lens-holder--lens)
* [Adapter board](#adapter-board)
  * [Pin mapping](#pin-mapping)
  * [Jumpers](#jumpers)
* [Shared](#shared)

[Software](#software)
* [Firmware](#firmware)
  * [Shell](#shell)
  * [Capture logic](#capture-logic)
  * [Trigger logic](#trigger-logic)
  * [Transfer format](#transfer-format)
* [Python](#python)
  * [epc901camera.py](#epc901camerapy)
  * [example scripts](#example-scripts)
 
[License](#license)

# Hardware

All hardware in this project is designed with KiCad version 5.1.6.

The hardware consists of a breakout board for the image sensor, and an adapter board for vertically mounting the breakout board on an ST Nulceo-144 development board.

![Breakout and adapter boards mounted on a NUCLEO L496ZG](https://github.com/astuder/epc901/blob/main/images/epc901-revA-hardware.jpg)

## Breakout board

The folder [```./hardware/breakout```](https://github.com/astuder/epc901/tree/main/hardware/breakout) contains the KiCad project for the epc901 breakout board.

The image sensor comes in a 2x16, 0.5 mm pitch BGA package and looks very fragile. With careful handling and placement, we successfully assembled a few boards
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
you will likely have to adapt the firmware to make it work.

### Pin mapping

|Arduino|Nucleo|L496ZG|Breakout|Comment
|-|-|-|-|-
|+3V3|CN8 7|+3V3||connected to JP1
|+5V|CN8 9|+5V||connected to 3.3V LDO
|GND|CN8 11|GND|GND|
|GND|CN8 13|GND|GND|
|D0|CN10 16|PD9|PWR_DOWN|
|D1|CN10 14|PD8|DATA_RDY|
|D2|CN10 12|PF15|CLR_DATA|
|D3|CN10 10|PE13|SHUTTER|controlled by TIM1_CH3
|D4|CN10 8|PF14|CLR_PIX|
|D9|CN7 18|PD15|READ|
|D10|CN7 16|PD14|ADC_CS|
|D12/MISO|CN7 12|PA6|ADC_DATA|
|D13/SCK|CN7 10|PA5|ADC_CLK|
|GND|CN7 8|GND|GND|
|A0|CN9 1|PA3|VIDEO_P|
|A1|CN9 3|PC0|VIDEO_N|
||CN10 1|AVDD||connected to JP3, reserved to supply analog power from Nucleo in future revisions
||CN10 3|AVSS||connected to JP4, alternative source for analog ground from Nucleo
||CN10 5|GND|GND|
||CN10 7|PB1|VIDEO_P|alternative analog pins for use with AVDD and AVSS connection in future revisions
||CN10 9|PC2|VIDEO_N|alternative analog pins for use with AVDD and AVSS connection in future revisions

### Jumpers

ID|Function|Comment
-|-|-
JP1|3.3V VCC|select between LDO on adapter or Nucleo (HOST) as source for 3.3V power supply to breakout board
JP2|* GND|star ground point between GND and GNDA. Close to connect, leave open if GNDA and GND are connected on breakout board.
JP3|AVDD|reserved for future use to supply power to VDD_OA of the EPC901 from the Nucleo
JP4|AVSS|star ground point between analog ground of the breakout board and analog ground of the Nucleo. May be useful when using the ADCs of the STM32.

## Shared

The folder [```./hardware/shared```](https://github.com/astuder/epc901/tree/main/hardware/shared) contains custom KiCad symbols, footprints and 3d models that can be reused across KiCad projects in this repository.

# Software

## Firmware

The folder [```./software/breakout```](https://github.com/astuder/epc901/tree/main/software/breakout) contains the firmware to control the epc901 breakout board with an
STM32L4 using the [NUCLEO-L496ZG evaluation board](https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-L496ZG/6697037). This specific MCU and
evaluation board were chosen because we had one laying around.

The firmware is developed and built with the latest version of STM32CubeIDE (version 1.4.2 as of October 2020). To import the project into STM32CubeIDE, clone this
repository, open STM32CubeIDE and import the project with ```File > Open Project From File System..```, and select the folder ```./software/breakout```. Follow the standard
procedure for building the firmware and programming the NUCLEO evaluation board.

### Shell

The firmware exposes a command shell over serial. The parameters of the serial connection are 115200 8N1.

Preceeding a command with @ will suppress local echo and command prompt, which is useful when writing an application that interacts with the shell.

Most commands that configure an aspect of the firmware return the current setting if no value is provided. For example ```exposure``` will return the currently configured exposure time.

|Command|Description|
|-|-|
|help|display list of available commands|
|help <command>|display help text for command|
|echo <on/off>|turn local echo on or off|
|regread <register>|read register of image sensor over I2C|
|regwrite <register> <value>|write to value to register of image sensor over I2C|
|reset mcu|reset the STM32 microcontroller|
|reset camera|reset the camera settings, for example exposure or burst, to their defaults|
|reset sensor|software reset of the image sensor through I2C|
|exposure <time_us>|set exposure time in microseconds|
|exposure max|get maximum exposure time supported|
|capture|start capture using the configured settings|
|capture abort|if there's a capture in progress, abort current capture|
|transfer|read next image from the frame buffer|
|transfer all|read all images available in the frame buffer|
|transfer last|read last image from frame buffer, all other images will be discarded|
|burst <on/off>|turn burst mode on or off|
|burst fast|turn on burst mode with fast capture, where image capture and data read out overlap. this can be noisier than regular burst mode.|
|burst frames <count>|set number of frames to capture in a burst|
|burst interval <time_ms>|set interval in milliseconds at which frames are captured. 0 will capture frames at the maximum speed possible.|
|trigger <on/off>|enable or disable trigger|
|trigger source <external/level/region>|select source of trigger signal|
|trigger direction <rising/falling>|set which direction of a signal will trigger. for ```level``` and ```region``` rising means above/inside level/region, falling means NOT above/inside|
|trigger level <level>|set trigger level for ```level``` trigger source|
|trigger region <x1 y1 x2 y2>|set trigger region for ```region``` trigger source|

### Capture logic

The capture command will start a process based on the configuration of the camera.

First the firmware check if there's enough space in the frame buffer for the configured number of frames (1 or burst frames). An error will be returned if there is not enoug space.

Next, if trigger is enabled, the camera waits for the trigger condition is met. 

Next, image capture starts. If burst is enabled, it will capture the configured number of frames. All frames captured are stored in a frame buffer. 

After the capture process is complete, images can be read from the frame buffer with the transfer command.

The shell will respond with ```BUSY``` if the camera is waiting for a trigger or a single image capture or burst is in progress. At any time, the command ```capture abort``` will return the camera back to idle state.

### Trigger logic

**External**

The external trigger source is wired to pin D35/PB11 and the USER button (B1).

Trigger direction ```rising``` triggers when the signal changes from low to high, or when the button is pressed down.
Trigger direction ```falling``` trigers when the signal changes from high to low, or when the button is released.

**Level**

The level trigger source uses live sensor data to trigger. When starting capture, the firmware will continously capture images until a trigger occurs or capture is aborted.

Trigger direction ```rising``` triggers when any pixel is above the configured trigger level.
Trigger direction ```falling``` triggers when no pixel is above the configured trigger level.

**Region**

The region trigger source uses live sensor data to trigger. When starting capture, the firmware will continously capture images until a trigger occurs or capture is aborted.

Trigger direction ```rising``` triggers when any pixel is inside the configured trigger region.
Trigger direction ```falling``` triggers when no pixel is inside the configured trigger region.

### Transfer format

The transfer command will read images out of the frame buffer and send them over serial. Each image is sent in two chuncks, each terminated with a linefeed. 

1. Frame metadata seperated with commas: frame number, time in milliseconds since first frame, exposure time in milliseconds
2. Frame data: a continous string with a 3-character hexadecimal number per pixel representing the brightness value

## Python

The folder [```./software/python```](https://github.com/astuder/epc901/tree/main/software/python) contains Python library that wraps the serial interface exposed by the
firmware, and examples of Python scripts that use the library.

### epc901camera.py

Python library wrapping the serial interface exposed by the firmware into an API that's easy to use from Python.

### example scripts

There are three scripts to interact with the breakout board:
* **live.py**: displays output of the image sensor in realtime
* **snap.py**: captures one image
* **burst.py**: captures a series of images in a burst

The scripts are controlled with shared and a few unique command line arguments.
|Argument|live.py|snap.py|burst.py|description|
|-|-|-|-|-|
|-h, --help|:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|Display help text.|
|-p PORT|:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|Serial port which connects to the camera board, this is the only required argument.|
|-e EXPOSURE|:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|Set exposure time in microseconds. If not specified, the default exposure time is 1ms (1000us).|
|-t {external,level,region}||:heavy_check_mark:|:heavy_check_mark:|Enable trigger and select trigger source. See [Trigger logic](#trigger-logic).|
|-tdir {falling,rising}||:heavy_check_mark:|:heavy_check_mark:|Set direction of trigger logic.|
|-tl TRIG_LEVEL||:heavy_check_mark:|:heavy_check_mark:|Set brightness for level trigger. Valid values are 0-4095.|
|-tr x1,y1,x2,y2||:heavy_check_mark:|:heavy_check_mark:|Define rectangle for region trigger. Valid x values are 0-1023, valid y values are 0-4095.|
|-td TRIG_DELAY||:heavy_check_mark:|:heavy_check_mark:|Set delay in milliseconds after trigger event before image capture starts.|
|-f FRAMES|||:heavy_check_mark:|Number of frames to capture in a burst.|
|-i INTERVAL|||:heavy_check_mark:|Set time interval in milliseconds between frames. 0 means best effort, i.e. at maximum speed possible. As it takes 1.3ms to read out a frame, effective capture rates may be lower than the configured interval. |
|-fast|||:heavy_check_mark:|Enable special burst mode where frames are read out of the sensor while the next exposure is captured. This may increase noise, but will increase effective burst speeds.|
|-gq||:heavy_check_mark:|:heavy_check_mark:|Don't display graph window.|
|-g GRAPH_TYPE|||:heavy_check_mark:|Display animated 2D graph or 3D surface plot. If argument is missing, no graph will be displayed.|
|-gc COLOR|||:heavy_check_mark:|Select alternative color map for 3D graph.|
|-gpng GRAPH_FILE||:heavy_check_mark:|:heavy_check_mark:|Save graph as PNG file.|
|-png PNG_FILE||:heavy_check_mark:|:heavy_check_mark:|Save sensor data as a gray-scale bitmap in a PNG file.|
|-csv CSV_FILE||:heavy_check_mark:|:heavy_check_mark:|Save sensor data as CSV file.|
|-a|:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|Automatically scale vertical axis of graphs based on image data. If not specified, the Y axis will go from 0 to 3000.|
|-sx px1,w1,px2,w2|:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|Scale labels of horizontel axis of graphs by linear interpolation based on two points, for example to reflect wavelengths in a spectrograph. If not specified, the X axis will be labelled with pixel positions 0-1024|

# License

Unless stated otherwise, all software in this project is licensed under the terms of MIT License.

The main exception are files imported from the STM32 SDK ([```./software/breakout/Drivers```](https://github.com/astuder/epc901/tree/main/software/breakout/Drivers)), and a few files
auto-generated by STM32CubeIDE. See source code for applicable license terms.

All hardware in this project is licensed under the terms of CERN Open Hardware Licence Version 2 - Permissive.

See [LICENSE-SW](LICENSE-SW) and [LICENSE-HW](LICENSE-HW) for additional details.
