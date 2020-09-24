#include "HX711.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

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
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void setup() {
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
    }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return;
    }
    
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
      appendFile(SD, "/plate.csv", "-----------음식무게 측정-------------- \n");
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
