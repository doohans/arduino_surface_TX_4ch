// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Expo Eq.
// Y=X*Exp((X-EndPoint)/dV))
// proper dV value : 300 ~ 700
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int calc_expo(unsigned short  value, unsigned short inMax, unsigned short dV) {
  if (value == servoCenter) return value;
  if (dV <= 0) return value;

  dV = 9 - dV; //1 ~ 9 -> 9 ~ 1 conversion

  dV = map(dV, 1, 9, 300, 700);

  if (value > servoCenter) {
    value = (value - servoCenter) * exp((((double)value - servoCenter) - (inMax - servoCenter)) / dV ) + servoCenter;
    return value;
  } else {
    value = ppmMax - value + ppmMin;
    inMax = ppmMax - inMax + ppmMin;

    value = (value - servoCenter) * exp((((double)value - servoCenter) - (inMax - servoCenter)) / dV ) + servoCenter;
    return ppmMax - value + ppmMin;
  }
}
