#include <EEPROM.h>

void setup() {
  // initialize the LED pin as an output.
   pinMode(13, OUTPUT);

   for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.write(i, 0);
   }
}

void loop() {
  // put your main code here, to run repeatedly:

}
