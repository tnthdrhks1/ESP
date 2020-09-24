#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_cs 14 
#define TFT_reset 33 
#define TFT_dc 27 

int echo = 15;
int trig = 2;
char erase[5] = {};

//Adafruit_ILI9341 tft = Adafruit_ILI9341(cs, reset, dc, sdi, sck, LED, sdo);

Adafruit_ILI9341 tft = Adafruit_ILI9341 (TFT_cs, TFT_dc, TFT_reset );

void setup() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  
  Serial.begin(9600);
  SPI.begin();
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_RED);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(0,0);  
}

void loop(){
  tft.fillScreen(ILI9341_RED);
  long duration, distance;

  digitalWrite(trig, HIGH);                                             //tirg 핀에 HIGH신호를.
  delayMicroseconds(10);                                        //  딜레이 (작동중지 시간)
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration * 17 / 1000;

  tft.setTextSize(3);
  tft.setCursor(0,0);
  tft.print("distance is : ");
  tft.setCursor(0,50);
  tft.print(distance);
  delay(100);
  }
