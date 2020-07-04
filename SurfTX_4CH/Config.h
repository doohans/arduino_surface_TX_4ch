// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Set library for SSD1306 SPI 0.96" OLED display TESTED OK! (128 bytes buffer)
// U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// Set library for SH1106 SPI 1.3" OLED display TESTED OK! (128 bytes buffer)
//U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// Set library for PCD8544 SPI Nokia 5110 display TESTED OK! (128 bytes buffer)
//U8G2_PCD8544_84X48_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

U8G2_ST7565_ERC12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_ERC12864_ALT_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); // contrast improved version for ERC12864

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* SPI wires cabling (OLED Pin name dependent on the manufacturer)

  OLED   Arduino
  ---------------------
  RES  > pin 8
  DC   > pin 9
  CS   > pin 10 (SS)
  MOSI > pin 11 (MOSI)
  CLK  > pin 13 (SCK)*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// config PPM settings
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define servoCenter 1500             // Servo center value (us)
//#define ppmLong 18500                // ppm frame (us) for 6 channels
#define ppmLong 18500                // ppm frame (us) for 6 channels
#define ppmPulse 300                 // ppm pulse width (us)
#define onState 0                    // Polarity: 0 neg / 1 pos
#define sigPin PD7                   // ppm output pin
#define ppmMin 1000                  // Min ppm output to be mapped (us)
#define ppmMax 2000                  // Max ppm output to be mapped (us)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Config global TX param
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CHANNELS 4                   // Number of channels
#define MODELS 30                    // Total memory models 30
#define potMin 0                     // Pot Min reference value
#define potMax 1023                  // Pot Max reference value
#define potCenter 512                // Pot Center reference value
//#define numBytesPerModel 32          // Maximum bytes for data storage per model
#define numBytesPerModel 25          // Maximum bytes for data storage per model
#define drMax 200                    // Maximum dual rate value

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Config pin assignment
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define swDR 5                       // Dual Rate pin (active LOW)
//#define batPin A7                    // Battery pin (over 5V need voltage divider and value conversion to display)
#define buzzer PD6                   // Buzzer pin (buzzer with standalone sound generator)
#define buttonSelect 4               // Button for Menu/Select functions
#define buttonUp 2                   // Button for Up/Prev functions
#define buttonDown 3                 // Button for Down/Next functions
//#define swA A6                       // Switch A Analog pin (active 0)
#define swB 12                       // Switch B Digital pin (active LOW)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define minBat 29                    // Minimum battery voltage for alarm (32 = 3.2V)
#define maxBat 42                    // Maximum battery voltage (7V) (42 = 4.2V)
#define corr_volt 2                  // Correction battery voltage
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// XBM image size for Model Selection screen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define logo_width 128            // Pixel width of logo image
#define logo_height 50           // Pixel height of logo image
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



#define dualRateSwEepromAddr 1022
#define actualModelEepromAddr 1023

#define MENU_COUNT 8            // Total Menu Count

#if F_CPU == 16000000L
#define OCR_MUL 2
#define KEY_DELAY 150
#else
#define OCR_MUL 1
#define KEY_DELAY 130
#endif

#define VCC 5       // arduino VCC 5(5v) or 33(3.3v)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// version
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char ver_str[] PROGMEM = "1.3";
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned short int ppm[CHANNELS];                       // ppm output array
unsigned short int pots[CHANNELS];                      // Input ADC data array
unsigned short int calibration[CHANNELS][2];            // Min/Max value array for read channels and store calibration
unsigned short int raw_Pots[CHANNELS];                  // Read raw input pots array
unsigned short int centerPos[2];                        // Center Position. only for Throttle and Steering

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Menu management variables
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char menuActual = 0;
unsigned char menuSubActual = 1;
unsigned char modelActual = 0;                // Added for Model Menu management
unsigned char menuSubModel = 0;               // Added for Model Menu management
unsigned char screen = 0;

unsigned char menuPage = 0;                  // for Menu Page
unsigned char modelPage = 0;                 // for Model Page

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Servo management parameters
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char modelNameSelection = 0xFF;      // Default value for Model Name Selection
unsigned char drSelection = 0xFF;             // Default value for Dual Rates Selection
unsigned char expSelection = 0xFF;            // Default value for Expo Selection
unsigned char servoReverse;                   // Reading bit status
unsigned char dual_rate_low[3];               // Dual Rates LOW value array
unsigned char dual_rate_hi[3];                // Dual Rates HIGH value array
unsigned char expo[2];                        // Expo value array

unsigned short int  deadBand = 30;            // Deadband center stick value (25 suggested value)
short  subTrim[2];                            // Sub Trim channel array
unsigned short int  subTrimSelection = 0xFF;  // Default value for Sub Trim

unsigned char epa[2];                         // EPA channel array
unsigned short int  epaSelection = 0xFF;      // Default value for EPA

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Model Name
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char modelName[5];                   // Model Name 5 char

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Checking conditions control status for warning switch and calibration
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
boolean statusCalib = 1;                // Checking status to enter in calibration procedure
boolean battStatus = 1;
boolean drSwStatus = 1;
unsigned short dr_check = 0;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

byte pulseBuzzer;                       // Sound pulse for battery alarm



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Battery voltage calculation, checking status for alarm, drawing battery bar
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int numReadings = 8;           // Number of reading to calculate average
int readings[numReadings];           // Readings from the analog input (battery input pin A7)
int index = 0;                       // Index of the current reading
int total = 0;                       // Running total
int average = 0;                     // Average value

float pin_batt_read;                 // Raw input battery pin value from average reading
int batt_volt;                     // Battery voltage calculated
float res_calc;                      // Calculation of voltage divider to be applied to the conversion
float R1 = 10000;                    // If you need more precision, measure voltage divider resistors value
float R2 = 10000;                    // with digital multimeter and put them in to R1 R2 variables
unsigned char perc_batt;             // Battery bar level indicator (20 pixel) only Min to Max values

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Using PROGMEM for characters and text strings to save RAM memory
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array text strings for drawing menu
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char menu_0[] PROGMEM = "SERVO DIRECTION";
const char menu_1[] PROGMEM = "DUAL RATES";
const char menu_2[] PROGMEM = "MODEL SELECTION";
const char menu_3[] PROGMEM = "SAVE MODEL DATA";
const char menu_4[] PROGMEM = "SUB TRIM SETUP";
const char menu_5[] PROGMEM = "SET MODEL NAME";
const char menu_6[] PROGMEM = "EXP SETUP";
const char menu_7[] PROGMEM = "END POINT ADJ";

const char* const menu_name[] PROGMEM = {
  menu_0,
  menu_1,
  menu_2,
  menu_3,
  menu_4,
  menu_5,
  menu_6,
  menu_7
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array text strings for screen messages
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char Message_0[] PROGMEM = "SAVE DATA";
const char Message_1[] PROGMEM = "NAME";
const char Message_2[] PROGMEM = "CALIBRATION";
const char Message_3[] PROGMEM = "Don't";
const char Message_4[] PROGMEM = "Move the Pots";
const char Message_5[] PROGMEM = "ERASE DATA";
const char Message_6[] PROGMEM = "WARNING !!!";
const char Message_7[] PROGMEM = "MIN MAX";
const char Message_8[] PROGMEM = "CENTER";

const char* const messages[] PROGMEM = {
  Message_0,
  Message_1,
  Message_2,
  Message_3,
  Message_4,
  Message_5,
  Message_6,
  Message_7,
  Message_8
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array text strings (short) for Channel/Switch/Pots names, status, model for all screens
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char chName_0[] PROGMEM = "STR";
const char chName_1[] PROGMEM = "THR";
const char chName_2[] PROGMEM = "CH3";
const char chName_3[] PROGMEM = "CH4";
const char chName_4[] PROGMEM = "UP";
const char chName_5[] PROGMEM = "DOWN";
const char chName_6[] PROGMEM = "SWA";
const char chName_7[] PROGMEM = "SWB";
const char chName_8[] PROGMEM = "MOD";
const char chName_9[] PROGMEM = "SEL";
const char chName_10[] PROGMEM = "DR";
const char chName_11[] PROGMEM = "VRA";
const char chName_12[] PROGMEM = "VRB";
const char chName_13[] PROGMEM = "NOR";
const char chName_14[] PROGMEM = "REV";
const char chName_15[] PROGMEM = "ON";
const char chName_16[] PROGMEM = "OFF";
const char chName_17[] PROGMEM = "PPM";
const char chName_18[] PROGMEM = "MODEL";
const char chName_19[] PROGMEM = "EXP";
const char chName_20[] PROGMEM = "FWD";
const char chName_21[] PROGMEM = "BWD";

const char* const channel_name[] PROGMEM = {
  chName_0,
  chName_1,
  chName_2,
  chName_3,
  chName_4,
  chName_5,
  chName_6,
  chName_7,
  chName_8,
  chName_9,
  chName_10,
  chName_11,
  chName_12,
  chName_13,
  chName_14,
  chName_15,
  chName_16,
  chName_17,
  chName_18,
  chName_19,
  chName_20,
  chName_21
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array for single characters
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char character_0[] PROGMEM = "S";
const char character_1[] PROGMEM = "T";
const char character_2[] PROGMEM = "Y";
const char character_3[] PROGMEM = "R";
const char character_4[] PROGMEM = "N";
const char character_5[] PROGMEM = "H";
const char character_6[] PROGMEM = "L";
const char character_7[] PROGMEM = "M";
const char character_8[] PROGMEM = "[";
const char character_9[] PROGMEM = "]";
const char character_10[] PROGMEM = "=";
const char character_11[] PROGMEM = "(";
const char character_12[] PROGMEM = ")";
const char character_13[] PROGMEM = "*";
const char character_14[] PROGMEM = ">";
const char character_15[] PROGMEM = "?";
const char character_16[] PROGMEM = "/";
const char character_17[] PROGMEM = ".";
const char character_18[] PROGMEM = "v";
const char character_19[] PROGMEM = "E";
const char character_20[] PROGMEM = "%";

const char* const one_char[] PROGMEM = {
  character_0,
  character_1,
  character_2,
  character_3,
  character_4,
  character_5,
  character_6,
  character_7,
  character_8,
  character_9,
  character_10,
  character_11,
  character_12,
  character_13,
  character_14,
  character_15,
  character_16,
  character_17,
  character_18,
  character_19,
  character_20
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// logo XBM bitmap for Boot screen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//static const unsigned char airplane[] U8X8_PROGMEM = {

static const unsigned char logo[] U8X8_PROGMEM = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x1C, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x01, 0x00, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xE1, 0x31, 0x00, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x11, 0x22,
  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x01, 0x00, 0xC9, 0x64, 0x00, 0xE0, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0xA9, 0x45, 0x00, 0x30, 0xF0, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x69, 0x45,
  0x00, 0x30, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x01, 0x00, 0xC9, 0x64, 0xE0, 0x13, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x11, 0x22, 0x00, 0x90, 0x03, 0x37,
  0xE0, 0xFF, 0xFF, 0x8F, 0xFF, 0x00, 0xFC, 0x87, 0x01, 0x00, 0xE1, 0x31,
  0x00, 0xD0, 0x86, 0x2D, 0xE0, 0xFF, 0xFF, 0x8F, 0xFF, 0x01, 0xFE, 0x87,
  0x01, 0x00, 0xFF, 0x1F, 0xFE, 0x78, 0xFD, 0x7A, 0xE0, 0xFF, 0xFF, 0x0F,
  0xFF, 0x01, 0xFE, 0x83, 0x01, 0x00, 0x41, 0x01, 0x00, 0xC0, 0x86, 0x0D,
  0xE0, 0xFF, 0xFF, 0x0F, 0xFF, 0x03, 0xFF, 0x83, 0x01, 0x00, 0x41, 0x03,
  0x00, 0x80, 0x03, 0x07, 0xE0, 0xFF, 0xFF, 0x0F, 0xFE, 0x03, 0xFF, 0x81,
  0x01, 0x00, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x0F,
  0xFE, 0x87, 0xFF, 0x81, 0x01, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xE0, 0xFF, 0xFF, 0x0F, 0xFC, 0x87, 0xFF, 0x80, 0x01, 0x00, 0x79, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0x0F, 0xF8, 0xCF, 0xFF, 0x80,
  0x01, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00,
  0xF8, 0xCF, 0x7F, 0x80, 0x01, 0x00, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xF0, 0x1F, 0x00, 0xF0, 0xFF, 0x7F, 0x80, 0x01, 0x00, 0x41, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0xF0, 0xFF, 0x3F, 0x80,
  0x01, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00,
  0xE0, 0xFF, 0x1F, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xF0, 0x1F, 0x00, 0xE0, 0xFF, 0x1F, 0x80, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0xC0, 0xFF, 0x0F, 0x80,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00,
  0xC0, 0xFF, 0x0F, 0x80, 0x01, 0xFC, 0xC3, 0x07, 0x1F, 0xFE, 0x03, 0xFC,
  0x0F, 0xF0, 0x1F, 0x00, 0x80, 0xFF, 0x07, 0x80, 0x01, 0xFE, 0xC7, 0x07,
  0x1F, 0xFE, 0x0F, 0xFC, 0x0F, 0xF0, 0x1F, 0x00, 0x00, 0xFF, 0x07, 0x80,
  0x01, 0xFF, 0xC7, 0x07, 0x1F, 0xFE, 0x1F, 0xFC, 0x0F, 0xF0, 0x1F, 0x00,
  0x00, 0xFF, 0x03, 0x80, 0x81, 0xFF, 0xC7, 0x07, 0x1F, 0xFE, 0x3F, 0xFC,
  0x0F, 0xF0, 0x1F, 0x00, 0x80, 0xFF, 0x07, 0x80, 0x81, 0x0F, 0xC2, 0x07,
  0x1F, 0x3E, 0x3E, 0x7C, 0x00, 0xF0, 0x1F, 0x00, 0xC0, 0xFF, 0x0F, 0x80,
  0x81, 0x0F, 0xC0, 0x07, 0x1F, 0x3E, 0x3E, 0x7C, 0x00, 0xF0, 0x1F, 0x00,
  0xC0, 0xFF, 0x0F, 0x80, 0x81, 0x3F, 0xC0, 0x07, 0x1F, 0x3E, 0x3E, 0x7C,
  0x00, 0xF0, 0x1F, 0x00, 0xE0, 0xFF, 0x1F, 0x80, 0x01, 0xFF, 0xC0, 0x07,
  0x1F, 0x3E, 0x3F, 0xFC, 0x0F, 0xF0, 0x1F, 0x00, 0xE0, 0xFF, 0x1F, 0x80,
  0x01, 0xFF, 0xC3, 0x07, 0x1F, 0xFE, 0x1F, 0xFC, 0x0F, 0xF0, 0x1F, 0x00,
  0xF0, 0xFF, 0x3F, 0x80, 0x01, 0xFE, 0xC7, 0x07, 0x1F, 0xFE, 0x0F, 0xFC,
  0x0F, 0xF0, 0x1F, 0x00, 0xF0, 0xDF, 0x7F, 0x80, 0x01, 0xF8, 0xC7, 0x07,
  0x1F, 0xFE, 0x07, 0xFC, 0x0F, 0xF0, 0x1F, 0x00, 0xF8, 0xCF, 0x7F, 0x80,
  0x01, 0xE0, 0xCF, 0x07, 0x1F, 0xBE, 0x0F, 0x7C, 0x00, 0xF0, 0x1F, 0x00,
  0xFC, 0x8F, 0xFF, 0x80, 0x01, 0x80, 0xCF, 0x07, 0x1F, 0x3E, 0x1F, 0x7C,
  0x00, 0xF0, 0x1F, 0x00, 0xFC, 0x87, 0xFF, 0x80, 0x81, 0x81, 0xCF, 0x8F,
  0x1F, 0x3E, 0x1F, 0x7C, 0x00, 0xF0, 0x1F, 0x00, 0xFE, 0x07, 0xFF, 0x81,
  0x81, 0xFF, 0xC7, 0xFF, 0x0F, 0x3E, 0x3E, 0x7C, 0x00, 0xF0, 0x1F, 0x00,
  0xFE, 0x03, 0xFF, 0x83, 0x81, 0xFF, 0x87, 0xFF, 0x0F, 0x3E, 0x7E, 0x7C,
  0x00, 0xF0, 0x1F, 0x00, 0xFF, 0x03, 0xFE, 0x83, 0x81, 0xFF, 0x03, 0xFF,
  0x07, 0x3E, 0x7C, 0x7C, 0x00, 0xF0, 0x1F, 0x80, 0xFF, 0x01, 0xFE, 0x87,
  0x01, 0xFF, 0x00, 0xFE, 0x01, 0x3E, 0xFC, 0x7C, 0x00, 0xF0, 0x1F, 0x80,
  0xFF, 0x00, 0xFC, 0x87, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xF0, 0x1F, 0xC0, 0xFF, 0x00, 0xFC, 0x8F, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
