#include "arduino_secrets.h"
#include "thingProperties.h" //Inkluderer filer fra Arduino Cloud 
#include <DHT.h> 
#define DHTPIN 3    //Definerer og inkulderer DHT sensor og bibliotek
#define DHTTYPE 11
#define MG_PIN                (0)     //Definerer forskjellige pins for CO2 sensoren
#define DC_GAIN               (8.5) 
#define READ_SAMPLE_INTERVAL  (50) 
#define READ_SAMPLE_TIMES     (50) 
#define ZERO_POINT_VOLTAGE    (3.940) //Setter referanse verdier til CO2 sensoren
#define REACTION_VOLTGAE      (1.940) 

DHT dht(DHTPIN,DHTTYPE);
int photoPin = A2;      //lyssensor pin
int airvalue = 1023;    //Jordfuktsmåleren sine verider i luft
int watervalue = 433;   //Jordfuktsmåleren sine verider i vann
int soilmoisttpin = A1; //Jordfuktsmåleren pin
int relePin = 2;        //Rele pin
float CO2Curve[3] = {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))}; //

void setup() {
  pinMode(photoPin, OUTPUT);  //Definerer pinen som en output verdi
  pinMode(relePin, OUTPUT);
  Serial.begin(9600);         //Lager en kontakt mellom pc og Arduino MKR
  dht.begin();                //Starter DHT sensorens fobindelse
  delay(1500); 
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

int needWater = 0;  //
int waitTime = 5;   //

void loop() {
  ArduinoCloud.update();
  float Temp = dht.readTemperature();   //Definerer de forskjellige variablene
  float Hum = dht.readHumidity();
  int readvalue = analogRead(photoPin);
  int sensorValue = analogRead(soilmoisttpin);
  float volts = MGRead(MG_PIN);
  int percentage = MGGetPercentage(volts,CO2Curve);
  jordfukt = map(sensorValue, airvalue, watervalue, 0, 100);
  temperatur = Temp;  //Overfører de forskjellige variablene til Arduino Cloud variabler
  LuftFukt = Hum;
  lys = readvalue;
  CO2 = percentage;
  Serial.println(lys);  //Printer de forksjellige verdiene for å kontrollere at data blir overført til Arduino cloud 
  Serial.println(LuftFukt);
  Serial.println(temperatur);
  Serial.println(jordfukt);
  Serial.println(CO2);
  
  if (40 > jordfukt > 0){  //Løkke for å vanne planten hvis den måler under 40% jordfuktighet
    needWater = needWater + 1;
    if (needWater >= waitTime){
      digitalWrite(relePin, HIGH); //Skrur på vannpumpa
      delay(4000);                 //Tiden pumpen er på
      digitalWrite(relePin, LOW);  //Skrur av vannpumpa
      needWater = 0;
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
