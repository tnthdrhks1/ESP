#include <SPI.h> // SPI 라이브러리

 

int slave1 = 43; // 1번 슬레이브 (아두이노 우노)

 

void setup (void){

   pinMode(slave1, OUTPUT); // 1번 슬레이브 핀을 출력으로 설정


   SPI.begin (); // SPI 통신 초기화

   digitalWrite(slave1, HIGH); // 1번 슬레이브가 선택되지 않은 상태로 유지


   // 속도가 빠른 경우 데이터가 정확하게 전달되지 않으므로

   // 분주비를 높여 전송 속도를 낮춘다.

   SPI.setClockDivider(SPI_CLOCK_DIV16);

}

 

void loop (void){

   const char *p = "Hello, World\n"; // 슬레이브로 전송할 문자열

 

   digitalWrite(slave1, LOW); // 슬레이브를 선택한다.

 

    for (int i = 0; i < strlen(p); i++){

      SPI.transfer(p[i]); // 문자열 전송

    }

    digitalWrite(slave1, HIGH); // 슬레이브 선택을 해제한다.

 

   delay(1000);

}
