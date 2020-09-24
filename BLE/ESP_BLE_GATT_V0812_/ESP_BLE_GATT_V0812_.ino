#include "sys/time.h"
#include "string.h"
#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEBeacon.h>
#include <BLECharacteristic.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define BEACON_UUID           "8ec76ea3-6668-48da-9866-75be8bc86f4d" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
#define SERVICE_UUID          "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define IN_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define OUT_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26fa"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharIn;
BLECharacteristic *pCharOut;
BLEAdvertising *pAdvertising;

int i=0;

bool isConnected = false;

/**
 * Characteristic event callback
 */
class ServerEventCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Connected!");
    isConnected = true;
  }
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Disconnected!");
    isConnected = false;
  }
}; // ServerEventCallback

/**
 * Characteristic event callback
 */

 //global receivedData = '';
class InCharEventCallback: public BLECharacteristicCallbacks {
  
  void onWrite(BLECharacteristic* pChar) {
    Serial.print("InCharEventCallback - onWrite: ");
    Serial.println(pChar->getValue().c_str());
    
    char str_left[] = "left";
    char str1_right[] = "right";
    char str1_stop[] = "stop";
    char str1_emergency[] = "emergency";


    if(strcmp(str_left, pChar->getValue().c_str()) == 0){
      Serial.println("left");
    }
    if(strcmp(str1_right, pChar->getValue().c_str()) == 0){
      Serial.println("right");
    }
    if(strcmp(str1_stop, pChar->getValue().c_str()) == 0){
      Serial.println("stop"); 
    }
    if(strcmp(str1_emergency, pChar->getValue().c_str()) == 0){
      Serial.println("emergency");
    }
  }
}; // InCharEventCallback


class OutCharEventCallback: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic* pChar) {
    Serial.println("OutCharEventCallback - onRead: ");
    pChar->setValue("Hello World!");
  }
}; // OutCharEventCallback

InCharEventCallback *pCharInCallback;
OutCharEventCallback *pCharOutCallback;


void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE work!");

  initBle();
  startAdvertising();

}

void loop() {
  if(isConnected) {
    int input_max = 20;
    char input_char[] = {0x00, 0x00, 0x00, 0x00, 0x00, 
                            0x00, 0x00, 0x00, 0x00, 0x00, 
                            0x00, 0x00, 0x00, 0x00, 0x00, 
                            0x00, 0x00, 0x00, 0x00, 0x00};
    char input_caseA[] = {'A','B'};
    char input_caseB[] = {'C','D'};  
  /*
    while(Serial.available()) {
      char input = Serial.read();
      input_char[i] = input;
      i++;
      if(i >= input_max) break;
    }
    //내가 조건해주면// */
    if(i>3){
      pCharOut->setValue((uint8_t*)input_caseA, 2);     //2 >> size~ M20        //전태준의 강렬한 흔적
      pCharOut->notify();       
      Serial.println(input_caseA);
        
      Serial.println(i);
      i++;
    }
    else if(i<0){
      pCharOut->setValue((uint8_t*)input_char, i);                  //#1
      Serial.println("#1");
      pCharOut->notify();                                           //#2
      Serial.println("#2");
      
      Serial.println(i);
      i++;
    }
  }
  delay(1000);
}

void initBle() {
  // Create the BLE Device
  BLEDevice::init("");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerEventCallback());

  setService();
}

void startAdvertising() {
  // Create advertising manager
  pAdvertising = pServer->getAdvertising();

  // Set beacon data
  setBeacon();

  // Start advertising
  pAdvertising->start();
}

void stopAdvertising() {
  // Stop advertising
  pAdvertising->stop();
}

void setBeacon() {
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(1);
  oBeacon.setMinor(1);
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)

  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04
  
  std::string strServiceData = "";
  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData(); 
  oAdvertisementData.addData(strServiceData);

  oScanResponseData.setName("MyBLESPLOLIN");

  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

void setService() {
  pService = pServer->createService(SERVICE_UUID);
  pCharIn = pService->createCharacteristic(
                                         IN_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharOut = pService->createCharacteristic(
                                         OUT_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  pCharOut->addDescriptor(new BLE2902());
  
  pCharInCallback = new InCharEventCallback();
  pCharOutCallback = new OutCharEventCallback();
  pCharIn->setCallbacks(pCharInCallback);
  pCharOut->setCallbacks(pCharOutCallback);
  pService->start();
}
