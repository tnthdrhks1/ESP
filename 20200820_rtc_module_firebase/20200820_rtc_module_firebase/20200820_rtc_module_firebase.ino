
/*
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2020 mobizt
 * 
 * This example is for the beginner
 *
*/


#include <FirebaseESP32.h>

//1. Change the following info
#define FIREBASE_HOST "test-20200820.firebaseio.com"
#define FIREBASE_AUTH "RMrmGZhov14j1l2MfePIvvTkTcleBQTMlUP1LymG"
#define WIFI_SSID "KNU_WLAN_Open"
#define WIFI_PASSWORD "beyondme"

int num = 0;

//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData firebaseData;


void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  String path = "/Test";
  
  //3. Set your Firebase info

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);

  //5. Try to set int data to Firebase
  //The set function returns bool for the status of operation
  //firebaseData requires for sending the data
  while(1){

  num++;
  
  if(Firebase.setInt(firebaseData, path + "/LED_Status"+num, num))
  {
    //Success
     Serial.println("Set int data success");

  }else{
    //Failed?, get the error reason from firebaseData

    Serial.print("Error in setInt, ");
    Serial.println(firebaseData.errorReason());
  }

  delay(500);
}
}
void loop()
{
}
