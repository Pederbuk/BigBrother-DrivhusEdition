const int Airvalue = 609;   //Air values from test
const int Watervalue = 256;  //Water values from test
int soilpinvalue = 0;
int soilpercent=0;  
void setup() {
  Serial.begin(9600);
}
void loop() {
  soilpinvalue = analogRead(A0);
  Serial.println(soilpinvalue);
  soilpercent = map(soilpinvalue, Airvalue, Watervalue, 0, 100);
  Serial.print(soilpercent);
  Serial.println("%");
}
