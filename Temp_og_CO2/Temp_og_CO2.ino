/************************Hardware Related Macros************************************/
#define         MG_PIN                       (0)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (2)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier Originalt 8.5

/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (50)     //define the time interval(in milisecond) between each samples in 
                                                     //normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
//Disse verdiene er ikke kalibrert ordentlig, bare gjetninger for rundt 400ppm
#define         ZERO_POINT_VOLTAGE           (0.270) //define the output of the sensor in volts when the concentration of CO2 is 400PPM    originalt 0.220 fungerer med 0.270
#define         REACTION_VOLTGAE             (0.050) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2   Originalt 0.020

//Biblioteker vi trenger
#include <DHT.h> // Library for temp sensor
#include <LiquidCrystal_I2C.h> // Library for skjerm
#include <Wire.h> // Library for kommunikasjon protokollen I2C

//Definerer konstanter
#define DHTPIN 4    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


/*****************************Globals***********************************************/
float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};   
                                                     //two points are taken from the curve. 
                                                     //with these two points, a line is formed which is
                                                     //"approximately equivalent" to the original curve.
                                                     //data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280) 
                                                     //slope = ( reaction voltage ) / (log400 –log1000) 

//Soilpin
int soilpin = A3;

//Temp and hum
float temp(){
  return dht.readTemperature();
}
float hum(){
  return dht.readHumidity();
}

//Water pump
int relePin = 2;

void setup()
{

    Serial.begin(9600);
    dht.begin();
    delay(100);
    lcd.init();                              //UART setup, baudrate = 9600bps
    pinMode(BOOL_PIN, INPUT);                        //set pin to input
    digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors

    pinMode(relePin, OUTPUT);     //Assigns the relepin as output
}

void loop()
{
    int percentage;
    float volts;
    int value = analogRead(A1); 

    
    volts = MGRead(MG_PIN);
    Serial.print( "SEN-00007:" );
    Serial.print(volts); 
    Serial.print( "V           " );

    percentage = MGGetPercentage(volts,CO2Curve);
    Serial.print("CO2:");
    if (percentage == -1) {
        Serial.print( "<400" );
    } else {
        Serial.print(percentage);
    }

    Serial.print( "ppm" );  
    Serial.print("\n");
    Serial.print("Analog Value :");
    Serial.println(value);
    Serial.print("Jordfunktighet: ");
    Serial.println(analogRead(soilpin));

    Serial.print("Temp: ");
    Serial.print(temp());
    Serial.println(" *C"); 
    Serial.print("Hum: ");
    Serial.print(hum());
    Serial.println(" %RH");
/*
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
*/


    if (digitalRead(BOOL_PIN) ){
        Serial.print( "=====BOOL is HIGH======" );
    } else {
        Serial.print( "=====BOOL is LOW======" );
    }

    Serial.print("\n");

    delay(200);
    PrintOnScreen();


  
 
}

/*****************************  MGRead *********************************************
Input:   mg_pin - analog channel
Output:  output of SEN-000007
Remarks: This function reads the output of SEN-000007
************************************************************************************/ 
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



/*****************************  MQGetPercentage **********************************
Input:   volts   - SEN-000007 output measured in volts
         pcurve  - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(MG-811 output) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
************************************************************************************/ 
int  MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else { 
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
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
