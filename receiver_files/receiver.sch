EESchema Schematic File Version 4
EELAYER 26 0
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
L promicro:ProMicro U1
U 1 1 5CCABC73
P 5450 3250
F 0 "U1" H 5450 4287 60  0000 C CNN
F 1 "ProMicro" H 5450 4181 60  0000 C CNN
F 2 "promicro:ProMicro" H 5550 2200 60  0001 C CNN
F 3 "" H 5550 2200 60  0000 C CNN
	1    5450 3250
	1    0    0    -1  
$EndComp
$Comp
L RF:NRF24L01_Breakout U2
U 1 1 5CCABD70
P 7700 2500
F 0 "U2" H 8178 2478 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 8178 2387 50  0000 L CNN
F 2 "RF_Module:NR24l01mini" H 7850 3100 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 7700 2400 50  0001 C CNN
	1    7700 2500
	1    0    0    -1  
$EndComp
$Comp
L RF:NRF24L01_Breakout U3
U 1 1 5CCABE77
P 7700 3950
F 0 "U3" H 8178 3928 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 8178 3837 50  0000 L CNN
F 2 "RF_Module:nRF24L01_Breakout" H 7850 4550 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 7700 3850 50  0001 C CNN
	1    7700 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 2200 7000 2200
Wire Wire Line
	7000 3650 7200 3650
Wire Wire Line
	7200 3750 6900 3750
Wire Wire Line
	6900 2300 7200 2300
Wire Wire Line
	7200 2400 6800 2400
Wire Wire Line
	6800 3850 7200 3850
Wire Wire Line
	7200 3950 6750 3950
Wire Wire Line
	6750 3950 6750 3900
Wire Wire Line
	6750 2500 7200 2500
Wire Wire Line
	7200 4150 6650 4150
Wire Wire Line
	6650 4150 6650 3650
Wire Wire Line
	6650 2700 7200 2700
Wire Wire Line
	7700 4550 8150 4550
Wire Wire Line
	8150 4550 8150 3350
Wire Wire Line
	8150 3100 7700 3100
$Comp
L AMS1117-3.3:AMS1117-3.3 U4
U 1 1 5CCAC4ED
P 6800 1600
F 0 "U4" H 6800 1233 50  0000 C CNN
F 1 "AMS1117-3.3" H 6800 1324 50  0000 C CNN
F 2 "AMS1117-3.3:SOT229P700X180-4N" H 6800 1600 50  0001 L BNN
F 3 "Sot223/Pkg 1-Amp 3.3-Volt Low Drop Out Voltage Regulatator" H 6800 1600 50  0001 L BNN
F 4 "Advanced Monolithic Systems" H 6800 1600 50  0001 L BNN "Field4"
F 5 "None" H 6800 1600 50  0001 L BNN "Field5"
F 6 "Unavailable" H 6800 1600 50  0001 L BNN "Field6"
F 7 "None" H 6800 1600 50  0001 L BNN "Field7"
F 8 "AMS1117-3.3" H 6800 1600 50  0001 L BNN "Field8"
	1    6800 1600
	-1   0    0    1   
$EndComp
Text Label 6150 2800 0    50   ~ 0
VCC1
Text Label 7400 1700 0    50   ~ 0
VCC1
Text Label 6200 1700 2    50   ~ 0
3.3V
Text Label 7700 1900 0    50   ~ 0
3.3V
Text Label 7700 3350 0    50   ~ 0
VCC1
Text Label 6150 2600 0    50   ~ 0
GND1
Text Label 6200 1500 2    50   ~ 0
GND1
Wire Wire Line
	8150 3350 8850 3350
Connection ~ 8150 3350
Wire Wire Line
	8150 3350 8150 3100
Text Label 8850 3350 0    50   ~ 0
GND1
Wire Wire Line
	6150 3500 7000 3500
Wire Wire Line
	7000 2200 7000 3500
Connection ~ 7000 3500
Wire Wire Line
	7000 3500 7000 3650
Wire Wire Line
	6150 3400 6900 3400
Wire Wire Line
	6900 2300 6900 3400
Connection ~ 6900 3400
Wire Wire Line
	6900 3400 6900 3750
Wire Wire Line
	6150 3300 6800 3300
Wire Wire Line
	6800 2400 6800 3300
Connection ~ 6800 3300
Wire Wire Line
	6800 3300 6800 3850
Wire Wire Line
	6650 3650 6150 3650
Wire Wire Line
	6150 3650 6150 3600
Connection ~ 6650 3650
Wire Wire Line
	6650 3650 6650 2700
Wire Wire Line
	6750 3900 4700 3900
Wire Wire Line
	4700 3900 4700 3600
Wire Wire Line
	4700 3600 4750 3600
Connection ~ 6750 3900
Wire Wire Line
	6750 3900 6750 2500
$EndSCHEMATC
