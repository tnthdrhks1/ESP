int trig = 2;                                      //trig핀을 디지털 2번핀에
int echo = 15;                                  // echo핀을 디지털 3번핀에

void setup()
{
  Serial.begin(9600);                          //시리얼 통신을, 9600속도로
  pinMode(trig, OUTPUT);                 // tirg (2번) 핀을 출력모드로
  pinMode(echo, INPUT);                 //  echo (3번) 핀을 입력모드로 
}

void loop()
{
  long vol;
  long duration, distance;
  digitalWrite(trig, HIGH);                                             //tirg 핀에 HIGH신호를.
  delayMicroseconds(10);                                        //  딜레이 (작동중지 시간)
  digitalWrite(trig, LOW);
  
  duration = pulseIn(echo, HIGH); // tirg 핀에 LOW신호를
  distance = duration * 17 / 1000;

  Serial.println(distance);
  delay(10);
}
