//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for read sticks, pots, switches values and applying calibration and rules
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void readPots() {

  for (int i = 0; i < CHANNELS; i++) {
    int tempPPM = servoCenter;

    raw_Pots[i] = read_adc(i);

    // only for throttle and steering ch.
    if ( i < 2) {

      // Applying calibration mapping
      // In case of surface TX, Left and Right rotation rate should be same.
      // So, Longer side length is used for both side.
      int gap = calibration[i][1] - centerPos[i];
      int gapTemp = centerPos[i] - calibration[i][0];

      // Select longer side
      if (gap < gapTemp) gap = gapTemp;

      // Calculate Center offset
      int centerOffset = potCenter - centerPos[i];

      // Applying initial value with center offset.
      pots[i] = raw_Pots[i] + centerOffset;

      // range out correction.
      if (pots[i] < calibration[i][0] + centerOffset) pots[i] = calibration[i][0] + centerOffset;
      if (pots[i] > calibration[i][1] + centerOffset) pots[i] = calibration[i][1] + centerOffset;

      int dRateVal = 0;

      // Checking Dual Rates switch status,
      // if DR activated limitation of the preset percentage value will be applied
      if (dr_check == 1) {
        // LOW rates calculation
        dRateVal = 500 - (500 * dual_rate_low[i] / 100);
      }

      if (dr_check == 2) {
        // HIGH rates calculation
        dRateVal = 500 - (500 * dual_rate_hi[i] / 100);
      }

      // Convert Analog Value to PPM Value
      if (pots[i]  < (potCenter - deadBand)) {
        tempPPM = map(pots[i], potCenter - gap, potCenter - deadBand, ppmMin + dRateVal, servoCenter);

        //expo.
        if (expo[i] > 0)
          tempPPM = calc_expo(tempPPM, ppmMin + dRateVal, expo[i]);

        
      } else if (pots[i]  > (potCenter + deadBand)) {
        tempPPM = map(pots[i], potCenter + deadBand, potCenter + gap - 1, servoCenter, ppmMax - dRateVal);

        //expo.
        if (expo[i] > 0)
          tempPPM = calc_expo(tempPPM, ppmMax - dRateVal, expo[i]);
        
      } else {
        tempPPM = servoCenter;
      }

      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, i) == 1) {
        tempPPM = ppmMax - tempPPM + ppmMin;
      }

      //apply subtrim value
      tempPPM += subTrim[i];

      //EPA check
      short epaAmt = 500 - (500 * epa[i] / 100);
      short epaMin = ppmMin + epaAmt;
      short epaMax = ppmMax - epaAmt;
      if(tempPPM < epaMin) tempPPM = epaMin;
      if(tempPPM > epaMax) tempPPM = epaMax;
      
      //Min Max Validation
      if (tempPPM < ppmMin) tempPPM = ppmMin;
      if (tempPPM > ppmMax) tempPPM = ppmMax;

    } else {
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // ppm mapping pot VRA to Switch SWA
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (i == 2) {
        // SWA switch ppm mapping on analog input (CH3)
        if (bitRead(servoReverse, 2) == 1) {
          tempPPM = map(read_adc(6), potMin, potMax, ppmMax, ppmMin);
        }
        else {
          // VRA pot mapping (CH3)
          tempPPM = map(read_adc(2), calibration[i][0], calibration[i][1], ppmMin, ppmMax);
        }
      }

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // ppm mapping pot VRB to Switch SWB
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (i == 3) {
        // SWB switch ppm mapping on digital input (CH4)
        if (bitRead(servoReverse, 3) == 1) {
          tempPPM = map(digitalRead(swB), 1, 0, ppmMin, ppmMax);
        }
        else {
          // VRB pot mapping (CH4)
          tempPPM = map(read_adc(3), calibration[i][0], calibration[i][1], ppmMin, ppmMax);
        }
      }
    }

    ppm[i] = tempPPM;


  }


}
