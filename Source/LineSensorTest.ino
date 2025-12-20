#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4Encoders encoders;
Zumo32U4OLED oled;
Zumo32U4Buzzer buzzer;

uint16_t lineSensorValue[3]; 

int threshold = 1500;
int thresholdmid = 600;

bool cleared = 0;
bool left = 1;
int center = 2;
int right = 3;


void setup(){
lineSensors.initThreeSensors();
Serial.begin(9600);
motors.setSpeeds(80,80);
}

void loop(){
oled.clear();
lineSensors.read(lineSensorValue);

if (lineSensorValue[0]>threshold){
  oled.print("Venstre");
  Serial.print("Venstre har ramt: ");
  Serial.print(left);
}

else if (lineSensorValue[1]>thresholdmid){
  oled.print("Center");
  Serial.print("Center har ramt: ");
  Serial.print(center);
}

else if (lineSensorValue[2]>threshold){
  oled.print("Højre");
  Serial.print("Højre har ramt: ");
  Serial.print(right);
}
  
else{
  oled.print("Clear");
  Serial.print("Alt er clear: ");
  Serial.print(cleared);
}
delay(100);
}
