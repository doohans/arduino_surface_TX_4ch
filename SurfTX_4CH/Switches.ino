// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for read switch status definitions (sw_status)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool isFirstDrCheck = true;
void readSwitches() {

  // excute only one time after drSw is up.
  if (drSwStatus == 1 && !bit_is_clear(PIND, swDR)) {
    //dr button pushed
    drSwStatus = !drSwStatus;
    
    if (screen != 0) {
      //DR btn can be cancle button. go main screen.
      screen = 0;
      menuSubActual = 1;
      menuActual = 0;
      menuPage=0;

      return;
    }

    if (isFirstDrCheck) {
      isFirstDrCheck = !isFirstDrCheck;
      return;
    }

    if (dr_check < 2)
      dr_check++;
    else
      dr_check = 0;

    // dual rate sw status
    EEPROM.update(dualRateSwEepromAddr, dr_check);


  }

  if (bit_is_clear(PIND, swDR)) {
    //dr button up
    drSwStatus = 1;;
  }

}
