EESchema Schematic File Version 4
LIBS:waterpump-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR01
U 1 1 5D46C571
P 1000 1600
F 0 "#PWR01" H 1000 1350 50  0001 C CNN
F 1 "GND" H 1005 1427 50  0000 C CNN
F 2 "" H 1000 1600 50  0001 C CNN
F 3 "" H 1000 1600 50  0001 C CNN
	1    1000 1600
	1    0    0    -1  
$EndComp
$Comp
L MCU_ST_STM32L0:STM32L051K6Tx U2
U 1 1 5D46CB6D
P 3600 3500
F 0 "U2" H 3750 2500 50  0000 C CNN
F 1 "STM32L051K6Tx" H 4150 2500 50  0000 C CNN
F 2 "Package_QFP:LQFP-32_7x7mm_P0.8mm" H 3100 2600 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00108219.pdf" H 3600 3500 50  0001 C CNN
	1    3600 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5D46CC3C
P 4700 2900
F 0 "D1" H 4691 3116 50  0000 C CNN
F 1 "LED" H 4691 3025 50  0000 C CNN
F 2 "" H 4700 2900 50  0001 C CNN
F 3 "~" H 4700 2900 50  0001 C CNN
	1    4700 2900
	-1   0    0    1   
$EndComp
$Comp
L Device:R R8
U 1 1 5D46CCAD
P 4350 2900
F 0 "R8" V 4143 2900 50  0000 C CNN
F 1 "330" V 4234 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4280 2900 50  0001 C CNN
F 3 "~" H 4350 2900 50  0001 C CNN
	1    4350 2900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5D46D334
P 3500 4750
F 0 "#PWR015" H 3500 4500 50  0001 C CNN
F 1 "GND" H 3505 4577 50  0000 C CNN
F 2 "" H 3500 4750 50  0001 C CNN
F 3 "" H 3500 4750 50  0001 C CNN
	1    3500 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4500 3500 4600
Wire Wire Line
	3500 4600 3600 4600
Wire Wire Line
	3600 4600 3600 4500
Connection ~ 3500 4600
Wire Wire Line
	3500 4600 3500 4750
$Comp
L power:+3.3V #PWR014
U 1 1 5D46D506
P 3500 2350
F 0 "#PWR014" H 3500 2200 50  0001 C CNN
F 1 "+3.3V" H 3515 2523 50  0000 C CNN
F 2 "" H 3500 2350 50  0001 C CNN
F 3 "" H 3500 2350 50  0001 C CNN
	1    3500 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2600 3500 2450
Wire Wire Line
	3500 2450 3600 2450
Wire Wire Line
	3600 2450 3600 2600
Connection ~ 3500 2450
Wire Wire Line
	3500 2450 3500 2350
Wire Wire Line
	3600 2450 3700 2450
Wire Wire Line
	3700 2450 3700 2600
Connection ~ 3600 2450
$Comp
L power:GND #PWR010
U 1 1 5D46D8F9
P 2750 3000
F 0 "#PWR010" H 2750 2750 50  0001 C CNN
F 1 "GND" H 2755 2827 50  0000 C CNN
F 2 "" H 2750 3000 50  0001 C CNN
F 3 "" H 2750 3000 50  0001 C CNN
	1    2750 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3000 3000 3000
Wire Wire Line
	3200 900  3050 900 
$Comp
L power:+3.3V #PWR018
U 1 1 5D46E34B
P 4200 900
F 0 "#PWR018" H 4200 750 50  0001 C CNN
F 1 "+3.3V" H 4215 1073 50  0000 C CNN
F 2 "" H 4200 900 50  0001 C CNN
F 3 "" H 4200 900 50  0001 C CNN
	1    4200 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 900  3900 900 
$Comp
L power:GND #PWR013
U 1 1 5D46E4F4
P 3500 1400
F 0 "#PWR013" H 3500 1150 50  0001 C CNN
F 1 "GND" H 3505 1227 50  0000 C CNN
F 2 "" H 3500 1400 50  0001 C CNN
F 3 "" H 3500 1400 50  0001 C CNN
	1    3500 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 1200 3500 1300
$Comp
L Device:C C6
U 1 1 5D46E705
P 3050 1150
F 0 "C6" H 3165 1196 50  0000 L CNN
F 1 "1uF" H 3165 1105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3088 1000 50  0001 C CNN
F 3 "~" H 3050 1150 50  0001 C CNN
	1    3050 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5D46E769
P 3900 1150
F 0 "C7" H 4015 1196 50  0000 L CNN
F 1 "1uF" H 4015 1105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3938 1000 50  0001 C CNN
F 3 "~" H 3900 1150 50  0001 C CNN
	1    3900 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1000 3900 900 
Connection ~ 3900 900 
Wire Wire Line
	3900 900  4200 900 
Wire Wire Line
	3500 1300 3900 1300
Connection ~ 3500 1300
Wire Wire Line
	3500 1300 3500 1400
Wire Wire Line
	3050 1000 3050 900 
Connection ~ 3050 900 
Wire Wire Line
	3050 1300 3500 1300
Wire Wire Line
	4500 2900 4550 2900
$Comp
L Connector:USB_A J2
U 1 1 5D47289C
P 1400 6100
F 0 "J2" H 1455 6567 50  0000 C CNN
F 1 "USB_A" H 1455 6476 50  0000 C CNN
F 2 "Connector_USB:USB_A_CNCTech_1001-011-01101_Horizontal" H 1550 6050 50  0001 C CNN
F 3 " ~" H 1550 6050 50  0001 C CNN
	1    1400 6100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR08
U 1 1 5D472A90
P 1900 5850
F 0 "#PWR08" H 1900 5700 50  0001 C CNN
F 1 "+5V" H 1915 6023 50  0000 C CNN
F 2 "" H 1900 5850 50  0001 C CNN
F 3 "" H 1900 5850 50  0001 C CNN
	1    1900 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5900 1900 5900
Wire Wire Line
	1900 5900 1900 5850
NoConn ~ 1700 6100
NoConn ~ 1700 6200
$Comp
L power:GND #PWR04
U 1 1 5D4737B1
P 1300 7100
F 0 "#PWR04" H 1300 6850 50  0001 C CNN
F 1 "GND" H 1305 6927 50  0000 C CNN
F 2 "" H 1300 7100 50  0001 C CNN
F 3 "" H 1300 7100 50  0001 C CNN
	1    1300 7100
	1    0    0    -1  
$EndComp
Text Label 4100 3900 0    50   ~ 0
ENABLE_PUMP1
$Comp
L Switch:SW_Push SW1
U 1 1 5D47AB49
P 1050 4250
F 0 "SW1" H 1050 4535 50  0000 C CNN
F 1 "SW_Push" H 1050 4444 50  0000 C CNN
F 2 "" H 1050 4450 50  0001 C CNN
F 3 "" H 1050 4450 50  0001 C CNN
	1    1050 4250
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR02
U 1 1 5D47BDD3
P 1300 3450
F 0 "#PWR02" H 1300 3300 50  0001 C CNN
F 1 "+3.3V" H 1315 3623 50  0000 C CNN
F 2 "" H 1300 3450 50  0001 C CNN
F 3 "" H 1300 3450 50  0001 C CNN
	1    1300 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5D47C6C2
P 1300 4500
F 0 "R2" H 1230 4454 50  0000 R CNN
F 1 "100" H 1230 4545 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1230 4500 50  0001 C CNN
F 3 "~" H 1300 4500 50  0001 C CNN
	1    1300 4500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5D47D7D1
P 1300 3700
F 0 "R1" V 1507 3700 50  0000 C CNN
F 1 "4k7" V 1416 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1230 3700 50  0001 C CNN
F 3 "~" H 1300 3700 50  0001 C CNN
	1    1300 3700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5D47E925
P 1300 4700
F 0 "#PWR03" H 1300 4450 50  0001 C CNN
F 1 "GND" H 1305 4527 50  0000 C CNN
F 2 "" H 1300 4700 50  0001 C CNN
F 3 "" H 1300 4700 50  0001 C CNN
	1    1300 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 4650 1300 4700
Text Label 1550 3850 0    50   ~ 0
REQ_WATER_PLANT
$Comp
L Device:C C3
U 1 1 5D48109B
P 1300 4100
F 0 "C3" H 1415 4146 50  0000 L CNN
F 1 "100nF" H 1415 4055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 1338 3950 50  0001 C CNN
F 3 "~" H 1300 4100 50  0001 C CNN
	1    1300 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 4250 1300 4350
$Comp
L Device:C C8
U 1 1 5D47CC0B
P 5200 1350
F 0 "C8" H 5315 1396 50  0000 L CNN
F 1 "1uF" H 5315 1305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 5238 1200 50  0001 C CNN
F 3 "~" H 5200 1350 50  0001 C CNN
	1    5200 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5D493F9F
P 7550 1550
F 0 "#PWR027" H 7550 1300 50  0001 C CNN
F 1 "GND" H 7555 1377 50  0000 C CNN
F 2 "" H 7550 1550 50  0001 C CNN
F 3 "" H 7550 1550 50  0001 C CNN
	1    7550 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 1550 7550 1450
$Comp
L power:+4V #PWR028
U 1 1 5D4954D1
P 7850 1100
F 0 "#PWR028" H 7850 950 50  0001 C CNN
F 1 "+4V" H 7865 1273 50  0000 C CNN
F 2 "" H 7850 1100 50  0001 C CNN
F 3 "" H 7850 1100 50  0001 C CNN
	1    7850 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 900  3050 900 
$Comp
L power:GND #PWR022
U 1 1 5D49502F
P 5200 1600
F 0 "#PWR022" H 5200 1350 50  0001 C CNN
F 1 "GND" H 5205 1427 50  0000 C CNN
F 2 "" H 5200 1600 50  0001 C CNN
F 3 "" H 5200 1600 50  0001 C CNN
	1    5200 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 1500 5200 1600
$Comp
L Device:C C9
U 1 1 5D499037
P 7550 1300
F 0 "C9" H 7665 1346 50  0000 L CNN
F 1 "47uF" H 7665 1255 50  0000 L CNN
F 2 "" H 7588 1150 50  0001 C CNN
F 3 "~" H 7550 1300 50  0001 C CNN
	1    7550 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1100 7550 1100
Wire Wire Line
	7550 1150 7550 1100
$Comp
L waterpump-rescue:Conn_ARM_JTAG_SWD_10-Connector-waterpump-rescue-waterpump-rescue J4
U 1 1 5D4AA481
P 10000 1550
F 0 "J4" H 9560 1596 50  0000 R CNN
F 1 "Conn_ARM_JTAG_SWD_10" H 9560 1505 50  0000 R CNN
F 2 "Connector_PinHeader_1.27mm:PinHeader_2x05_P1.27mm_Vertical_SMD" H 10050 1000 50  0001 L TNN
F 3 "http://infocenter.arm.com/help/topic/com.arm.doc.faqs/attached/13634/cortex_debug_connectors.pdf" V 9650 300 50  0001 C CNN
	1    10000 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR032
U 1 1 5D4AA54D
P 10000 900
F 0 "#PWR032" H 10000 750 50  0001 C CNN
F 1 "+3.3V" H 10015 1073 50  0000 C CNN
F 2 "" H 10000 900 50  0001 C CNN
F 3 "" H 10000 900 50  0001 C CNN
	1    10000 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 950  10000 900 
$Comp
L power:GND #PWR031
U 1 1 5D4AB5DB
P 9900 2250
F 0 "#PWR031" H 9900 2000 50  0001 C CNN
F 1 "GND" H 9905 2077 50  0000 C CNN
F 2 "" H 9900 2250 50  0001 C CNN
F 3 "" H 9900 2250 50  0001 C CNN
	1    9900 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 2150 9900 2150
Wire Wire Line
	9900 2150 9900 2250
NoConn ~ 10500 1750
NoConn ~ 10500 1650
Text Label 10500 1250 0    50   ~ 0
SWD_RESET
Text Label 10500 1450 0    50   ~ 0
SWD_CLK
Text Label 10500 1550 0    50   ~ 0
SWD_IO
Text Label 2850 2800 2    50   ~ 0
SWD_RESET
Wire Wire Line
	3000 2800 2950 2800
$Comp
L Device:R R6
U 1 1 5D4B236D
P 2950 2500
F 0 "R6" H 3100 2450 50  0000 C CNN
F 1 "10k" H 3100 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2880 2500 50  0001 C CNN
F 3 "~" H 2950 2500 50  0001 C CNN
	1    2950 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2650 2950 2800
Connection ~ 2950 2800
Wire Wire Line
	2950 2800 2850 2800
$Comp
L power:+3.3V #PWR012
U 1 1 5D4B3660
P 2950 2300
F 0 "#PWR012" H 2950 2150 50  0001 C CNN
F 1 "+3.3V" H 2965 2473 50  0000 C CNN
F 2 "" H 2950 2300 50  0001 C CNN
F 3 "" H 2950 2300 50  0001 C CNN
	1    2950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2300 2950 2350
$Comp
L Device:C C4
U 1 1 5D4B4D72
P 1700 2450
F 0 "C4" H 1815 2496 50  0000 L CNN
F 1 "100nF" H 1815 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 1738 2300 50  0001 C CNN
F 3 "~" H 1700 2450 50  0001 C CNN
	1    1700 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5D4B4E22
P 1250 2450
F 0 "C2" H 1365 2496 50  0000 L CNN
F 1 "100nF" H 1365 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 1288 2300 50  0001 C CNN
F 3 "~" H 1250 2450 50  0001 C CNN
	1    1250 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5D4B4E78
P 850 2450
F 0 "C1" H 965 2496 50  0000 L CNN
F 1 "100nF" H 965 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 888 2300 50  0001 C CNN
F 3 "~" H 850 2450 50  0001 C CNN
	1    850  2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5D4BCA03
P 1300 6700
F 0 "R3" H 1500 6650 50  0000 R CNN
F 1 "0" H 1450 6750 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1230 6700 50  0001 C CNN
F 3 "~" H 1300 6700 50  0001 C CNN
	1    1300 6700
	-1   0    0    1   
$EndComp
Text Label 3000 4200 2    50   ~ 0
GSM_TX
Text Label 3000 4300 2    50   ~ 0
GSM_RX
Text Label 4100 4200 0    50   ~ 0
SWD_CLK
Text Label 4100 4100 0    50   ~ 0
SWD_IO
$Comp
L power:GND #PWR020
U 1 1 5D4CC72A
P 5100 2900
F 0 "#PWR020" H 5100 2650 50  0001 C CNN
F 1 "GND" H 5105 2727 50  0000 C CNN
F 2 "" H 5100 2900 50  0001 C CNN
F 3 "" H 5100 2900 50  0001 C CNN
	1    5100 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2900 4850 2900
Text Label 4100 4000 0    50   ~ 0
REQ_WATER_PLANT
NoConn ~ 4100 2800
NoConn ~ 4100 3000
NoConn ~ 4100 3100
NoConn ~ 4100 3200
NoConn ~ 4100 3300
NoConn ~ 4100 3400
NoConn ~ 4100 3500
NoConn ~ 4100 3600
NoConn ~ 3000 3400
NoConn ~ 3000 3500
NoConn ~ 3000 3700
NoConn ~ 3000 3800
NoConn ~ 3000 3900
$Comp
L Device:C C5
U 1 1 5D4F12F9
P 2150 2450
F 0 "C5" H 2265 2496 50  0000 L CNN
F 1 "1uF" H 2265 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 2188 2300 50  0001 C CNN
F 3 "~" H 2150 2450 50  0001 C CNN
	1    2150 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2300 1250 2300
Connection ~ 1250 2300
Wire Wire Line
	1250 2300 1450 2300
Connection ~ 1700 2300
Wire Wire Line
	1700 2300 2150 2300
$Comp
L power:+3.3V #PWR06
U 1 1 5D4F2C94
P 1450 2100
F 0 "#PWR06" H 1450 1950 50  0001 C CNN
F 1 "+3.3V" H 1465 2273 50  0000 C CNN
F 2 "" H 1450 2100 50  0001 C CNN
F 3 "" H 1450 2100 50  0001 C CNN
	1    1450 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 2100 1450 2300
Connection ~ 1450 2300
Wire Wire Line
	1450 2300 1700 2300
Wire Wire Line
	850  2600 1250 2600
Connection ~ 1250 2600
Wire Wire Line
	1250 2600 1500 2600
Connection ~ 1700 2600
Wire Wire Line
	1700 2600 2150 2600
$Comp
L power:GND #PWR07
U 1 1 5D4F5BF2
P 1500 2750
F 0 "#PWR07" H 1500 2500 50  0001 C CNN
F 1 "GND" H 1505 2577 50  0000 C CNN
F 2 "" H 1500 2750 50  0001 C CNN
F 3 "" H 1500 2750 50  0001 C CNN
	1    1500 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 2750 1500 2600
Connection ~ 1500 2600
Wire Wire Line
	1500 2600 1700 2600
NoConn ~ 4100 4300
Wire Wire Line
	4200 2900 4100 2900
Wire Wire Line
	1300 3450 1300 3550
Wire Wire Line
	1050 3850 1300 3850
Connection ~ 1300 3850
Wire Wire Line
	1300 3850 1550 3850
Wire Wire Line
	1300 3850 1300 3950
Wire Wire Line
	1050 3850 1050 4050
Wire Wire Line
	1050 4450 1050 4650
Wire Wire Line
	1050 4650 1300 4650
Connection ~ 1300 4650
Wire Wire Line
	5200 1050 5200 1100
Text Label 4100 3700 0    50   ~ 0
ENABLE_GSM
$Comp
L power:+5V #PWR05
U 1 1 5D531259
P 1450 850
F 0 "#PWR05" H 1450 700 50  0001 C CNN
F 1 "+5V" H 1465 1023 50  0000 C CNN
F 2 "" H 1450 850 50  0001 C CNN
F 3 "" H 1450 850 50  0001 C CNN
	1    1450 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR021
U 1 1 5D537A80
P 5200 1050
F 0 "#PWR021" H 5200 900 50  0001 C CNN
F 1 "+5V" H 5215 1223 50  0000 C CNN
F 2 "" H 5200 1050 50  0001 C CNN
F 3 "" H 5200 1050 50  0001 C CNN
	1    5200 1050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 5D53A472
P 2800 900
F 0 "#PWR011" H 2800 750 50  0001 C CNN
F 1 "+5V" H 2815 1073 50  0000 C CNN
F 2 "" H 2800 900 50  0001 C CNN
F 3 "" H 2800 900 50  0001 C CNN
	1    2800 900 
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:MCP1700-3302E_SOT23 U1
U 1 1 5D53A818
P 3500 900
F 0 "U1" H 3500 1142 50  0000 C CNN
F 1 "MCP1700-3302E_SOT23" H 3500 1051 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3500 1125 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001826C.pdf" H 3500 900 50  0001 C CNN
	1    3500 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5D55423D
P 6350 1800
F 0 "R13" H 6500 1750 50  0000 C CNN
F 1 "1k" H 6500 1850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6280 1800 50  0001 C CNN
F 3 "~" H 6350 1800 50  0001 C CNN
	1    6350 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5D554319
P 7050 1400
F 0 "R14" H 7200 1350 50  0000 C CNN
F 1 "2.2k" H 7200 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6980 1400 50  0001 C CNN
F 3 "~" H 7050 1400 50  0001 C CNN
	1    7050 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5D558405
P 6350 2000
F 0 "#PWR024" H 6350 1750 50  0001 C CNN
F 1 "GND" H 6355 1827 50  0000 C CNN
F 2 "" H 6350 2000 50  0001 C CNN
F 3 "" H 6350 2000 50  0001 C CNN
	1    6350 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 1650 7050 1650
Wire Wire Line
	7050 1650 7050 1550
Connection ~ 7550 1100
$Comp
L Transistor_FET:IRLB8721PBF Q1
U 1 1 5D5623FD
P 1700 6800
F 0 "Q1" H 1550 6700 50  0000 L CNN
F 1 "IRLB8748PBF" H 1250 6600 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 1950 6725 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/irlb8721pbf.pdf?fileId=5546d462533600a40153566056732591" H 1700 6800 50  0001 L CNN
	1    1700 6800
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5D567BA2
P 2150 6800
F 0 "R4" V 2350 6800 50  0000 R CNN
F 1 "100" V 2250 6850 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2080 6800 50  0001 C CNN
F 3 "~" H 2150 6800 50  0001 C CNN
	1    2150 6800
	0    -1   -1   0   
$EndComp
Text Label 2550 6800 0    50   ~ 0
ENABLE_PUMP1
Wire Wire Line
	1900 6800 2000 6800
Wire Wire Line
	2300 6800 2450 6800
Wire Wire Line
	1600 7050 1600 7000
Wire Wire Line
	1300 6500 1300 6550
Wire Wire Line
	1400 6500 1600 6500
Wire Wire Line
	1600 6500 1600 6600
Wire Wire Line
	1300 6850 1300 7050
Wire Wire Line
	1600 7050 1300 7050
Connection ~ 1300 7050
Wire Wire Line
	1300 7050 1300 7100
Text Label 4100 3800 0    50   ~ 0
ENABLE_PUMP2
$Comp
L Connector:USB_A J3
U 1 1 5D5668DD
P 3650 6100
F 0 "J3" H 3705 6567 50  0000 C CNN
F 1 "USB_A" H 3705 6476 50  0000 C CNN
F 2 "Connector_USB:USB_A_CNCTech_1001-011-01101_Horizontal" H 3800 6050 50  0001 C CNN
F 3 " ~" H 3800 6050 50  0001 C CNN
	1    3650 6100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 5D5668E4
P 4150 5850
F 0 "#PWR017" H 4150 5700 50  0001 C CNN
F 1 "+5V" H 4165 6023 50  0000 C CNN
F 2 "" H 4150 5850 50  0001 C CNN
F 3 "" H 4150 5850 50  0001 C CNN
	1    4150 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 5900 4150 5900
Wire Wire Line
	4150 5900 4150 5850
NoConn ~ 3950 6100
NoConn ~ 3950 6200
$Comp
L power:GND #PWR016
U 1 1 5D5668EE
P 3550 7100
F 0 "#PWR016" H 3550 6850 50  0001 C CNN
F 1 "GND" H 3555 6927 50  0000 C CNN
F 2 "" H 3550 7100 50  0001 C CNN
F 3 "" H 3550 7100 50  0001 C CNN
	1    3550 7100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5D5668F4
P 3550 6700
F 0 "R7" H 3750 6650 50  0000 R CNN
F 1 "0" H 3700 6750 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3480 6700 50  0001 C CNN
F 3 "~" H 3550 6700 50  0001 C CNN
	1    3550 6700
	-1   0    0    1   
$EndComp
$Comp
L Transistor_FET:IRLB8721PBF Q2
U 1 1 5D5668FB
P 3950 6800
F 0 "Q2" H 3800 6700 50  0000 L CNN
F 1 "IRLB8748PBF" H 3500 6600 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4200 6725 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/irlb8721pbf.pdf?fileId=5546d462533600a40153566056732591" H 3950 6800 50  0001 L CNN
	1    3950 6800
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5D566902
P 4400 6800
F 0 "R9" V 4600 6800 50  0000 R CNN
F 1 "100" V 4500 6850 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4330 6800 50  0001 C CNN
F 3 "~" H 4400 6800 50  0001 C CNN
	1    4400 6800
	0    -1   -1   0   
$EndComp
Text Label 4950 6800 0    50   ~ 0
ENABLE_PUMP2
Wire Wire Line
	4150 6800 4250 6800
Wire Wire Line
	3850 7050 3850 7000
Wire Wire Line
	3550 6500 3550 6550
Wire Wire Line
	3650 6500 3850 6500
Wire Wire Line
	3850 6500 3850 6600
Wire Wire Line
	3550 6850 3550 7050
Wire Wire Line
	3850 7050 3550 7050
Connection ~ 3550 7050
Wire Wire Line
	3550 7050 3550 7100
$Comp
L Device:R R5
U 1 1 5D57111A
P 2450 7050
F 0 "R5" H 2650 7000 50  0000 R CNN
F 1 "10k" H 2650 7100 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2380 7050 50  0001 C CNN
F 3 "~" H 2450 7050 50  0001 C CNN
	1    2450 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 6800 2450 6900
$Comp
L power:GND #PWR09
U 1 1 5D5771C3
P 2450 7300
F 0 "#PWR09" H 2450 7050 50  0001 C CNN
F 1 "GND" H 2455 7127 50  0000 C CNN
F 2 "" H 2450 7300 50  0001 C CNN
F 3 "" H 2450 7300 50  0001 C CNN
	1    2450 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 7300 2450 7200
$Comp
L Device:R R10
U 1 1 5D5795FC
P 4700 7050
F 0 "R10" H 4900 7000 50  0000 R CNN
F 1 "10k" H 4900 7100 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4630 7050 50  0001 C CNN
F 3 "~" H 4700 7050 50  0001 C CNN
	1    4700 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 6800 2450 6800
Connection ~ 2450 6800
Wire Wire Line
	4550 6800 4700 6800
Wire Wire Line
	4700 6900 4700 6800
Connection ~ 4700 6800
Wire Wire Line
	4700 6800 4950 6800
$Comp
L power:GND #PWR019
U 1 1 5D57FE4E
P 4700 7300
F 0 "#PWR019" H 4700 7050 50  0001 C CNN
F 1 "GND" H 4705 7127 50  0000 C CNN
F 2 "" H 4700 7300 50  0001 C CNN
F 3 "" H 4700 7300 50  0001 C CNN
	1    4700 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 7300 4700 7200
Connection ~ 9900 2150
$Comp
L MIC37XXX:MIC37302 U3
U 1 1 5D56B087
P 6250 1850
F 0 "U3" H 6350 2900 50  0000 L CNN
F 1 "MIC37302" H 6500 2900 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-263-5_TabPin3" H 6250 1850 50  0001 C CNN
F 3 "" H 6250 1850 50  0001 C CNN
	1    6250 1850
	1    0    0    -1  
$EndComp
Connection ~ 5200 1100
Wire Wire Line
	5200 1100 5200 1200
$Comp
L power:GND #PWR023
U 1 1 5D57AF25
P 6150 1650
F 0 "#PWR023" H 6150 1400 50  0001 C CNN
F 1 "GND" H 6155 1477 50  0000 C CNN
F 2 "" H 6150 1650 50  0001 C CNN
F 3 "" H 6150 1650 50  0001 C CNN
	1    6150 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 1650 6150 1550
Wire Wire Line
	6350 1950 6350 2000
Wire Wire Line
	6350 1650 6350 1500
Connection ~ 6350 1650
Wire Wire Line
	6800 1100 7050 1100
Wire Wire Line
	7050 1250 7050 1100
Connection ~ 7050 1100
Wire Wire Line
	7050 1100 7550 1100
Text Label 6250 650  2    50   ~ 0
ENABLE_GSM
Wire Wire Line
	6250 800  6250 700 
$Comp
L SIM800L:SIM800L U4
U 1 1 5D5A6186
P 7250 3900
F 0 "U4" H 7250 4775 50  0000 C CNN
F 1 "SIM800L" H 7250 4684 50  0000 C CNN
F 2 "SIM800L:SIM800L" H 7250 3900 50  0001 C CNN
F 3 "" H 7250 3900 50  0001 C CNN
	1    7250 3900
	1    0    0    -1  
$EndComp
NoConn ~ 6850 3250
NoConn ~ 7650 3250
NoConn ~ 7650 3450
NoConn ~ 7650 3550
NoConn ~ 7650 3650
NoConn ~ 7650 3750
$Comp
L power:+4V #PWR025
U 1 1 5D5B48FA
P 6600 2950
F 0 "#PWR025" H 6600 2800 50  0001 C CNN
F 1 "+4V" H 6615 3123 50  0000 C CNN
F 2 "" H 6600 2950 50  0001 C CNN
F 3 "" H 6600 2950 50  0001 C CNN
	1    6600 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 5D5B70CE
P 6850 3900
F 0 "#PWR026" H 6850 3650 50  0001 C CNN
F 1 "GND" H 6855 3727 50  0000 C CNN
F 2 "" H 6850 3900 50  0001 C CNN
F 3 "" H 6850 3900 50  0001 C CNN
	1    6850 3900
	1    0    0    -1  
$EndComp
Text Label 6850 3550 2    50   ~ 0
GSM_TX
Text Label 6850 3650 2    50   ~ 0
GSM_RX
Text Label 6150 3450 2    50   ~ 0
GSM_RST
Text Label 8100 3350 0    50   ~ 0
GSM_DTR
Text Label 3000 4100 2    50   ~ 0
GSM_RST
Text Label 3000 4000 2    50   ~ 0
GSM_DTR
$Comp
L Device:R R12
U 1 1 5D5BCE9E
P 6300 3200
F 0 "R12" H 6150 3150 50  0000 C CNN
F 1 "10k" H 6150 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6230 3200 50  0001 C CNN
F 3 "~" H 6300 3200 50  0001 C CNN
	1    6300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3450 6300 3450
Wire Wire Line
	6150 3450 6300 3450
Connection ~ 6300 3450
$Comp
L Device:R R11
U 1 1 5D5D7A05
P 5600 850
F 0 "R11" H 5750 800 50  0000 C CNN
F 1 "10k" H 5750 900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5530 850 50  0001 C CNN
F 3 "~" H 5600 850 50  0001 C CNN
	1    5600 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1000 5600 1550
Wire Wire Line
	5600 1550 6150 1550
Connection ~ 6150 1550
Wire Wire Line
	6150 1550 6150 1500
Wire Wire Line
	5600 700  6250 700 
Connection ~ 6250 700 
Wire Wire Line
	6250 700  6250 650 
Wire Wire Line
	5200 1100 5800 1100
$Comp
L power:+4V #PWR029
U 1 1 5D5EAE89
P 9000 3150
F 0 "#PWR029" H 9000 3000 50  0001 C CNN
F 1 "+4V" H 9015 3323 50  0000 C CNN
F 2 "" H 9000 3150 50  0001 C CNN
F 3 "" H 9000 3150 50  0001 C CNN
	1    9000 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5D5EAEFA
P 9000 3450
F 0 "#PWR030" H 9000 3200 50  0001 C CNN
F 1 "GND" H 9005 3277 50  0000 C CNN
F 2 "" H 9000 3450 50  0001 C CNN
F 3 "" H 9000 3450 50  0001 C CNN
	1    9000 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 5D5EAF6B
P 9000 3300
F 0 "C10" H 9115 3346 50  0000 L CNN
F 1 "1uF" H 9115 3255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 9038 3150 50  0001 C CNN
F 3 "~" H 9000 3300 50  0001 C CNN
	1    9000 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3750 6850 3900
Wire Wire Line
	6300 3350 6300 3450
Wire Wire Line
	6600 3350 6850 3350
Wire Wire Line
	6600 3350 6600 3000
Wire Wire Line
	6300 3050 6300 3000
Wire Wire Line
	6300 3000 6600 3000
Connection ~ 6600 3000
Wire Wire Line
	6600 3000 6600 2950
$Comp
L Connector:USB_B_Mini J1
U 1 1 5D5FE7C1
P 1000 1050
F 0 "J1" H 1055 1517 50  0000 C CNN
F 1 "USB_B_Mini" H 1055 1426 50  0000 C CNN
F 2 "Connector_USB:USB_Mini-B_Lumberg_2486_01_Horizontal" H 1150 1000 50  0001 C CNN
F 3 "~" H 1150 1000 50  0001 C CNN
	1    1000 1050
	1    0    0    -1  
$EndComp
NoConn ~ 1300 1050
NoConn ~ 1300 1150
NoConn ~ 1300 1250
Wire Wire Line
	1450 850  1300 850 
Wire Wire Line
	1000 1450 1000 1600
Wire Wire Line
	900  1450 1000 1450
Connection ~ 1000 1450
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5D621D85
P 1950 6500
F 0 "#FLG0101" H 1950 6575 50  0001 C CNN
F 1 "PWR_FLAG" H 1950 6674 50  0000 C CNN
F 2 "" H 1950 6500 50  0001 C CNN
F 3 "~" H 1950 6500 50  0001 C CNN
	1    1950 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 6500 1950 6500
Connection ~ 1600 6500
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5D632243
P 4200 6500
F 0 "#FLG0102" H 4200 6575 50  0001 C CNN
F 1 "PWR_FLAG" H 4200 6674 50  0000 C CNN
F 2 "" H 4200 6500 50  0001 C CNN
F 3 "~" H 4200 6500 50  0001 C CNN
	1    4200 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 6500 3850 6500
Connection ~ 3850 6500
$Comp
L Device:R R15
U 1 1 5D582C66
P 7950 3200
F 0 "R15" H 7800 3150 50  0000 C CNN
F 1 "10k" H 7800 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7880 3200 50  0001 C CNN
F 3 "~" H 7950 3200 50  0001 C CNN
	1    7950 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 3350 7950 3350
Connection ~ 7950 3350
Wire Wire Line
	7950 3350 8100 3350
$Comp
L power:+4V #PWR0101
U 1 1 5D5861D2
P 7950 3000
F 0 "#PWR0101" H 7950 2850 50  0001 C CNN
F 1 "+4V" H 7965 3173 50  0000 C CNN
F 2 "" H 7950 3000 50  0001 C CNN
F 3 "" H 7950 3000 50  0001 C CNN
	1    7950 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 3000 7950 3050
$EndSCHEMATC
