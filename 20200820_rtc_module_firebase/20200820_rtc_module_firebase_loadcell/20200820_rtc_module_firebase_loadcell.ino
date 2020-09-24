
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


#include <WiFi.h>
#include <FirebaseESP32.h>
#include "HX711.h"

//1. Change the following info
#define FIREBASE_HOST "test-20200820.firebaseio.com"
#define FIREBASE_AUTH "RMrmGZhov14j1l2MfePIvvTkTcleBQTMlUP1LymG"
#define WIFI_SSID "KNU_WLAN_Open"
#define WIFI_PASSWORD "beyondme"

HX711 scale1; //39
int weight = 0;

float calibration_factor1 = 200000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData firebaseData;

IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "pool.ntp.org";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

unsigned long delayStart = 0; // the time the delay started
unsigned long delaySynchro = 0; // the time the delay started
byte hours = 0;
byte minutes = 0;
byte seconds = 0;

char c;

void setup()
{

  Serial.begin(115200);

  scale1.begin(12,32); //dt 2 sck3 //39
  scale1.set_scale();
  scale1.tare();  //Reset the scale to 0

  delay(1000);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  long zero_factor1 = scale1.read_average();
  
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

  String path = "/loadcell_test";
  
  //3. Set your Firebase info

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);

  //5. Try to set int data to Firebase
  //The set function returns bool for the status of operation
  //firebaseData requires for sending the data
  while(1){
  scale1.set_scale(calibration_factor1);

  getTime();

  weight = scale1.get_units()*1000;
  
  Serial.print("현재 무게 : ");
  Serial.print(weight);
  Serial.println(" g  ");
  Serial.println();

  

  if(Firebase.setInt(firebaseData, path + "/time" + hours + ":" + minutes + ":"+ seconds, weight))
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

void getTime(){
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  }
  else {
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:


    // print the hour, minute and second:
    Serial.print("현재시간  ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    hours = (epoch  % 86400L) / 3600;
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    minutes = ((epoch  % 3600) / 60);
    
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
    seconds = epoch % 60;
  }
}

unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
