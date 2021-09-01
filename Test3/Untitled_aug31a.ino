#include "arduino_secrets.h"
#include "thingProperties.h"
#include <DHT.h> 
#define DHTPIN 3
#define DHTTYPE 11
#define MG_PIN                (0)     
#define DC_GAIN               (8.5) 
#define READ_SAMPLE_INTERVAL  (50) 
#define READ_SAMPLE_TIMES     (50) 
#define ZERO_POINT_VOLTAGE    (3.940) 
#define REACTION_VOLTGAE      (1.940)

DHT dht(DHTPIN,DHTTYPE);
int photoPin = A2; //lys
int airvalue = 1023; //Tørt
int watervalue = 433; //Vått
int soilmoisttpin = A1;
int relePin = 2;
float CO2Curve[3] = {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};

void setup() {
  pinMode(photoPin, OUTPUT);
  pinMode(relePin, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  delay(1500); 
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

int needWater = 0;
int waitTime = 5;

void loop() {
  ArduinoCloud.update();
  float Temp = dht.readTemperature();
  float Hum = dht.readHumidity();
  int readvalue = analogRead(photoPin);
  int sensorValue = analogRead(soilmoisttpin);
  float volts = MGRead(MG_PIN);
  int percentage = MGGetPercentage(volts,CO2Curve);
  jordfukt = map(sensorValue, airvalue, watervalue, 0, 100);
  temperatur = Temp;
  LuftFukt = Hum;
  lys = readvalue;
  CO2 = percentage;
  Serial.println(lys);
  Serial.println(LuftFukt);
  Serial.println(temperatur);
  Serial.println(jordfukt);
  Serial.println(CO2);
  
  if (50 > jordfukt > 0){
    needWater = needWater + 1;
    if (needWater >= waitTime){
      digitalWrite(relePin, HIGH);
      delay(3000);
      digitalWrite(relePin, LOW);
      needWater = 0;
    }
  } 
  else if (jordfukt < 50){
    if (relePin == HIGH){
      needWater = 0;
      digitalWrite(relePin, LOW);
    } 
  } 
  else {
    needWater = 0;
  }
  delay(1000);
}

float MGRead(int mg_pin)
{
    int i;
    float v=0;

    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *5/1024 ;
    return v;  
}

int  MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else { 
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
}
