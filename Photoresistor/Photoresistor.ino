int photoPin = A0;

void setup() {
  pinMode(photoPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int readValue = analogRead(photoPin); 
  Serial.println("Analog Value :");
  Serial.println(readValue);  
  delay(500);
}
