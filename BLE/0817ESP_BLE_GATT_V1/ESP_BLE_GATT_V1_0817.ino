#include "sys/time.h"
#include "string.h"
#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEBeacon.h>
#include <BLECharacteristic.h>
#include <BLE2902.h>

/*************************************************************************************************************
                                                    BLE_SET  
 *************************************************************************************************************/

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

/*
 * Service event callback
 */
bool isConnected = false;
 
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

/*************************************************************************************************************
                                                 HELMET_SET 
 *************************************************************************************************************/
const int press_sensor = 34; 
const int alcohol_sensor = 32;

int alcohol_cnt = 1;
int num = 10;

boolean holdhit_stn = false;
boolean helmet_stn = false;
boolean alcohol_stn = false;
boolean driver_stn = false;


/************ONOFF_SWITCH************/
const int onoff_sw_pin = 27;
boolean onoff_sw_pst = LOW;
boolean onoff_sw_crt = LOW;
boolean onoff_sw_stn = false;
/************Alcohol_LED************/
int al_led_b = 12;
int al_led_g = 13;
int al_led_r = 14;

boolean al_led_stn_b = false;
/************direction_LED************/

int dr_led_bl = 4;
int dr_led_g =15;
int dr_led_r = 2;
int dr_led_br = 0;

/************time********************/
unsigned long timeCurr;
unsigned long timePrev; 
unsigned long hold_timeCurr;
unsigned long hold_timePrev;

/*************************************************************************************************************
******************************************     S E T T I N G    **********************************************
 *************************************************************************************************************/

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE work!");

  initBle();
  startAdvertising();

  pinMode(onoff_sw_pin, INPUT);

  pinMode(al_led_b, OUTPUT);
  pinMode(al_led_g, OUTPUT);
  pinMode(al_led_r, OUTPUT);
  
}

/*************************************************************************************************************
******************************************       M A I N      **************************************************
 *************************************************************************************************************/
void loop() {
  if(isConnected) {
    int input_max = 20;

    char input_caseA[] = {'P','E','R','M','I','T'};
    
    if(i<3){
      pCharOut->setValue((uint8_t*)input_caseA, 6);     //2 >> size~ M20        //전태준의 강렬한 흔적
      pCharOut->notify();       
      Serial.println(input_caseA);
      i++;  
      Serial.println(i);
    }
    else if(i==0){
      pCharOut->setValue((uint8_t*)input_char, i);                  //#1
      Serial.println("#1");
      pCharOut->notify();                                           //#2
      Serial.println("#2");
    }
  }
  delay(1000);
}
/*********************************  H E L M E T    F U C T I O N ************************************************/
/*
 * alcohol_delay
 */
void alcohol_delay()
{
  for(int i= 0 ; i < num ;i++)
  {
    Serial.println("delay num:");
    Serial.println(num);
/* 카운트 언제까지 가는지 체크하고 조건달아줘야함.
    if(alc_cnt 
*/
    delay(1000);
  }
}
/*
 * alcohol value check 
 */
boolean alcohol()
{  
   int alcohol_sensor_val = analogRead(alcohol_sensor);   
   if(alcohol_stn ==false)Serial.println("alcohol_V :" +String(alcohol_sensor_val));
   
   if(alcohol_sensor_val <= 130){ // 후 부호반대 수정
      alcohol_stn = true;
        return true,alcohol_stn;
   }
   else if(alcohol_sensor_val>130 )
   {
      alcohol_stn = false;
      al_ledr();
      alcohol_cnt += 1;
      return false,alcohol_stn,alcohol_cnt; 
   }
   al_ledoff();
}
/*
 * Helmet value 
 */
boolean helmet()
{
    int press_sensor_val = analogRead(press_sensor);  
    //if(helmet_stn == false) Serial.println("Press_V : "+String(press_sensor_val));
    if(press_sensor_val < 50 )
    {
      helmet_stn = false;
      return false, helmet_stn;
    }
    if(press_sensor_val >= 50 )//&& helmet_stn==false)//수정해야되//
    { 
      //if(helmet_stn ==true)
      
      //analogWrite(ctr_led,brightness);
      helmet_stn = true;
      return true, helmet_stn;
    }  
}

/******************************hold_heating********************************************/
bool hold_heating()
{
 
  for(int i =0 ; i <3 ; i++)
  {  
    digitalWrite(al_led_b, HIGH);
    delay(1000);  
  }
  delay(500);
  al_ledg();
  al_ledoff();
  holdhit_stn = true;
  return holdhit_stn;
}


/******************************SWITCH********************************************/
void onoff_sw_f()
{
  onoff_sw_crt = digitalRead(onoff_sw_pin);
  if(onoff_sw_pst == LOW && onoff_sw_crt == HIGH)
  {
    onoff_sw_stn =  !(onoff_sw_stn);
  }
  onoff_sw_pst = onoff_sw_crt;
}

void onoff_sw_t()
{
  //onoff_sw_crt = digitalRead(onoff_sw_pin);
  if(onoff_sw_pst == HIGH && onoff_sw_crt == LOW)
  {
    onoff_sw_stn =! onoff_sw_stn;
  }
  onoff_sw_pst = onoff_sw_crt;
}
/******************************ALCOHOL LED********************************************/

void al_ledg(){
  digitalWrite(al_led_g, HIGH);
  digitalWrite(al_led_b, LOW);
  digitalWrite(al_led_r, LOW);
  delay(500);
}
void al_ledb(){
  digitalWrite(al_led_g, LOW);
  digitalWrite(al_led_b, HIGH);
  digitalWrite(al_led_r, LOW);
  delay(500);
}
void al_ledr(){
  digitalWrite(al_led_g, LOW);
  digitalWrite(al_led_b, LOW);
  digitalWrite(al_led_r, HIGH);
}
void al_ledoff(){
  digitalWrite(al_led_g, LOW);
  digitalWrite(al_led_b, LOW);
  digitalWrite(al_led_r, LOW);
}


/******************************DRIVE__PERMIT***********************************/

bool drive_permit()
{
   if ( helmet_stn==false || alcohol_stn==false)
    {
      driver_stn = false;
      al_ledr();
      delay(500);
      al_ledoff();
      return false, driver_stn;
    }
   else if (helmet_stn==true && alcohol_stn==true )
   {  
      
      ESP_BT.write('1');
      //BTSerial.write('1');
      driver_stn = true;
      
      return driver_stn;
    }
    
}

/*************************DIRECT_LIGHT*************************************/

void direct_light()
{
  char val = ESP_BT.read();
  if(ESP_BT.available())
  { 
      Serial.write(ESP_BT.read());   
  }
  
  delay(100);
  //Serial.println(val);  
  switch(val)
  {
      direct_led_off();
      
      case 'r':
        Serial.println("좌향"); //후 이줄을 led 출력으로 수정
        direct_led_left_d();
        break;
      case 'l':
        Serial.println("우향");
        direct_led_right_d();
        break;
      case 'e':
        Serial.println("비상등");
        direct_led_emergency_d();
        break;   
      case 'R':
        Serial.println("좌향"); //후 이줄을 led 출력으로 수정
        direct_led_left_s();
        break;
      case 'L':
        Serial.println("우향");
        direct_led_right_s();
        break;
      case 'E':
        Serial.println("비상등");
        direct_led_emergency_s();
        break;    
      case 's':
        Serial.println("브레이크");
        direct_led_stop();
        break;       
      case 'd':
        Serial.println("주행");
        direct_led_drive();
        break; 
      case 'f':
        Serial.println("Drive off");
        direct_led_off();
        break;       

      
  } 
  direct_led_off();
}



/******************************Alcohol LED********************************************/
/*          Yellow : hold_heating, Waiting      Green : Check_allow     Red : Drinking*/
void direct_led_drive()
{
  digitalWrite(dr_led_g, HIGH);
  digitalWrite(dr_led_bl, LOW);
  digitalWrite(dr_led_r, LOW);
  digitalWrite(dr_led_br, LOW);
  delay(500);
}
void direct_led_left_d()
{
  digitalWrite(dr_led_g, HIGH);
  digitalWrite(dr_led_bl, HIGH);
  digitalWrite(dr_led_r, LOW);
  digitalWrite(dr_led_br, LOW);
  delay(500);
}
void direct_led_right_d()
{
  digitalWrite(dr_led_g, HIGH);
  digitalWrite(dr_led_bl, LOW);
  digitalWrite(dr_led_r, LOW);
  digitalWrite(dr_led_br, HIGH);
  delay(500);
}
void direct_led_emergency_d()
{
  digitalWrite(dr_led_g, HIGH);
  digitalWrite(dr_led_bl, HIGH);
  digitalWrite(dr_led_r, LOW);
  digitalWrite(dr_led_br, HIGH);
  delay(500);
}
void direct_led_left_s()
{
  digitalWrite(dr_led_g, LOW);
  digitalWrite(dr_led_bl, HIGH);
  digitalWrite(dr_led_r, HIGH);
  digitalWrite(dr_led_br, LOW);
  delay(500);
}
void direct_led_right_s()
{
  digitalWrite(dr_led_g, LOW);
  digitalWrite(dr_led_bl, LOW);
  digitalWrite(dr_led_r, HIGH);
  digitalWrite(dr_led_br, HIGH);
  delay(500);
}
void direct_led_emergency_s()
{
  digitalWrite(dr_led_g, LOW);
  digitalWrite(dr_led_bl, HIGH);
  digitalWrite(dr_led_r, HIGH);
  digitalWrite(dr_led_br, HIGH);
  delay(500);
}
void direct_led_off()
{
  digitalWrite(dr_led_g, LOW);
  digitalWrite(dr_led_bl, LOW);
  digitalWrite(dr_led_r, LOW);
  digitalWrite(dr_led_br, LOW);
  delay(500);
}
void direct_led_stop()
{
  digitalWrite(dr_led_g, LOW);
  digitalWrite(dr_led_bl, LOW);
  digitalWrite(dr_led_r, HIGH);
  digitalWrite(dr_led_br, LOW);
  delay(500);
}
/********************************************  BLE _ FUCTION  **************************************************/
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
