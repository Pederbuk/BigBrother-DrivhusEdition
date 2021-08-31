#define         MG_PIN                       (0)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (2)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier Originalt 8.5
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (50)     //define the time interval(in milisecond) between each samples in 
#define         ZERO_POINT_VOLTAGE           (0.270) //define the output of the sensor in volts when the concentration of CO2 is 400PPM    originalt 0.220 fungerer med 0.270
#define         REACTION_VOLTGAE             (0.050) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2   Originalt 0.020
float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};   

void setup()
{
    Serial.begin(9600);
    dht.begin();
    delay(100);
    pinMode(BOOL_PIN, INPUT);                        //set pin to input
    digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors
}

void loop()
{
    int percentage;
    float volts;
    volts = MGRead(MG_PIN);
    percentage = MGGetPercentage(volts,CO2Curve);
    
    delay(200);
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