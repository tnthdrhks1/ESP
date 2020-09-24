#include <DS1302.h>
#include "HX711.h"

HX711 scale1; //39

int weight = 0;

float calibration_factor1 = 200000;    //로드셀 종류나 상황에 따라 적당한 값으로 시작

// RTC 모듈과 아두이노의 연결
const int RST   = 33;   // RESET(RST)핀 (Chip Enable)
const int DATA   = 21;   // DATA핀 (I/O)
const int CLOCK = 18;   // Clock 핀 (Serial Clock)

//  DS1302 객체 생성
DS1302 rtc(RST, DATA, CLOCK);

// 일요일~토요일까지 알맞는 문자열 반환
String dayAsString(const Time::Day day)
{
 switch (day)
 {
   case Time::kSunday: return "Sun";
   case Time::kMonday: return "Mon";
   case Time::kTuesday: return "Tue";
   case Time::kWednesday: return "Wed";
   case Time::kThursday: return "Thu";
   case Time::kFriday: return "Fri";
   case Time::kSaturday: return "Sat";
 }
 return "(unknown day)";
}

void printTime() // 날짜 출력 함수
{
// 칩에서 현재 시간과 날짜를 가져옴
Time t = rtc.time();

// 요일 코드에 알맞는 문자열 가져옴
const String day = dayAsString(t.day);

// 날짜와 시간 정보를 형식화하고 임시 버퍼에 형식 넣음
char buf[50];
snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d  %s",
         t.yr, t.mon, t.date,
         t.hr, t.min, t.sec,
         day.c_str());
        
// 시간을 볼 수 있도록 형식화된 문자열을 출력
Serial.println(buf);
}

void setup()
{
  Serial.begin(115200);

  scale1.begin(12,32); //dt 2 sck3 //39

 

  scale1.set_scale();
  scale1.tare();  //Reset the scale to 0
 
  long zero_factor1 = scale1.read_average(); 


 /* 아래 1~4번의 작업은 처음 날짜를 세팅하고 업로드 한후,
    주석 처리하여 다시 업로드 해야 합니다.
    그렇지 않으면 아두이노가 off->on 될 때 마다 시간을 초기화 합니다.*/

 /*1. 쓰기 방지 기능을 끈다.*/
 //rtc.writeProtect(false);

 /*2. 동작 모드로 설정 */
 //rtc.halt(false);

 /*3. 새로운 시간과 날짜를 세팅
        2019년, 1월, 12일, 01시, 25분, 00초, 월요일*/
 //Time t(2020, 8, 20, 17, 47, 30, Time::kThursday);

 /*4. 칩에 시간과 날짜를 설정*/
 //rtc.time(t);
}

void loop()
{
 printTime(); //날짜와 시간을 출력

 scale1.set_scale(calibration_factor1);

 Serial.print("앞:");
 Serial.print(scale1.get_units(), 3);
 Serial.print(" kg  ");
 Serial.println();
  
 delay(1000); // 1초 딜레이
}
