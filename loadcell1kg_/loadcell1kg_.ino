#include "HX711.h"

HX711 scale1; //39

float calibration_factor1 = -200000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup() {
  Serial.begin(115200);
  scale1.begin(22,33); //dt 2 sck3 //39

 

  scale1.set_scale();
  scale1.tare();  //Reset the scale to 0
 
  long zero_factor1 = scale1.read_average(); 

  while(1){
  scale1.set_scale(calibration_factor1);

  uint32_t weight = 1000 * scale1.get_units();
  
  Serial.print("앞:");
  Serial.print(weight);
  Serial.print(" kg  ");
  Serial.println();

  delay(500);
  }
}

void loop() {
 
}
