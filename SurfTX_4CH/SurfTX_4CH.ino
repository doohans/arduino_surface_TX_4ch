// ref. 1
// This code is based on Arduino-RC-6CH-Radio-control sketch
// Gabapentin
// https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control

// ref. 2
// This code is based on ardutx v1.3 sketch
// (c) 2014 Patricio Reinoso
// www.patolin.com

/* This project is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  this code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See t he
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this code. If not, see <http://www.gnu.org/licenses/>.*/

// *************************************************************************************************************
//
// *************************************************************************************************************
// Recommended Arduino IDE 1.8.2 // Seems to be optimized and stable compiler. Less Flash and RAM used
// Arduino AVR Boards 1.6.18
// *************************************************************************************************************
/*
  +++++++++++++++++
  Input Pots
  4 channels
  +++++++++++++++++
  Steering  CH1 = A0
  Throtle   CH2 = A1
  Pot VRA  CH3 = A2
  Pot VRB  CH4 = A3

  //---------------------------------------------------------------------------------------------
  // 2 position switch (pin A6 in the center, VCC and GND at both ends)
  // NOTE: switch is activated when pin A6 is closed to GND (normaly closed to VCC)
  //---------------------------------------------------------------------------------------------
  swA = A6

  //---------------------------------------------------------------------------------------------
  // Must be used R1-10K/R2-10K voltage divider for 2S lion/LiPo battery
  // (R1-20K/R2-10K voltage divider for 3S lion/LiPo battery)
  //---------------------------------------------------------------------------------------------
  batPin = A7

  (Battery voltage divider)

  Vbatt MAX 10V ----+
                   |
                   R1 10K
                   |
                   |+-----> (PIN A7)
                   |
                   R2 10K
                   |
                   =
                   .  (GND)


  //---------------------------------------------------------------------------------------------
  // Mount 0,1uF/100nF(104)ceramic capacitor from pin to GND
  //---------------------------------------------------------------------------------------------
  UP Button    pin 2
  Down Button  pin 3
  Enter Button pin 4

  //---------------------------------------------------------------------------------------------
  // 2 position switch (is activated when pin is closed to GND)
  //---------------------------------------------------------------------------------------------
  swDR         pin 5
  swB          Pin 12

  //---------------------------------------------------------------------------------------------
  buzzer       pin 6   // Buzzer with sound generator (need transistor driving)

  //---------------------------------------------------------------------------------------------
  // Must be used voltage divider on pin 7 to limit at 3Vpp ppm signal
  // because T6A/CT6B(A7105)TX module is not 5V tollerant
  //---------------------------------------------------------------------------------------------
  sigPin       pin 7

  (PPM Signal voltage divider)

  sigPin 7 ----------+
                   |
                   R1 10K
                   |
                   |+-----> 2.7Vpp(to TX PPM PIN)
                   |
                   R2 12K(Max 15K)
                   |
                   =
                   .  (GND)
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <SPI.h>                   // Arduino standard library
#include <EEPROM.h>                // Arduino standard library
#include <U8g2lib.h>               // Using U8G2 library "https://github.com/olikraus/u8g2"
#include "Config.h"                // Load static and variable configuration settings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup() {

  //Serial.begin(115200);

  // LCD config with U8G2 library display init (mandatory)
  u8g2.begin();

  if (VCC == 5)
    u8g2.setContrast(115);

  // Set font type
  u8g2.setFont(u8g2_font_5x7_tr);

  // Set memory buffer for text strings
  char msg_buffer[21];
  char char_buffer[3];

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Boot Screen
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  u8g2.firstPage();
  do {
    // confirm msg
    // Print "V" text string
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[18])));
    u8g2.setCursor(0, 6);
    u8g2.print(char_buffer);

    // Print version string
    strcpy_P(msg_buffer, (char*)ver_str);
    u8g2.setCursor(6, 6);
    u8g2.print(msg_buffer);

    // Print logo XBM image
    u8g2.drawXBMP(0, 8, logo_width, logo_height, logo);

  } while (u8g2.nextPage());


  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Input ADC sampling rate for reading analog input pots (1000 KHz is maximum for stable reading)
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));   // Remove bit settings from Arduino library

#if F_CPU == 16000000L
  ADCSRA |= (1 << ADPS2);                                  // Set ADC prescaler to 16 (1000 KHz)
#else
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);                     // Set ADC prescaler to 8 (1000 KHz)
#endif


  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);                                  // Avcc(+5v) as voltage reference

  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)); // ADC in free-running mode

  ADCSRA |= (1 << ADEN);                                   // Turn on ADC
  ADCSRA |= (1 << ADSC);                                   // Do an initial conversion this one is the slowest to ensure that everything is up and running

  DIDR0 = 0x3F;                                            // Digital circuit on analog ports disabled
  DIDR1 = 0x03;                                            // Disable digital input on AIN0/1

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Debouncing mechanical buttons
  // NOTE: For input pin buttons is necessary to mount on every pin
  // 0,1uF/100nF(104)ceramic capacitors from pin to GND
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(swDR, INPUT_PULLUP);
  pinMode(swB,  INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  res_calc = (R1 + R2) / R2;             // Voltage divider calculation

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Initialize value for average ADC readings
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; // initialize all the readings to 0

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Default state values for ppm
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  ppm[0] = servoCenter;     // channel 1 (STR)
  ppm[1] = servoCenter;     // channel 2 (THR)
  ppm[2] = ppmMin;          // channel 3 (VRA,SWA)
  ppm[3] = ppmMin;          // channel 4 (VRB,SW2)

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Default state config parameters
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Servo Direction bit mask: 0 Normal, 1 Reverse
  servoReverse = 0b00000000;

  // Dual Rates and Sub Trim default values (only for first two channels)
  for (int i = 0; i < CHANNELS - 2; i++) {
    dual_rate_low[i] = 75;
    dual_rate_hi[i] = 100;
    subTrim[i] = 0;
  }

  //model name
  for (int i = 0; i < 5; i++) {
    modelName[i] = 0x5f;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Config output signal pin
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  DDRD |= _BV(sigPin);  // Set pin 7 as an output
  PORTD ^= _BV(sigPin); // Set pin 7 toggle state

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ppm timer config
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  configTimer1();

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Hold button "Down" on power ON to initialize default memory data
  // Will only write data settings to default (not erase calibration data)
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Delay before reading button (about charge capacitor pulse on pin)
  delay(100);


  if ((PIND & (1 << 3)) == 0) {

    bool isWait = true;

    u8g2.firstPage();
    do {

      // Print "ERASE DATA" text string
      strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[5])));
      u8g2.setCursor(0, 20);
      u8g2.print(msg_buffer);

      // Print "?" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[15])));
      u8g2.setCursor(53, 20);
      u8g2.print(char_buffer);

      // Print "Y" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
      u8g2.setCursor(5, 40);
      u8g2.print(char_buffer);

      // Print "=" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(12, 40);
      u8g2.print(char_buffer);

      // Print "SEL" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(19, 40);
      u8g2.print(char_buffer);

      // Print "N" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
      u8g2.setCursor(55, 40);
      u8g2.print(char_buffer);

      // Print "=" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(62, 40);
      u8g2.print(char_buffer);

      // Print "UP" text string
      strcpy_P(char_buffer, (char*)pgm_read_word(&(channel_name[4])));
      u8g2.setCursor(69, 40);
      u8g2.print(char_buffer);


    } while (u8g2.nextPage());

    while (isWait) {

      switch (readKeys()) {
        case 1:
          // upbutton
          isWait = false;
          break;
        case 2:
          // upbutton
          isWait = false;

          // Recall "Reset to default" macro
          resetEeprom();

          u8g2.firstPage();
          do {
            // Print "ERASE DATA" text string
            strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[5])));
            u8g2.setCursor(0, 20);
            u8g2.print(msg_buffer);

          } while (u8g2.nextPage());
          
          break;
      }
    }


  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Load data from Eeprom
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  modelActual = storedDataEeprom(255);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Set default font type used for all display sessions (mandatory)
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  u8g2.setFont(u8g2_font_5x7_tr);

  // for boot logo duration
  delay(1000);
}

// End of Setup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void loop() {
  // Start Calibration screen if button Enter/Select is pressed on power on ***********************  
  if (readKeys() == 2 && statusCalib == 1) {

    Calibration();    // Recall calibration procedure
  }

  // Set condition 0 to continue loop if calibration procedure is not selected
  else {
    statusCalib = 0;
  }

  // Start of checking switches default status ****************************************************

  readSwitches();     // Read sw_status switches values


  // Starting main session *************************************************************************

  readPots();                 // Recall macro for calculating ppm output


  batteryCheck();             // Checking battery status

  // Start of screen display management ************************************************************

  if (screen == 0) {          // Print and drawing main screen
    Screen_0();
  }
  else {
    if (screen == 1) {        // Print menu selection screen
      Screen_1();
    }
    if (screen == 2) {

      if (menuActual == 1) {  // Execute "Servo Direction" task
        Menu_1();
      }
      if (menuActual == 2) {  // Execute "Dual Rates" task
        Menu_2();
      }
      if (menuActual == 3) {  // Execute "Model Selection" task
        Menu_3();
      }
      if (menuActual == 4) {  // Execute "Save Model Data" task
        Menu_4();
      }
      if (menuActual == 5) {  // Execute "Sub Trim Setup" task
        Menu_5();
      }
      if (menuActual == 6) {  // Execute "Set Model Name" task
        Menu_6();
      }
      if (menuActual == 7) {  // Execute "Exp Setup" task
        Menu_7();
      }
    }
  }

  // If pressed buttonSelect/Enter
  if (readKeys() == 2) {
    if (screen < 2) {
      screen++;
      menuActual = menuSubActual;
      menuSubActual = 1;
      menuSubModel = 0;
    }
    else {
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Added if Menu/Select button is pressed without menu cursor (do nothing)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (menuActual == 0) {
        screen--;
        menuActual--;
      }
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Options for each menu
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (menuActual == 1) {

        // Selection value for Servo Direction
        bitWrite(servoReverse, menuSubActual - 1, !bitRead(servoReverse, menuSubActual - 1));
      }
      if (menuActual == 2) {

        // Selection value for Dual Rates
        if (drSelection != 0xFF) {
          drSelection = 0xFF;
        }
        else {
          drSelection = menuSubActual - 1;
        }
      }

      // Model Selection data
      if (menuActual == 3) {

        // Selection value for Model Selection
        modelActual = menuSubModel;

        // Save data in eeprom
        EEPROM.update(actualModelEepromAddr, modelActual);
        storedDataEeprom(modelActual);

        screen = 0;
        menuSubModel = 1;
        menuActual = 0;

        delay(1);
      }

      if (menuActual == 5) {

        // Selection value for Sub Trim
        if (subTrimSelection != 0xFF) {
          subTrimSelection = 0xFF;
        }
        else {
          subTrimSelection = menuSubActual - 1;
        }
      }
      if (menuActual == 6) {

        // Selection value for Dual Rates
        if (modelNameSelection != 0xFF) {
          modelNameSelection = 0xFF;
        }
        else {
          modelNameSelection = menuSubActual - 1;
        }
      }

      if (menuActual == 7) {

        // Selection value for Expo
        if (expSelection != 0xFF) {
          expSelection = 0xFF;
        }
        else {
          expSelection = menuSubActual - 1;
        }
      }
    }
  }

  if (readKeys() == 3) {
    switch (menuActual) {

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Dual Rate screen step control for selecting channels/values (buttonDown)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 2:

        // Initial value for Dual Rates selection value
        if (drSelection == 0xFF) {

          // Only first 2 channels
          if (menuSubActual - 1 < (CHANNELS * 2) - 5) { // Two values * 2 channels
            menuSubActual++;
            if (screen == 0) {
              screen++;
            }
          }
        }
        // Dual Rates step control for changing selected channel value(DOWN)
        else {
          if ((drSelection % 2) == 0) {

            // Define Min for LOW Rates value
            if (dual_rate_low[drSelection / 2] > 0) {

              // Define step value
              dual_rate_low[drSelection / 2]--;
            }
          }
          else {

            // Define Min for HIGH Rates value
            if (dual_rate_hi[drSelection / 2] > 0) {

              // Define step value
              dual_rate_hi[drSelection / 2]--;
            }
          }
        }

        break;

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Model Selection
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 3:
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // Added for Model Menu management
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (menuSubModel < MODELS - 1) { // 0 to 4 items
          menuSubModel++;
          modelPage = (menuSubModel) / 10;
          if (screen == 0) {
            screen++;
          }
        }



        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        break;

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Sub Trim screen step control for selecting channels/values (buttonDown)
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 5:

        // Initial value for Sub Trim selection value
        if (subTrimSelection == 0xFF) {

          // Only first two channels
          if (menuSubActual < CHANNELS - 2) {
            menuSubActual++;

            if (screen == 0) {
              screen++;
            }
          }
        }

        // Sub Trim step control for changing selected channel value DOWN
        else {
          if ((subTrimSelection % 1)  == 0) {

            // Define Min Sub trim value
            if (subTrim[subTrimSelection / 1] > potCenter * -1) {

              // Define step value
              subTrim[subTrimSelection / 1] -= 5;
            }
          }
        }
        break;
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Set Model Name screen step control for selecting values (buttonDown)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 6:

        // Initial value for Model Name selection value
        if (modelNameSelection == 0xFF) {

          // Only 5 characters
          if (menuSubActual < 5) {
            menuSubActual++;
            if (screen == 0) {
              screen++;
            }
          }
        }
        // changing selected position character aschii value(DOWN)
        else {
          if (modelName[modelNameSelection] > 0x21)
            modelName[modelNameSelection]--;
        }

        break;

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Expo screen step control for selecting channels/values (buttonDown)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 7:

        // Initial value for Expo selection value
        if (expSelection == 0xFF) {

          // Only first 2 channels
          if (menuSubActual < 2) { // 2 channels
            menuSubActual++;
            if (screen == 0) {
              screen++;
            }
          }
        }
        // Exp step control for changing selected channel value(DOWN)
        else {
          if (expo[expSelection] > 0) {

            // Define step value
            expo[expSelection]--;
          }
        }

        break;

      default:


        if (menuSubActual < MENU_COUNT) {  // 1 to 5 items
          menuSubActual++;
          if (screen == 0) {
            screen++;
          }
        }
        menuPage = (menuSubActual - 1) / 5;
        break;
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
  }

  if (readKeys() == 1) {
    switch (menuActual) {

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Dual Rate screen step control for selecting channels/values (buttonUp)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 2:

        // Initial value for Dual Rates selection value
        if (drSelection == 0xFF) {
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 2;
            menuActual = 0;
          }
          else {
            menuSubActual--;
          }
        }

        // Step control for changing selected channel value UP
        else {
          if ((drSelection % 2) == 0) {

            // Define Max for LOW Rates value
            if (dual_rate_low[drSelection / 2] < 100) {

              // Define step value
              dual_rate_low[drSelection / 2]++;
            }
          }
          else {

            // Define Max for HIGH Rates value
            if (dual_rate_hi[drSelection / 2] < 100) {

              // Define step value
              dual_rate_hi[drSelection / 2]++;
            }
          }
        }
        break;

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Model Selection
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 3:
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // Added for Model Menu management
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (menuSubModel > 0) {
          menuSubModel--;
          modelPage = (menuSubModel) / 10;
        }
        else {
          if (screen > 0) {
            screen--;
            menuSubModel = 1;
            menuSubActual = 3;
            menuActual = 0;
          }
        }


        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        break;

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Sub Trim screen step control for selecting channels/values (buttonUp)
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 5:
        if (subTrimSelection == 0xFF) {
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 5;
            menuActual = 0;
          }
          else {
            menuSubActual--;
          }
        }

        // Sub Trim step control for changing selected channel UP
        else {
          if ((subTrimSelection % 1) == 0 ) {

            // Define max value for Sub Trim
            if ((subTrim[subTrimSelection / 1]) < potCenter) {

              // Define step value
              subTrim[subTrimSelection / 1] += 5;
            }
          }
        }
        break;

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Set Model Name screen step control for selecting values (buttonUp)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 6:
        // Initial value for Model Name selection value
        if (modelNameSelection == 0xFF) {
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 6;
            menuActual = 0;
          }
          else {
            menuSubActual--;
          }
        }
        // changing selected position character aschii value(UP)
        else {
          if (modelName[modelNameSelection] < 0x7e)
            modelName[modelNameSelection]++;
        }

        break;

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Exp screen step control for selecting channels/values (buttonUp)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 7:

        // Initial value for Exp selection value
        if (expSelection == 0xFF) {
          if (menuSubActual < 2) {
            screen--;
            menuSubActual = 7;
            menuActual = 0;
          }
          else {
            menuSubActual--;
          }
        }

        // Step control for changing selected channel value UP
        else {
          // Define Max for Exp value
          if (expo[expSelection] < 9) {

            // Define step value
            expo[expSelection]++;
          }
        }
        break;

      default:

        if (menuSubActual > 1) {
          menuSubActual--;

          menuSubActual = menuSubActual < 1 ? 1 : menuSubActual;
          menuPage = (menuSubActual - 1) / 5;
        }
        else {
          if (screen > 0) {
            screen--;
            menuSubActual = 1;
            menuActual = 0;

          }
        }



        break;

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
  }
}
