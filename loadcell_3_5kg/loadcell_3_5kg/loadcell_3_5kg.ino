#include "HX711.h"

HX711 scale1; //39

float calibration_factor1 = 7000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup() {
  Serial.begin(9600);
  scale1.begin(33,18); //dt 2 sck3 //39

 

  scale1.set_scale();
  scale1.tare();  //Reset the scale to 0
 
  long zero_factor1 = scale1.read_average(); 
}

void loop() {

  scale1.set_scale(calibration_factor1);

  Serial.print("앞:");
  Serial.print(scale1.get_units()/4, 3);
  Serial.print(" kg  ");
  Serial.println();

delay(500);
 
}
