int thresholdUp = 400;
int thresholdDown = 250;


int sensorPin = A0;


void setup(){
  Serial.begin(9600);
}

void loop(){
  
  String DisplayWords;
  String WaterLevel;
  

  int sensorValue;
  sensorValue = analogRead(sensorPin);

  
  WaterLevel = String(analogRead(sensorPin), DEC);
  Serial.print("Water Level:");
  Serial.println(WaterLevel);
  

  if (sensorValue < thresholdDown){
    String DisplayWords = "Dry, Water it!";
    Serial.print(DisplayWords);
    

  
  } else if (sensorValue > thresholdUp){
    String DisplayWords = "Wet, Leave it!";
    Serial.print(DisplayWords); 
  

  } else {
    Serial.print(sensorValue);
  }
  delay(1000);
}
