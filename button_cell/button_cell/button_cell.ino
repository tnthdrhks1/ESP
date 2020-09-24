#include "HX711.h"
HX711 scale;
float calibration_factor = 200000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작 + or - 어떤 값이 될지 모름

int buttonPin =  22; 
int count = 0; 
int state = 0;
int state_time = 0;
long second_time;
int min_time;
float weight1, weight2;

// variables will change:

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  scale.begin(33,18); //dt 2 sck3

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {  
  int buttonstate = digitalRead(buttonPin);
  int time1, time2;
  int my_time;
  scale.set_scale(calibration_factor); 

  //button
  if (buttonstate == LOW) {
    if (state == 0){
      delay(10);
      state = 1;}
    }
  if (buttonstate == HIGH) {
    if (state == 1){
      count = count + 1;
      state = 0;}
    }

  //time
  if (count == 1) {
    if (state_time == 0) {
      time1 = millis();
      weight1 = scale.get_units();

      Serial.println("-----------음식무게 측정--------------");
      Serial.println("식사시작");
      Serial.print("식사전 음식무게 : ");
      Serial.print(weight1, 3); 
      Serial.println(" kg"); 
      Serial.println(" "); 
      state_time = 1; }
    }
    
  if (count == 2) {
    if (state_time == 1) {
      time2 = millis();
      weight2 = scale.get_units();
      
      Serial.println("식사종료");
      Serial.print("식사후 음식무게 : ");
      Serial.print(weight2, 3); 
      Serial.println(" kg"); 

      Serial.print("섭취한 음식무게 : ");
      Serial.println(weight1 - weight2, 3); 

      Serial.println(" "); 
      my_time = time2 - time1;
      second_time = my_time % 60;
      min_time = my_time / 60;
      
      Serial.println("-----------식사 시간--------------");
      
      Serial.print("분 : ");
      Serial.println(min_time);
      
      Serial.print("초 : ");
      Serial.println(second_time);      
      count = 0;
      state_time = 0; }
    }

}
