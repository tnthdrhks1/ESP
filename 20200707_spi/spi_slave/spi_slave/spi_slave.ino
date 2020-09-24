#include <SPI.h> // SPI 라이브러리

 

char buf[100]; // 수신된 문자 저장을 위한 버퍼

// pos와 process_it은 인터럽트 처리 루틴에서 값을 바꾸는 변수이므로

// volatile 선언을 통해 업데이트된 값이 정확하게 반영되도록 한다.

volatile byte pos; // 수신 버퍼에 문자를 기록할 위치

volatile boolean process_it; // 개행 문자를 만난 경우 출력하기 위한 플래그

 

void setup (void){

   Serial.begin (9600); // 수신 문자열 출력을 위한 직렬 통신 초기화

   // 디지털 핀은 디폴트값으로 입력으로 설정되어 있으므로

   // MOSI, SCLK, SS는 입력으로 설정하지 않아도 된다.

   pinMode(MISO, OUTPUT);

 

    // 마스터의 전송 속도에 맞추어 통신 속도를 설정한다.

   SPI.setClockDivider(SPI_CLOCK_DIV16);

 

   // SPI 통신을 사용할 수 있도록 레지스터를 설정

   // SPCR : SPI Control Register

  SPCR |= _BV(SPE); // SPE : SPI Enable

 

   // SPI 통신에서 슬레이브로 동작하도록 설정

   SPCR &= ~_BV(MSTR); // MSTR : Master Slave Select

 

   pos = 0; // 버퍼가 비어 있으므로 0번부터 수신 문자 기록

   process_it = false; // Serial로 출력할 문자열 없음

 

    // SPI 통신으로 문자가 수신될 경우 인터럽트 발생을 허용

   SPCR |= _BV(SPIE); // SPIE : SPI Interrupt Enable

}

 

// SPI 통신으로 문자가 수신될 때 발생하는 인터럽트 처리 루틴

ISR (SPI_STC_vect)

{

   byte c = SPDR; // 수신된 문자를 얻어온다.

   if (pos < sizeof(buf)){ // 현재 버퍼에 저장할 공간이 있는 경우

     buf[pos++] = c; // 버퍼에 수신된 문자 기록


        

      if (c == '\n'){ // 개행 문자를 만나면 수신된 문자열을 Serial로 출력

        process_it = true;

      }

   }

}

 

void loop (void){

   if (process_it){ // Serial로 출력할 문자열이 있는 경우

     buf[pos] = 0; // 문자열의 끝 표시

     Serial.print(buf); // 문자열을 Serial로 출력

     pos = 0; // 버퍼가 비었음을 표시

     process_it = false; // Serial로 출력할 문자가 없음을 표시

   }

}
