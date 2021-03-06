//+++++++++++++++++++++++++++++++++++++++++++++++++
// Read input buttons status and send value to loop
//+++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char readKeys() {

  // buttonSelect
  if ((PIND & (1 << buttonSelect)) == 0) {
    delay(10);
    return 2;
  }
  // buttonUp
  if ((PIND & (1 << buttonUp)) == 0) {
    delay(10);
    return 1;
  }
  // buttonDown
  if ((PIND & (1 << buttonDown)) == 0) {
    delay(10);
    return 3;
  }

  return 0;

}
