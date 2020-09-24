int trig = 5;                                      //trig핀을 디지털 2번핀에
int echo = 22;  
float a;
float b;

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
  
  if(digitalRead(echo) == HIGH)
    {b = micros();
    Serial.print("b = ");
    Serial.println(b);
    delay(5);
    if(digitalRead(echo) == LOW)
      {
        a = micros();
        Serial.print("a = ");
        Serial.println(a);
        delay(5);
        }
    duration = a - b;
    }

  distance = duration * 17 / 1000;


  delay(10);
}
