int buttonPin =  22; 
int count = 0; 
int state = 0;
int state_time = 0;
long second_time;
int min_time;

// variables will change:

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  int buttonstate = digitalRead(buttonPin);
  int time1, time2;
  int my_time;
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
      Serial.println("count = 1");
      state_time = 1; }
    }
    
  if (count == 2) {
    if (state_time == 1) {
      time2 = millis();
      my_time = time2 - time1;
      second_time = my_time % 60;
      min_time = my_time / 60;
      
      Serial.println(my_time);
      Serial.print("분 : ");
      Serial.println(min_time);
      
      Serial.print("초 : ");
      Serial.println(second_time);      
      count = 0;
      state_time = 0; }
    }
}
