#include <DHT.h> 

#define DHTPIN 12
#define DHTTYPE 11

DHT dht(DHTPIN,DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  
  
}

void loop() {
  
  float Temp;
  float Hum;

  Temp = Temperature();
  Hum = Humidity();

  Serial.print("Temp;");
  Serial.print(Temp);
  Serial.print("ºC");
  Serial.print(" Hum:");
  Serial.print(Hum);
  Serial.print("\n");
  delay(1500);
}

float Temperature(){
  return dht.readTemperature();
}

float Humidity(){
  return dht.readHumidity();
}
