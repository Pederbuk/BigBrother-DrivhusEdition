int thresholdUp = 550; //Tørt
int thresholdDown = 300; //Vått

//Veldig tørr jord ute = 557 og 540

int sensorPin = A0;

//Water pump
int relePin = 2;

void setup(){
  Serial.begin(9600);

  pinMode(relePin, OUTPUT); 
}

int needWater = 0;
int waitTime = 5;

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


//Funksjon for å starte pumpa
void StartPump(){
  digitalWrite(relePin, HIGH);
}

//Funksjon for å stoppe punpa
void StopPump(){
  digitalWrite(relePin, LOW);
}
