# surfTX (arduino_surface_TX_4ch)

Forum Link : https://www.rcgroups.com/forums/showthread.php?3709477-surfTX-%28arduino_surface_TX_4ch%29#post45446587

## Overview


![surfTX Logo](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/logo2.png)

This project is another fork from https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control

The focus of this project is Making PISTOL type TX for surface vehicle such as car or boat.

This transmitter does not have its own communication module. Only PPM output is supported. A separate PPM module is required to use this transmitter. In my case, I use 4 in 1 multiprotocol module(https://github.com/pascallanger/DIY-Multiprotocol-TX-Module).

1. Add Model Name Feature (5 characters per model)
2. Increase the number of models(30 models)
3. Add Exponential Feature
4. Add EPA(End Point Adjustment) Feature
5. Modify Range and Center Calibration Feature
6. Modify PPM calculation logic (more suitable for surface vehicle)
7. Preventing EEPROM worn lifetime
8. Remove Throttle Cut Feature and Warning Screen
9. Modify Low Voltage Beep Pattern


And many of codes modified for surface vehicle.


## First Startup Procedures

This needs First Startup Procedures (almost same with Gabapentin's https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/wiki/5.-First-Startup-Procedures)

This has Two steps Calibrations. first is Range Calibaration. second one is center position Calibration. you need to press UP button to finish every step. 

## Buttons

1. UP (LEFT)
2. SELECT (MENU)
3. DOWN (RIGHT)
4. D/R (EXIT)  
   1) Toggle Dual Rate state.
   2) Return main screen when this button is pressed in any menu screens.

## Screens
surfTX uses 12864-06d LCD screen. 
(If you want to use Nokia 5110 use this branch : https://github.com/doohans/arduino_surface_TX_4ch/tree/nokia5110_ver )

![Screen01](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen01.jpg)
![Screen02](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen02.jpg)
![Screen03](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen03.jpg)
![Screen04](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen04.jpg)
![Screen05](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen05.jpg)
![Screen06](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen06.jpg)
![Screen07](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen07.jpg)
![Screen08](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen08.jpg)
![Screen09](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen09.jpg)
![Screen10](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/screen10.jpg)

## Low Voltage Alarm
You need modify Low Voltage Alarm value(minBat and maxBat). In my case, I use voltage step-up module(buck boost converter) to use 1s li-ion battery. If you use 2s li-po battery, modify like that minBat is 64 and maxBat is 84. If there is a gap between measured voltage value and real voltage value, Modify "corr_volt" value for corrention.

```
#define minBat 32                    // Minimum battery voltage for alarm (32 = 3.2V)
#define maxBat 42                    // Maximum battery voltage (7V) (42 = 4.2V)
#define corr_volt 2                  // Correction battery voltage
```

## EEPROM Memory Map

https://docs.google.com/spreadsheets/d/1R2shopYCNBNx50pZLSfzXPBwvNPSjA0mwf481hEYQtA/edit#gid=0

