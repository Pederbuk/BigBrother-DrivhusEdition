int soilpin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(soilpin));
  delay(1000);
}
