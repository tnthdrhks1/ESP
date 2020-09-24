#define led 22
#define InPin 34

float x, value ;
const int freq = 5000;
const int ledChannel = 2;
const int resolution = 10;

void setup() {
   Serial.begin(9600);
   pinMode(led, OUTPUT);
   ledcSetup(ledChannel, freq, resolution);
   ledcAttachPin(led, ledChannel);
}

void loop()
{
  x = analogRead(InPin);
  value = (x/4095) * 3.7;
  Serial.println(x);
  ledcWrite(ledChannel, x/4);
  delay(1000);
}
