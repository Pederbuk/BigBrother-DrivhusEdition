int thresholdUp = 550; //Tørt    //Veldig tørr jord ute = 557 og 540
int thresholdDown = 300; //Vått
int sensorPin = A0;
int relePin = 2; //Water pump
int needWater = 0;
int waitTime = 5;

void setup(){
  Serial.begin(9600);
  pinMode(relePin, OUTPUT); 
}
void loop(){
  String DisplayWords;
  String WaterLevel;  
  int sensorValue;
  sensorValue = analogRead(sensorPin);
  
  WaterLevel = String(analogRead(sensorPin), DEC);
  Serial.print("Water Level:");
  Serial.println(WaterLevel);

  if (sensorValue > thresholdUp){
    String DisplayWords = "Dry, Water it!";
    needWater = needWater + 1;
    if (needWater >= waitTime){
      StartPump();
      delay(2000);
      StopPump();
      needWater = 0;
    }
    Serial.print(DisplayWords);
  } else if (sensorValue < thresholdDown){
    String DisplayWords = "Wet, Leave it!";
    if (relePin == HIGH){
      needWater = 0;
      StopPump();
    } else{
      Serial.print(DisplayWords); 
    }
  } else {
    Serial.print(sensorValue);
  }
  delay(1000);
}
void StartPump(){ //Funksjon for å starte pumpa
  digitalWrite(relePin, HIGH);
}
void StopPump(){ //Funksjon for å stoppe punpa
  digitalWrite(relePin, LOW);
}
