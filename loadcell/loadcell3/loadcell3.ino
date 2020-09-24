int voltage;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  voltage = analogRead(33);
  Serial.println(voltage);
  delay(1000);
}
 
