int t1 = 15;
int t2 = 37;
int t3 = 00;

//RTC_DATA_ATTR 

RTC_DATA_ATTR int wakeupCnt = 0;
int wakeupCnt1 = t1 * 3600 + t2 * 60 + 25;
  
void setup(){ // At sleep wake-up programme starts from here!
  Serial.begin(115200);
  while(1){
  wakeupCnt = wakeupCnt + 1;
  int now = wakeupCnt1 + wakeupCnt;
  
  Serial.print(now/3600);
  Serial.print(":");
  Serial.print((now%3600)/60);
  Serial.print(":");
  Serial.println(now%60);
  
  esp_sleep_enable_timer_wakeup(1 * 1000000);
  esp_deep_sleep_start();
}
}
void loop(){
  // This section will never run! 
}
