EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "epc901 Breakout to Shield Adapter"
Date ""
Rev "A"
Comp "(C)2020 Adrian Studer"
Comment1 "CERN Open Hardware Licence Version 2 - Permissive"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 8950 1450 1    60   ~ 0
Vin
Text Label 9350 1450 1    60   ~ 0
IOREF
Text Label 8900 2500 0    60   ~ 0
VIDEO_P
Text Label 8900 2600 0    60   ~ 0
VIDEO_N
Text Label 10550 3000 0    60   ~ 0
PWR_DOWN
Text Label 10550 2900 0    60   ~ 0
DATA_RDY
Text Label 10550 2800 0    60   ~ 0
CLR_DATA
Text Label 10550 2700 0    60   ~ 0
SHUTTER
Text Label 10550 2600 0    60   ~ 0
CLR_PIX
Text Label 10550 2000 0    60   ~ 0
READ
Text Label 10550 1900 0    60   ~ 0
ADC_CS
Text Label 10550 1700 0    60   ~ 0
ADC_DATA
Text Label 10550 1600 0    60   ~ 0
ADC_CLK
Text Label 10550 1400 0    60   ~ 0
AREF
NoConn ~ 9400 1600
Text Label 10550 1300 0    60   ~ 0
SDA
Text Label 10550 1200 0    60   ~ 0
SCL
Text Notes 10850 1000 0    60   ~ 0
Holes
Text Notes 8550 750  0    60   ~ 0
Shield for Arduino that uses\nthe same pin disposition\nlike "Uno" board Rev 3.
$Comp
L Connector_Generic:Conn_01x08 P1
U 1 1 56D70129
P 9600 1900
F 0 "P1" H 9600 2350 50  0000 C CNN
F 1 "Power" V 9700 1900 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x08" V 9750 1900 20  0000 C CNN
F 3 "" H 9600 1900 50  0000 C CNN
	1    9600 1900
	1    0    0    -1  
$EndComp
Text Label 8650 1800 0    60   ~ 0
Reset
$Comp
L power:+3.3V #PWR01
U 1 1 56D70538
P 9150 1450
F 0 "#PWR01" H 9150 1300 50  0001 C CNN
F 1 "+3.3V" V 9150 1700 50  0000 C CNN
F 2 "" H 9150 1450 50  0000 C CNN
F 3 "" H 9150 1450 50  0000 C CNN
	1    9150 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR02
U 1 1 56D707BB
P 9050 1350
F 0 "#PWR02" H 9050 1200 50  0001 C CNN
F 1 "+5V" V 9050 1550 50  0000 C CNN
F 2 "" H 9050 1350 50  0000 C CNN
F 3 "" H 9050 1350 50  0000 C CNN
	1    9050 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 56D70CC2
P 9300 3150
F 0 "#PWR03" H 9300 2900 50  0001 C CNN
F 1 "GND" H 9300 3000 50  0000 C CNN
F 2 "" H 9300 3150 50  0000 C CNN
F 3 "" H 9300 3150 50  0000 C CNN
	1    9300 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 56D70CFF
P 10300 3150
F 0 "#PWR04" H 10300 2900 50  0001 C CNN
F 1 "GND" H 10300 3000 50  0000 C CNN
F 2 "" H 10300 3150 50  0000 C CNN
F 3 "" H 10300 3150 50  0000 C CNN
	1    10300 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 P2
U 1 1 56D70DD8
P 9600 2700
F 0 "P2" H 9600 2300 50  0000 C CNN
F 1 "Analog" V 9700 2700 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x06" V 9750 2750 20  0000 C CNN
F 3 "" H 9600 2700 50  0000 C CNN
	1    9600 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P5
U 1 1 56D71177
P 10800 650
F 0 "P5" V 10900 650 50  0000 C CNN
F 1 "CONN_01X01" V 10900 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 10721 724 20  0000 C CNN
F 3 "" H 10800 650 50  0000 C CNN
	1    10800 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P6
U 1 1 56D71274
P 10900 650
F 0 "P6" V 11000 650 50  0000 C CNN
F 1 "CONN_01X01" V 11000 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 10900 650 20  0001 C CNN
F 3 "" H 10900 650 50  0000 C CNN
	1    10900 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P7
U 1 1 56D712A8
P 11000 650
F 0 "P7" V 11100 650 50  0000 C CNN
F 1 "CONN_01X01" V 11100 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" V 11000 650 20  0001 C CNN
F 3 "" H 11000 650 50  0000 C CNN
	1    11000 650 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 P8
U 1 1 56D712DB
P 11100 650
F 0 "P8" V 11200 650 50  0000 C CNN
F 1 "CONN_01X01" V 11200 650 50  0001 C CNN
F 2 "Socket_Arduino_Uno:Arduino_1pin" H 11024 572 20  0000 C CNN
F 3 "" H 11100 650 50  0000 C CNN
	1    11100 650 
	0    -1   -1   0   
$EndComp
NoConn ~ 10800 850 
NoConn ~ 10900 850 
NoConn ~ 11000 850 
NoConn ~ 11100 850 
$Comp
L Connector_Generic:Conn_01x08 P4
U 1 1 56D7164F
P 10000 2600
F 0 "P4" H 10000 2100 50  0000 C CNN
F 1 "Digital" V 10100 2600 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x08" V 10150 2550 20  0000 C CNN
F 3 "" H 10000 2600 50  0000 C CNN
	1    10000 2600
	-1   0    0    -1  
$EndComp
Wire Notes Line
	8525 825  9925 825 
Wire Notes Line
	9925 825  9925 475 
Wire Wire Line
	9350 1450 9350 1700
Wire Wire Line
	9350 1700 9400 1700
Wire Wire Line
	9400 1900 9150 1900
Wire Wire Line
	9400 2000 9050 2000
Wire Wire Line
	9400 2300 8950 2300
Wire Wire Line
	9400 2100 9300 2100
Wire Wire Line
	9400 2200 9300 2200
Connection ~ 9300 2200
Wire Wire Line
	8950 2300 8950 1450
Wire Wire Line
	9050 2000 9050 1350
Wire Wire Line
	9150 1900 9150 1450
Wire Wire Line
	9400 2500 8900 2500
Wire Wire Line
	9400 2600 8900 2600
$Comp
L Connector_Generic:Conn_01x10 P3
U 1 1 56D721E0
P 10000 1600
F 0 "P3" H 10000 2150 50  0000 C CNN
F 1 "Digital" V 10100 1600 50  0000 C CNN
F 2 "Socket_Arduino_Uno:Socket_Strip_Arduino_1x10" V 10150 1600 20  0000 C CNN
F 3 "" H 10000 1600 50  0000 C CNN
	1    10000 1600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10200 2100 10550 2100
Wire Wire Line
	10200 2000 10550 2000
Wire Wire Line
	10200 1900 10550 1900
Wire Wire Line
	10200 1800 10550 1800
Wire Wire Line
	10200 1700 10550 1700
Wire Wire Line
	10200 1600 10550 1600
Wire Wire Line
	10200 1400 10550 1400
Wire Wire Line
	10200 1300 10550 1300
Wire Wire Line
	10200 1200 10550 1200
Wire Wire Line
	10200 3000 10550 3000
Wire Wire Line
	10200 2900 10550 2900
Wire Wire Line
	10200 2800 10550 2800
Wire Wire Line
	10200 2700 10550 2700
Wire Wire Line
	10200 2600 10550 2600
Wire Wire Line
	10200 2500 10550 2500
Wire Wire Line
	10200 2400 10550 2400
Wire Wire Line
	10200 2300 10550 2300
Wire Wire Line
	10200 1500 10300 1500
Wire Wire Line
	10300 1500 10300 3150
Wire Wire Line
	9300 2100 9300 2200
Wire Wire Line
	9300 2200 9300 3150
Wire Notes Line
	8500 500  8500 3450
Wire Notes Line
	8500 3450 11200 3450
Wire Wire Line
	9400 1800 8650 1800
Text Notes 9700 1600 0    60   ~ 0
1
Wire Notes Line
	11200 1000 10700 1000
Wire Notes Line
	10700 1000 10700 500 
$Comp
L Connector_Generic:Conn_02x10_Odd_Even J1
U 1 1 5F823CC8
P 4150 3150
F 0 "J1" H 4200 3767 50  0000 C CNN
F 1 "Camera" H 4200 3676 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x10_P2.54mm_Vertical" H 4150 3150 50  0001 C CNN
F 3 "~" H 4150 3150 50  0001 C CNN
	1    4150 3150
	1    0    0    -1  
$EndComp
Text Label 3950 2750 2    50   ~ 0
GND
Text Label 3950 3250 2    50   ~ 0
GND
Text Label 3950 3450 2    50   ~ 0
GND
Text Label 3950 3550 2    50   ~ 0
GND
Text Label 4450 3450 0    50   ~ 0
VCC_3V3
Text Label 3950 2850 2    50   ~ 0
DATA_RDY
Text Label 3950 2950 2    50   ~ 0
SHUTTER
Text Label 3950 3150 2    50   ~ 0
VIDEO_N
Text Label 4450 3050 0    50   ~ 0
VIDEO_P
Text Label 3950 3050 2    50   ~ 0
GND_OA
Text Label 4450 3150 0    50   ~ 0
VDD_OA
Text Label 4450 2750 0    50   ~ 0
PWR_DOWN
Text Label 4450 2850 0    50   ~ 0
CLR_DATA
Text Label 4450 2950 0    50   ~ 0
CLR_PIX
Text Label 4450 3250 0    50   ~ 0
ADC_CLK
Text Label 4450 3350 0    50   ~ 0
ADC_CS
Text Label 3950 3350 2    50   ~ 0
ADC_DATA
Text Label 4450 3550 0    50   ~ 0
READ
Text Label 4450 3650 0    50   ~ 0
SCL
Text Label 3950 3650 2    50   ~ 0
SDA
$Comp
L power:GND #PWR0101
U 1 1 5F826F02
P 4500 5600
F 0 "#PWR0101" H 4500 5350 50  0001 C CNN
F 1 "GND" H 4505 5427 50  0000 C CNN
F 2 "" H 4500 5600 50  0001 C CNN
F 3 "" H 4500 5600 50  0001 C CNN
	1    4500 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0102
U 1 1 5F827477
P 4100 5600
F 0 "#PWR0102" H 4100 5350 50  0001 C CNN
F 1 "GNDA" H 4105 5427 50  0000 C CNN
F 2 "" H 4100 5600 50  0001 C CNN
F 3 "" H 4100 5600 50  0001 C CNN
	1    4100 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NO_Small JP2
U 1 1 5F82808C
P 4300 5550
F 0 "JP2" H 4300 5735 50  0000 C CNN
F 1 "Jumper" H 4300 5644 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 5550 50  0001 C CNN
F 3 "~" H 4300 5550 50  0001 C CNN
	1    4300 5550
	1    0    0    -1  
$EndComp
Text Notes 4000 5250 0    50   ~ 0
RevA breakout only:\nStar ground\nopen if closed on\nbreakout board
Wire Wire Line
	4100 5600 4100 5550
Wire Wire Line
	4100 5550 4200 5550
Wire Wire Line
	4500 5550 4500 5600
Wire Wire Line
	4400 5550 4500 5550
NoConn ~ 10550 1800
NoConn ~ 10550 2100
NoConn ~ 9400 2700
NoConn ~ 9400 2800
NoConn ~ 9400 2900
NoConn ~ 9400 3000
$Comp
L power:+5V #PWR0103
U 1 1 5F839562
P 5350 1500
F 0 "#PWR0103" H 5350 1350 50  0001 C CNN
F 1 "+5V" V 5350 1700 50  0000 C CNN
F 2 "" H 5350 1500 50  0000 C CNN
F 3 "" H 5350 1500 50  0000 C CNN
	1    5350 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5F83A4AC
P 6200 2350
F 0 "#PWR0104" H 6200 2100 50  0001 C CNN
F 1 "GND" H 6205 2177 50  0000 C CNN
F 2 "" H 6200 2350 50  0001 C CNN
F 3 "" H 6200 2350 50  0001 C CNN
	1    6200 2350
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_3_Bridged12 JP1
U 1 1 5F83A8B0
P 7050 1550
F 0 "JP1" V 7004 1617 50  0000 L CNN
F 1 "Jumper" V 7095 1617 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 7050 1550 50  0001 C CNN
F 3 "~" H 7050 1550 50  0001 C CNN
	1    7050 1550
	0    -1   1    0   
$EndComp
Text Label 7300 1550 0    50   ~ 0
VCC_3V3
Text Label 3950 5550 2    50   ~ 0
GND_OA
Wire Wire Line
	3950 5550 4100 5550
Connection ~ 4100 5550
$Comp
L Device:Jumper_NO_Small JP3
U 1 1 5F83D9C5
P 4300 6900
F 0 "JP3" H 4300 7085 50  0000 C CNN
F 1 "Jumper" H 4300 6994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 6900 50  0001 C CNN
F 3 "~" H 4300 6900 50  0001 C CNN
	1    4300 6900
	1    0    0    -1  
$EndComp
Text Label 3950 6900 2    50   ~ 0
VDD_OA
Wire Wire Line
	3950 6900 4200 6900
Wire Wire Line
	4400 6900 4650 6900
Text Label 4650 6900 0    50   ~ 0
AVDD
$Comp
L Device:Jumper_NO_Small JP4
U 1 1 5F83FE30
P 4300 7200
F 0 "JP4" H 4300 7385 50  0000 C CNN
F 1 "Jumper" H 4300 7294 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 7200 50  0001 C CNN
F 3 "~" H 4300 7200 50  0001 C CNN
	1    4300 7200
	1    0    0    -1  
$EndComp
Text Label 3950 7200 2    50   ~ 0
GND_OA
Wire Wire Line
	3950 7200 4200 7200
Wire Wire Line
	4400 7200 4650 7200
Text Label 4650 7200 0    50   ~ 0
AVSS
Text Notes 3750 6600 0    50   ~ 0
RevB breakout only:\nAVDD and AVSS from STM32\nRemove jumpers on breakout
Wire Wire Line
	5350 1500 5350 1900
$Comp
L Device:C_Small C1
U 1 1 5F849329
P 5750 2150
F 0 "C1" H 5842 2196 50  0000 L CNN
F 1 "1uF" H 5842 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5750 2150 50  0001 C CNN
F 3 "~" H 5750 2150 50  0001 C CNN
	1    5750 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5F8498E6
P 6600 2150
F 0 "C2" H 6692 2196 50  0000 L CNN
F 1 "1uF" H 6692 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6600 2150 50  0001 C CNN
F 3 "~" H 6600 2150 50  0001 C CNN
	1    6600 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 1900 6900 1900
Wire Wire Line
	7200 1550 7300 1550
Wire Wire Line
	7050 1900 7050 1800
$Comp
L power:+3.3V #PWR0105
U 1 1 5F857FDE
P 7050 1200
F 0 "#PWR0105" H 7050 1050 50  0001 C CNN
F 1 "+3.3V" V 7050 1450 50  0000 C CNN
F 2 "" H 7050 1200 50  0000 C CNN
F 3 "" H 7050 1200 50  0000 C CNN
	1    7050 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 1200 7050 1300
$Comp
L Device:CP_Small C3
U 1 1 5F859DEA
P 5350 2150
F 0 "C3" H 5438 2196 50  0000 L CNN
F 1 "100uF" H 5438 2105 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 5350 2150 50  0001 C CNN
F 3 "~" H 5350 2150 50  0001 C CNN
	1    5350 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C4
U 1 1 5F85A5A8
P 6900 2150
F 0 "C4" H 6988 2196 50  0000 L CNN
F 1 "100uF" H 6988 2105 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 6900 2150 50  0001 C CNN
F 3 "~" H 6900 2150 50  0001 C CNN
	1    6900 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2050 6600 1900
Connection ~ 6600 1900
Wire Wire Line
	6600 1900 6500 1900
Wire Wire Line
	6900 2050 6900 1900
Connection ~ 6900 1900
Wire Wire Line
	6900 1900 6600 1900
Wire Wire Line
	5750 2050 5750 1900
Connection ~ 5750 1900
Wire Wire Line
	5350 2050 5350 1900
Connection ~ 5350 1900
Wire Wire Line
	5350 1900 5750 1900
$Comp
L power:GND #PWR0106
U 1 1 5F862BF0
P 6600 2350
F 0 "#PWR0106" H 6600 2100 50  0001 C CNN
F 1 "GND" H 6605 2177 50  0000 C CNN
F 2 "" H 6600 2350 50  0001 C CNN
F 3 "" H 6600 2350 50  0001 C CNN
	1    6600 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2250 6600 2350
$Comp
L power:GND #PWR0107
U 1 1 5F86717B
P 6900 2350
F 0 "#PWR0107" H 6900 2100 50  0001 C CNN
F 1 "GND" H 6905 2177 50  0000 C CNN
F 2 "" H 6900 2350 50  0001 C CNN
F 3 "" H 6900 2350 50  0001 C CNN
	1    6900 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 2350 6900 2250
$Comp
L power:GND #PWR0108
U 1 1 5F868ABE
P 5750 2350
F 0 "#PWR0108" H 5750 2100 50  0001 C CNN
F 1 "GND" H 5755 2177 50  0000 C CNN
F 2 "" H 5750 2350 50  0001 C CNN
F 3 "" H 5750 2350 50  0001 C CNN
	1    5750 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 2350 5750 2250
$Comp
L power:GND #PWR0109
U 1 1 5F86A6DB
P 5350 2350
F 0 "#PWR0109" H 5350 2100 50  0001 C CNN
F 1 "GND" H 5355 2177 50  0000 C CNN
F 2 "" H 5350 2350 50  0001 C CNN
F 3 "" H 5350 2350 50  0001 C CNN
	1    5350 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2350 5350 2250
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 5F86FF89
P 7250 3850
F 0 "J2" H 7330 3892 50  0000 L CNN
F 1 "Conn_01x05" H 7330 3801 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7250 3850 50  0001 C CNN
F 3 "~" H 7250 3850 50  0001 C CNN
	1    7250 3850
	1    0    0    -1  
$EndComp
Text Label 7050 3650 2    50   ~ 0
AVDD
Text Label 7050 3750 2    50   ~ 0
AVSS
Text Label 7050 3850 2    50   ~ 0
GND
Text Label 7050 3950 2    50   ~ 0
VIDEO_P
Text Label 7050 4050 2    50   ~ 0
VIDEO_N
NoConn ~ 10550 2300
NoConn ~ 10550 2400
NoConn ~ 10550 2500
Wire Wire Line
	5750 1900 5900 1900
$Comp
L Regulator_Linear:AP1117-33 U1
U 1 1 5F888340
P 6200 1900
F 0 "U1" H 6200 2142 50  0000 C CNN
F 1 "AP1117-33" H 6200 2051 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 6200 2100 50  0001 C CNN
F 3 "http://www.diodes.com/datasheets/AP1117.pdf" H 6300 1650 50  0001 C CNN
	1    6200 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2200 6200 2350
$EndSCHEMATC
