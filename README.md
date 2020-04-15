# surfTX (arduino_surface_TX_4ch)

## Overview


![surfTX Logo](https://github.com/doohans/arduino_surface_TX_4ch/blob/master/Images/logo.png)

This project is another fork from https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control

The focus of this project is Making PISTOL type TX for surface vehicle such as car or boat.

This transmitter does not have its own communication module. Only PPM output is supported. A separate PPM module is required to use this transmitter. In my case, I use 4 in 1 multiprotocol module.

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

## EEPROM Memory Map

https://docs.google.com/spreadsheets/d/1R2shopYCNBNx50pZLSfzXPBwvNPSjA0mwf481hEYQtA/edit#gid=0


