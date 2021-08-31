const int AirValue = 609;   //Air values from test
const int WaterValue = 256;  //Water values from test
int soilMoistureValue = 0;
int soilmoisturepercent=0;  
void setup() {
  Serial.begin(9600);
}
void loop() {
  soilMoistureValue = analogRead(A0);
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.print(soilmoisturepercent);
  Serial.println("%");
}
