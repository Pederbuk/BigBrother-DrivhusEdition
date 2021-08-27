//Biblioteker vi trenger
#include <DHT.h> // Library for temp sensor
#include <LiquidCrystal_I2C.h> // Library for skjerm
#include <Wire.h> // Library for kommunikasjon protokollen I2C

//Definerer konstanter
#define DHTPIN 4    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  delay(100);
  lcd.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  PrintOnScreen();
}

//Funksjoner for å hente ut data
float temp(){
  return dht.readTemperature();
}
float hum(){
  return dht.readHumidity();
}

//Funksjon for å printe på lcd skjerm
void PrintOnScreen(){
  float t = temp();
  float h = hum();
  lcd.backlight();
  bool Cleared = true;

  lcd.setCursor(0,0);
  lcd.print("Temp: ");  
  lcd.setCursor(8,0);  
  lcd.print(t);  
  lcd.setCursor(13,0);  
  lcd.print("*C");  
  lcd.setCursor(0,1);  
  lcd.print("Hum :");  
  lcd.setCursor(8,1);  
  lcd.print(h);  
  lcd.setCursor(13,1);
  lcd.print("%RH");
}
