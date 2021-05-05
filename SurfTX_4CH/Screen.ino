// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Main Screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Screen_0() {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];

  // Start of main screen display informations "0" ************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro again for stable ppm pulse

    u8g2.setFont(u8g2_font_6x10_tr);

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    //u8g2.setFont(u8g2_font_4x6_tr);

    if (dr_check > 0) {

      // Print Dual Rates status only if active (DR)
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[10])));
      u8g2.setCursor(58, 7);
      u8g2.print(chName_buffer);

      // Dual Rates switch status checking
      if (dr_check == 1) {
        // Print "L" character
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
        u8g2.setCursor(72, 7);
        u8g2.print(char_buffer);
      }
      if (dr_check == 2) {
        // Print "H" character
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(72, 7);
        u8g2.print(char_buffer);
      }

    }

    // Print Voltage
    u8g2.setCursor(96, 7);
    u8g2.print(batt_volt / 10);

    //strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[17])));
    //u8g2.setCursor(100, 6);
    //u8g2.print(char_buffer);
    u8g2.drawPixel(101, 7);

    u8g2.setCursor(103, 7);
    u8g2.print(batt_volt % 10);

    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[18])));
    u8g2.setCursor(109, 7);
    u8g2.print(char_buffer);

    // Battery simbol contruction
    u8g2.drawFrame(116, 0, 11, 7);             // Battery box
    u8g2.drawBox(116, 1, perc_batt, 5);        // level bar
    u8g2.drawBox(126, 2, 2, 3);                // Battery nipple plus pole


    //u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.setFont(u8g2_font_VCR_OSD_tr);

    // Print ">" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
    u8g2.setCursor(0, 28);
    u8g2.print(char_buffer);

    //model Name
    u8g2.drawStr(15, 28, modelName);

    // Sub Trim
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);
    u8g2.drawBox(88, 11, 8, 18);             // Trim Box
    //u8g2.drawBox(50, 7, 7, 14);
    for (int i = 0; i < 2; i++) {
      // Print Sub Trim value
      if ( i == 0) {
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[1])));
      } else {
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
      }
      u8g2.setCursor(90, 19 + i * 8);
      u8g2.print(char_buffer);

      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[i])));
      u8g2.setCursor(98, 19 + i * 8);
      u8g2.print(char_buffer);

      u8g2.setCursor(106, 19 + i * 8);
      u8g2.print(subTrim[i]);
    }
    u8g2.setDrawColor(1);
    u8g2.drawFrame(88, 11, 40, 18);             // Trim Box
    u8g2.setFontMode(0);

    // Print first 2 channels reference with input pots order

    // Print "S" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[0])));
    u8g2.setCursor(5, 41);
    u8g2.print(char_buffer);

    // Print "T" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[1])));
    u8g2.setCursor(5, 53);
    u8g2.print(char_buffer);

    //Drawing vertical middle/center separation line
    u8g2.drawVLine(63, 32, 24);

    // Drawing only first 2 channels
    for (int i = 0; i < 2; i++) {

      // Define value bar reference
      unsigned int valBar;

      // Value bars subdivision (ppm value / 55)
      valBar = map(ppm[i], ppmMin, ppmMax, 0 , 100);

      // Draw boxes/frames for every channel
      u8g2.drawFrame(13, 34 + (i * 12), 101, 8);

      // Drawing cursor in every channel bar
      if (valBar < 50) {
        u8g2.drawBox(14 + valBar, 34 + (i * 12), 50 - valBar, 8);
      } else if (valBar > 50) {
        u8g2.drawBox(64, 34 + (i * 12), valBar - 50, 8);
      }

      unsigned short subTrimVal = map(subTrim[i], 0, 500, 0, 50);

      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, i) == 1) {
        subTrimVal = -subTrimVal;
      }


      unsigned short minMaxValid = 0;
      //EPA
      minMaxValid = 63 - (50 * epa[i]   / 100) + subTrimVal;
      if (minMaxValid < 13) minMaxValid = 13;
      if (minMaxValid > 113) minMaxValid = 113;
      u8g2.drawVLine(minMaxValid, 35 + (i * 12), 2);


      minMaxValid = 63 + (50 * epa[i]   / 100) + subTrimVal;
      if (minMaxValid < 13) minMaxValid = 13;
      if (minMaxValid > 113) minMaxValid = 113;
      u8g2.drawVLine(minMaxValid, 35 + (i * 12), 2);

      // Dual Rates switch status checking
      if (dr_check == 1) {
        // DR L

        short drl_1 = dual_rate_low[i];
        short drl_2 = dual_rate_low[i];

        if (i == 1) {
          if (bitRead(servoReverse, i) == 1) {
            drl_2 = dual_rate_low[2];
          } else {
            drl_1 = dual_rate_low[2];
          }
        }

        minMaxValid = 63 - (50 * drl_1 / 100) + subTrimVal;
        if (minMaxValid < 13) minMaxValid = 13;
        if (minMaxValid > 113) minMaxValid = 113;
        u8g2.drawVLine(minMaxValid, 39 + (i * 12), 2);

        minMaxValid = 63 + (50 * drl_2 / 100) + subTrimVal;
        if (minMaxValid < 13) minMaxValid = 13;
        if (minMaxValid > 113) minMaxValid = 113;
        u8g2.drawVLine(minMaxValid, 39 + (i * 12), 2);

      }

      if (dr_check == 2) {
        // DR H

        short drh_1 = dual_rate_hi[i];
        short drh_2 = dual_rate_hi[i];

        if (i == 1) {
          if (bitRead(servoReverse, i) == 1) {
            drh_2 = dual_rate_hi[2];
          } else {
            drh_1 = dual_rate_hi[2];
          }
        }

        minMaxValid = 63 - (50 * drh_1 / 100) + subTrimVal;
        if (minMaxValid < 13) minMaxValid = 13;
        if (minMaxValid > 113) minMaxValid = 113;
        u8g2.drawVLine(minMaxValid, 39 + (i * 12), 2);

        minMaxValid = 63 + (50 * drh_2 / 100) + subTrimVal;
        if (minMaxValid < 13) minMaxValid = 13;
        if (minMaxValid > 113) minMaxValid = 113;
        u8g2.drawVLine(minMaxValid, 39 + (i * 12), 2);

      }



      if (bitRead(servoReverse, i) == 1) {

        // Print "R"everse channel status for every channel
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(117, 41 + i * 12);
        u8g2.print(char_buffer);
      }
      else {

        // Print "N"ormal channel status for every channel
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
        u8g2.setCursor(117, 41 + i * 12);
        u8g2.print(char_buffer);
      }

      //Expo
      if (expo[i] > 0) {
        // Print "E" character
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[19])));
        u8g2.setCursor(123, 41 + i * 12);
        u8g2.print(char_buffer);
      }

    }


    //If switch SWA print ON/OFF state
    if (bitRead(servoReverse, 2) == 1) {

      // Print "SWA" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[6])));
      u8g2.setCursor(0, 64);
      u8g2.print(chName_buffer);

      if (read_adc(6) < 500) {

        // Print SWA "ON" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[15])));
        u8g2.setCursor(20, 64);
        u8g2.print(chName_buffer);
      }
      else {

        // Print SWA "OFF" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[16])));
        u8g2.setCursor(20, 64);
        u8g2.print(chName_buffer);
      }
    }
    else {

      //If pot VRA print value in %
      unsigned int VRA;
      VRA = map(ppm[2], ppmMin, ppmMax, 0, 100);

      // Print "VRA" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[11])));
      u8g2.setCursor(0, 64);
      u8g2.print(chName_buffer);

      // Print VRA value
      u8g2.setCursor(20, 64);
      u8g2.print(VRA);
    }

    //If switch SWB print ON/OFF state
    if (bitRead(servoReverse, 3) == 1) {

      // Print "SWB" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[7])));
      u8g2.setCursor(93, 64);
      u8g2.print(chName_buffer);

      if (digitalRead(swB) == 0) {

        // Print SWB "ON" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[15])));
        u8g2.setCursor(113, 64);
        u8g2.print(chName_buffer);
      }
      else {

        // Print SWB "OFF" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[16])));
        u8g2.setCursor(113, 64);
        u8g2.print(chName_buffer);
      }
    }
    else {

      //If pot VRB print value in %
      unsigned int VRB;
      VRB = map(ppm[3], ppmMin, ppmMax, 0, 100);

      // Print "VRB" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[12])));
      u8g2.setCursor(93, 64);
      u8g2.print(chName_buffer);

      // Print VRB value
      u8g2.setCursor(113, 64);
      u8g2.print(VRB);
    }

  } while (u8g2.nextPage());
  u8g2.setFont(u8g2_font_6x10_tr);
  // End of main screen display informations "0" **************************
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Menu Selection screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Screen_1 () {

  // Set memory buffer for text strings
  char menu_buffer[18];
  char char_buffer[2];

  // Start of menu selection screen
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    // Print "MODEL" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(menu_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);


    //model Name
    u8g2.drawStr(48, 7, modelName);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // "/"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[16])));
    u8g2.setCursor(117, 7);
    u8g2.print(char_buffer);

    u8g2.setCursor(111, 7);
    u8g2.print(menuPage + 1);

    u8g2.setCursor(123, 7);
    u8g2.print( (MENU_COUNT - 1) / 5 + 1);  //Total Menu Count / menu count per page + 1

    for (int i = 1; i < 6; i++) {

      if (i + (5 * menuPage) > MENU_COUNT) break;

      // Print main menu items
      strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[i + (5 * menuPage) - 1])));

      if (i + (5 * menuPage) == menuSubActual) {

        // Print selection cursor character "*"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[13])));
        u8g2.setCursor(5, 9 + (i * 10));
        u8g2.print(char_buffer);

        // Print main menu items
        u8g2.setCursor(14, 9 + (i * 10));
        u8g2.print(menu_buffer);
      }
      else {

        // Print main menu items
        u8g2.setCursor(20, 9 + (i * 10));
        u8g2.print(menu_buffer);
      }
    }
  } while (u8g2.nextPage());
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Servo Direction screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_1 () {

  // Set memory buffer for text strings
  char chName_buffer[18];
  char char_buffer[2];

  // Start of Servo direction setting screen "2" "1" **********************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "SERVO DIR" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(menu_name[0])));
    u8g2.setCursor(75, 7);
    u8g2.print(chName_buffer);

    // Print Servo Direction channels screen list
    for (int i = 0; i < CHANNELS; i++) {

      // Print channel name list
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));

      if (i == menuSubActual - 1) {

        // Print selection cursor "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(5, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print character "=");
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
        u8g2.setCursor(31, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print selection cursor character "]" for selected item
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(57, 20 + i * 13);
        u8g2.print(char_buffer);

      }

      // Print channel items name
      u8g2.setCursor(12, 20 + i * 13);
      u8g2.print(chName_buffer);

      if (bitRead(servoReverse, i) == 1) {

        // Print(REV)erse status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[14])));
        u8g2.setCursor(38, 20 + i * 13);
        u8g2.print(chName_buffer);
      }
      else {

        // Print(NOR)mal status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[13])));
        u8g2.setCursor(38, 20 + i * 13);
        u8g2.print(chName_buffer);
      }

      if (bitRead(servoReverse, 2) == 1) {
        // Print SWA if REVerse status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[6])));
        u8g2.setCursor(38, 46);
        u8g2.print(chName_buffer);
      }
      else {

        // Print VRA if NORmal status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[11])));
        u8g2.setCursor(38, 46);
        u8g2.print(chName_buffer);
      }
      if (bitRead(servoReverse, 3) == 1) {
        // Print SWB if REVerse status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[7])));
        u8g2.setCursor(38, 59);
        u8g2.print(chName_buffer);
      }
      else {

        // Print VRB if NORmal status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[12])));
        u8g2.setCursor(38, 59);
        u8g2.print(chName_buffer);
      }
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Drawing dynamic graphics items
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Drawing status of CH5 if VRA(POT) or SWA(SWITCH)
    if (bitRead(servoReverse, 2) == 1) {
      u8g2.drawRFrame(73, 43, 11, 20, 3);               // Draw SWA box
      u8g2.drawDisc(78, 48, 3, U8G2_DRAW_ALL);          // Draw SWA round point
    }
    else {
      u8g2.drawCircle(78, 51, 9, U8G2_DRAW_ALL);        // Draw VRA
      u8g2.drawBox(66, 50, 9, 3);                       // Draw VRA cursor box
      u8g2.drawDisc(78, 51, 5, U8G2_DRAW_ALL);          // Draw SWB round point
    }

    // Drawing status of CH6 if VRB(POT) or SWB(SWITCH)
    if (bitRead(servoReverse, 3) == 1) {
      u8g2.drawRFrame(107, 43, 11, 20, 3);              // Draw SWB box
      u8g2.drawDisc(112, 48, 3, U8G2_DRAW_ALL);         // Draw SWB round point
    }
    else {
      u8g2.drawCircle(112, 51, 9, U8G2_DRAW_ALL);       // Draw VRB
      u8g2.drawBox(100, 50, 9, 3);                      // Draw VRB cursor box
      u8g2.drawDisc(112, 51, 5, U8G2_DRAW_ALL);         // Draw VRB round point
    }


    /*
        u8g2.drawFrame(47, 7, 18, 16);                     // Left Stick squared box

        // Drawing left stick position CH4/CH3
        u8g2.drawDisc(map(ppm[3], 1000 , 2000, 48, 64), map(ppm[2], 1000 , 2000, 21, 7), 1);

        u8g2.drawFrame(66, 7, 18, 16);                    // Right Stick squared box

        // Drawing left stick position CH1/CH2
        u8g2.drawDisc(map(ppm[0], 1000 , 2000, 67, 83), map(ppm[1], 1000 , 2000, 21, 7), 1);

    */
    for (int i = 0; i < 2; i++) {
      u8g2.drawHLine(72, 20 + i * 13, 45);
      u8g2.drawVLine(94, 20 + i * 13 - 4, 4);
      u8g2.drawBox(map(ppm[i], 1000 , 2000, 74, 114) - 1, 18 + (i * 13), 3, 2);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



  } while (u8g2.nextPage());
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Dual Rates screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_2 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];
  char menu_buffer[20];

  // Start of Dual Rates setting screen ***********************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    unsigned char counterTemp = 0;

    // Print Dual Rates channels list
    for (int i = 0; i < 3; i++) {

      // Print channel name list
      if (i > 1) {
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[1])));
      } else {
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      }

      u8g2.setCursor(2, 20 + i * 13);
      u8g2.print(chName_buffer);


      if (i == 1) {
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[20])));
        u8g2.setCursor(24, 20 + i * 13);
        u8g2.print(chName_buffer);
      }

      if (i == 2) {
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[21])));
        u8g2.setCursor(24, 20 + i * 13);
        u8g2.print(chName_buffer);
      }

      if (menuSubActual - 1 == counterTemp) {
        if (drSelection == counterTemp) {

          // Print "[" character for item selected for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(44, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print "]" character for item selected for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(82, 20 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(44, 20 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print "L" character for LOW value
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
      u8g2.setCursor(50, 20 + i * 13);
      u8g2.print(char_buffer);

      // Print character "=");
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(58, 20 + i * 13);
      u8g2.print(char_buffer);

      // Print LOW value
      u8g2.setCursor(66, 20 + i * 13);
      u8g2.print(dual_rate_low[i]);

      counterTemp++;

      if (menuSubActual - 1 == counterTemp) {
        if (drSelection == counterTemp) {

          // Print "[" character for item selected for HIGH value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(83, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print "]" character for item selected for HIGH value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(121, 20 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for HIGH value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(83, 20 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print "H" text character for HIGH value
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
      u8g2.setCursor(89, 20 + i * 13);
      u8g2.print(char_buffer);

      // Print character "=");
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(96, 20 + i * 13);
      u8g2.print(char_buffer);

      // Print HIGH value
      u8g2.setCursor(104, 20 + i * 13);
      u8g2.print(dual_rate_hi[i]);

      counterTemp++;
    }

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "DUAL RATES" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[1])));
    u8g2.setCursor(68, 7);
    u8g2.print(menu_buffer);

  } while (u8g2.nextPage());
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Model Selection screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_3 () {

  // Set memory buffer for text strings
  char char_buffer[2];
  char chName_buffer[6];

  // For Eeprom position reference
  unsigned int eepromPos;

  int tempModelNoIdx;
  // Start of Model Selection screen ***********************************

  u8g2.firstPage();
  do {
    //u8g2.setFont(u8g2_font_4x6_tr);

    //readPots(); // Recall macro for stable ppm pulse

    // Print Model Selection list
    for (int i = 0; i < 5; i++) {
      /// -----------------------------------------------------
      /// Left Section Start
      /// -----------------------------------------------------
      tempModelNoIdx = i + (10 * modelPage);
      if (tempModelNoIdx > MODELS) break;

      u8g2.setCursor(8, 19 + i * 10);
      u8g2.print(tempModelNoIdx + 1);

      // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
      eepromPos = numBytesPerModel * tempModelNoIdx;

      for (int j = 0; j < 5; j++) {
        u8g2.setCursor(23 + (j * 6), 19 + i * 10);
        char ch = EEPROM.read(eepromPos++);
        u8g2.print(ch);
      }

      if (tempModelNoIdx == menuSubModel) {

        // Print model selection cursor
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(0, 19 + i * 10);
        u8g2.print(char_buffer);

        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(55, 19 + i * 10);
        u8g2.print(char_buffer);
      }
      /// -----------------------------------------------------
      /// Left Section End
      /// -----------------------------------------------------

      /// -----------------------------------------------------
      /// Right Section Start
      /// -----------------------------------------------------
      tempModelNoIdx = (i + 5) + (10 * modelPage);
      if (tempModelNoIdx > MODELS) break;

      u8g2.setCursor(75, 19 + i * 10);
      u8g2.print(tempModelNoIdx + 1);

      // Define start position for Eeprom write/update (25 * [0,1,2,3,4])
      eepromPos = numBytesPerModel * tempModelNoIdx;

      for (int j = 0; j < 5; j++) {
        u8g2.setCursor(90 + (j * 6), 19 + i * 10);
        char ch = EEPROM.read(eepromPos++);
        u8g2.print(ch);
      }

      if (tempModelNoIdx == menuSubModel) {

        // Print model selection cursor
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(67, 19 + i * 10);
        u8g2.print(char_buffer);

        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(122, 19 + i * 10);
        u8g2.print(char_buffer);
      }
      /// -----------------------------------------------------
      /// Right Section End
      /// -----------------------------------------------------

    }


    //u8g2.setFont(u8g2_font_5x7_tr);
    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "MODEL SELECTION" text string
    //strcpy_P(chName_buffer, (char*)pgm_read_word(&(menu_name[2])));
    //u8g2.setCursor(35, 6);
    //u8g2.print(chName_buffer);

    // Drawing vertical line before airplane XBM image
    //u8g2.drawBox(18, 0, 2, 48);

    // Print airplane XBM image
    //u8g2.drawXBMP(24, 9, airplane_width, airplane_height, airplane);

    // Drawing vertical line after airplane XBM image
    //u8g2.drawBox(82, 0, 2, 48);

    // "/"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[16])));
    u8g2.setCursor(117, 7);
    u8g2.print(char_buffer);

    u8g2.setCursor(111, 7);
    u8g2.print(modelPage + 1);

    u8g2.setCursor(123, 7);
    u8g2.print( (MODELS - 1) / 10 + 1);  //Total model Count / model count per page + 1


  } while (u8g2.nextPage());


}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Save Model Data screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_4 () {

  // For Eeprom position reference
  unsigned int eepromBase;

  // For write/Update Sub Trim stored values
  //unsigned int posEeprom;

  // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
  eepromBase = numBytesPerModel * modelActual;

  // For write/Update Servo Direction and Dual Rates position
  unsigned int eepromPos = eepromBase;

  // Save Actual Model data
  EEPROM.update(actualModelEepromAddr, modelActual);

  // model name
  for (int i = 0; i < 5; i++) {
    EEPROM.update(eepromPos++, modelName[i]);
  }

  // Save Servo Direction data
  EEPROM.update(eepromPos++, servoReverse);

  // Save Sub Trim center stick values for two channels in every model memory bank
  for (int i = 0; i < 2; i++) {

    // Save center stick values for every channels
    EEPROMUpdateInt(eepromPos, subTrim[i]);
    eepromPos += 2;
  }

  // Save Dual Rates data
  for (int i = 0; i < 3; i++) {

    EEPROM.update(eepromPos, dual_rate_low[i]);
    eepromPos++;

    EEPROM.update(eepromPos, dual_rate_hi[i]);
    eepromPos++;
  }

  // Save Exp data
  for (int i = 0; i < 2; i++) {

    EEPROM.update(eepromPos, expo[i]);
    eepromPos++;
  }

  // Save EPA data
  for (int i = 0; i < 2; i++) {

    EEPROM.update(eepromPos, epa[i]);
    eepromPos++;
  }

  // Start of Save Data message screen ************************
  u8g2.firstPage();
  do {

    // Changing fot type
    //u8g2.setFont(u8g2_font_5x7_tr);

    // Set memory buffer for text strings
    char msg_buffer[21];

    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
    u8g2.setCursor(40, 12);
    u8g2.print(msg_buffer);

    strcpy_P(msg_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(43, 35);
    u8g2.print(msg_buffer);

    u8g2.setCursor(78, 35);
    u8g2.print(modelActual + 1);

    //model Name
    for (int i = 0; i < 5; i++) {
      u8g2.setCursor(47 + i * 7, 55);
      u8g2.print(modelName[i]);
    }

  } while (u8g2.nextPage());

  // End of Save Data message screen **************************

  delay(1000); // Screen message for 2sec

  // Changing to default font type
  //u8g2.setFont(u8g2_font_5x7_tr);

  screen--;
  menuActual = 0;
  menuSubActual = 4;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Sub Trim Setup screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_5 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];
  char menu_buffer[18];

  // Start Sub Trim Setup screen ***********************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    unsigned char temp_Counter = 0;

    // Print Sub Trim channels list
    for (int i = 0; i < 2; i++) {

      // Print channel name items
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(22, 20 + i * 13);
      u8g2.print(chName_buffer);

      if (menuSubActual - 1 == temp_Counter) {
        if (subTrimSelection == temp_Counter) {

          // Print selection cursor character ">" for channel
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(10, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print selection cursor character "[" for selected item
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(50, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print selection cursor character "]" for selected item
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(85, 20 + i * 13);
          u8g2.print(char_buffer);


        }
        else {

          // Print selection cursor character ">" for channel
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(10, 20 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print Sub Trim value
      u8g2.setCursor(58, 20 + i * 13);
      u8g2.print(subTrim[i]);


      // Print "PPM"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[17])));
      u8g2.setCursor(98, 20 + i * 13);
      u8g2.print(chName_buffer);

      temp_Counter++;
    }

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of actual model
    u8g2.setCursor(31, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "SUB TRIM" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[4])));
    u8g2.setCursor(81, 7);
    u8g2.print(menu_buffer);

  } while (u8g2.nextPage());

  // End Sub Trim Setup screen ***********************************
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Set Model Name screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_6 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];

  // Start of Set Model Name screen ***********************************
  u8g2.firstPage();
  do {
    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "NAME" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(messages[1])));
    u8g2.setCursor(100, 7);
    u8g2.print(chName_buffer);

    u8g2.setFont(u8g2_font_VCR_OSD_tr);
    // Print Model Name 5 byte
    for (int i = 0; i < 5; i++) {
      u8g2.setCursor(13 + (i * 23), 45);
      u8g2.print(modelName[i]);

      if (menuSubActual - 1 == i) {
        if (modelNameSelection == i) {

          // Print "[" character for item selected for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(1 + (i * 23), 45);
          u8g2.print(char_buffer);

          // Print "]" character for item selected for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(25 + (i * 23), 45);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(1 + (i * 23), 45);
          u8g2.print(char_buffer);
        }
      }
    }

    u8g2.setFont(u8g2_font_6x10_tr);

  } while (u8g2.nextPage());

}



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Exp screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_7 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];
  char menu_buffer[18];

  // Start of Exp setting screen ***********************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    // Print Expo channels list
    for (int i = 0; i < 2; i++) {

      // Print channel name list
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(7, 20 + i * 13);
      u8g2.print(chName_buffer);

      // Print Exp value
      u8g2.setCursor(34, 20 + i * 13);
      u8g2.print(expo[i]);
        
      if (menuSubActual - 1 == i) {
        if (expSelection == i) {

          // Print "[" character for item selected for Exp value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(28, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print "]" character for item selected for Exp value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(40, 20 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for Exp value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(0, 20 + i * 13);
          u8g2.print(char_buffer);
        }

        // Draw Exp Graph
        u8g2.drawHLine(53, 36, 75);
        u8g2.drawVLine(92, 10, 54);

        u8g2.drawFrame(53, 10, 75, 54);
        u8g2.drawLine(52, 64, 128, 10);

        if (expo[i] > 0) {


          for (int j = 52; j <= 91; j++) {
            u8g2.drawPixel(j, map(calc_expo(servoCenter, map(j, 52, 91, ppmMin, servoCenter), ppmMin, expo[i]), ppmMin, servoCenter, 64, 36));
          }

          for (int j = 91; j <= 128; j++) {
            u8g2.drawPixel(j, map(calc_expo(servoCenter, map(j, 91, 128, servoCenter, ppmMax), ppmMax, expo[i]), servoCenter, ppmMax, 35, 9));
          }
        }

      }

    }

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(31, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "EXP" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
    u8g2.setCursor(75, 7);
    u8g2.print(menu_buffer);

  } while (u8g2.nextPage());


}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing EPA Setup screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_8 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];
  char menu_buffer[18];

  // Start EPA Setup screen ***********************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    unsigned char temp_Counter = 0;

    // Print EPA channels list
    for (int i = 0; i < 2; i++) {

      // Print channel name items
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(22, 20 + i * 13);
      u8g2.print(chName_buffer);

      if (menuSubActual - 1 == temp_Counter) {
        if (epaSelection == temp_Counter) {

          // Print selection cursor character ">" for channel
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(10, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print selection cursor character "[" for selected item
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(50, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print selection cursor character "]" for selected item
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(78, 20 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for channel
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(10, 20 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print EPA value
      u8g2.setCursor(58, 20 + i * 13);
      u8g2.print(epa[i]);

      // Print "%"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[20])));
      u8g2.setCursor(98, 20 + i * 13);
      u8g2.print(char_buffer);
      
      temp_Counter++;
    }

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of actual model
    u8g2.setCursor(31, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    // Print "ENDPOINT ADJ" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[7])));
    u8g2.setCursor(51, 7);
    u8g2.print(menu_buffer);

  } while (u8g2.nextPage());

  // End EPA Setup screen ***********************************
}
