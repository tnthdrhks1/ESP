#include "HX711.h"

int t1 = 15;
int t2 = 53;
int t3 = 00;

RTC_DATA_ATTR int wakeupCnt = 0;
int wakeupCnt1 = t1 * 3600 + t2 * 60 + 25;

HX711 scale1; //39
float calibration_factor1 = 200000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

void setup(){ // At sleep wake-up programme starts from here!
  Serial.begin(115200);

  delay(500);
  
  scale1.begin(12,32); //dt 2 sck3 //3
  scale1.set_scale();
  scale1.tare();  //Reset the scale to 0

  wakeupCnt = wakeupCnt + 2;
  int now = wakeupCnt1 + wakeupCnt;

  scale1.set_scale(calibration_factor1);
  
  while(1){
  delay(1000); 
  
  Serial.print(now/3600);
  Serial.print(":");
  Serial.print((now%3600)/60);
  Serial.print(":");
  Serial.println(now%60);

  Serial.print(scale1.get_units(), 3);
  Serial.print(" kg  ");
  Serial.println(); 
  esp_sleep_enable_timer_wakeup(1000);
  esp_deep_sleep_start();
  
  delay(500);
}
}
void loop(){
  // This section will never run! 
}
