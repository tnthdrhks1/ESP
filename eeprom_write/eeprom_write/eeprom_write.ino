/*
   EEPROM Write

   Stores random values into the EEPROM.
   These values will stay in the EEPROM when the board is
   turned off and may be retrieved later by another sketch.
*/

#include "EEPROM.h"
int addr = 0;
#define EEPROM_SIZE 64
void setup()
{
  Serial.begin(115200);
  Serial.println("start...");
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }
  
}

void loop()
{
  for (int i = 0; i < EEPROM_SIZE; i++)
    {
      Serial.print(byte(EEPROM.read(i))); Serial.print(" ");
    }
  Serial.println( );
  Serial.println("--------------------------");

  delay(100);
}
