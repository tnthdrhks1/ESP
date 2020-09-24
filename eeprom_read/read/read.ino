int row = 0;

void setup()

{

    Serial.begin(9600);

    Serial.println("CLEARDATA");

    Serial.println("LABEL,Time,temp");

 

}

 

void loop()

{

    //온도 측정

   Serial.print("DATA,TIME,");//현재 시간을 출력

   Serial.println(getTemp());

   row ++;

 

   if(row  == 1000) //1000개의 데이터가 쌓이면 아래와 같이 처리한다.

   {

     row = 0 ;

     Serial.println("ROW,SET,2"); // 액셀의 2번째 행으로 커서를 이동 시킨다.

   }

}

 

int getTemp()

{

    // 온도 측정 센서에 따라 온도가 출력되도록 프로그래밍한다.

   int tempVal = 0;

   return tempVal;

}
