#include <Zumo32U4.h>
#include <Adafruit_VEML7700.h>
#include "GyroTurn.h" //til gyroskop så jeg kan kalde på funktioner

Adafruit_VEML7700 veml = Adafruit_VEML7700();
Zumo32U4OLED oled;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors prox;
Zumo32U4LineSensors lineSensors;
Zumo32U4Buzzer buzzer;

//Line
int threshold = 1500;
int thresholdmid = 600;
int thresholdWhite = 300;
uint16_t lineSensorValue[3]; 

//Prox
int left;
int centerLeft;
int centerRight;
int right;

//Ultrasonic
long duration;
int distance;
const int trigPin = 13;
const int echoPin = 8;

//lux
const int totalSteps = 12;
int bestIndex = 0;
int bestLux = 0;
int targetLux = 1000;

//loop
enum States {
  scanForLight,
  driveToLight,
  avoiding,
};
States State = scanForLight;


//---------------------------------------------Funktioner--------------------------------------------------

//Ultrasonic
void readUS(){
    oled.clear();
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    distance = duration * 0.034 / 2;
    oled.gotoXY(0,0);
    oled.print(distance);
    delay(50);
}

void checkStartObstacle(){
  for (int i = 0; i < totalSteps; i++) {
    readUS();
    if (distance<12){
      oled.clear();
      oled.gotoXY(0,1);
      oled.print("Move");
      oled.gotoXY(1,1);
      oled.print("away");
      setup();
    }
    turnByAngle(30);
    delay(50);
  }
}


//prox
void readProx(){ //læs proximity sensor og gemmer værdierne i variablerne
  prox.read();
  left = prox.countsLeftWithLeftLeds();
  centerLeft = prox.countsFrontWithLeftLeds();
  centerRight = prox.countsFrontWithRightLeds();
  right = prox.countsRightWithRightLeds();
}




//movement
void stopZumo(){
  motors.setSpeeds(0,0);
}

//movement
void backAway(){
  motors.setSpeeds(-150,-150);
  delay(200);
  stopZumo();
  delay(500);
}

//movement
void startZumo(){
  motors.setSpeeds(100,100);
}



//program
void stopProgram(){
  motors.setSpeeds(0,0);
  oled.clear();
  oled.print("PrgmStop");
  while (1);
}

//line
void readLines(){
  lineSensors.read(lineSensorValue);
}

//line
void checkForLines(){
  //venstre
  if (lineSensorValue[0]>threshold){
    stopZumo();
    delay(500);
    backAway();
    avoidTapeL();
  }
  //center
  else if (lineSensorValue[1]>thresholdmid){
    stopZumo();
    delay(500);
    backAway();
    avoidTapeL();
  }
  //højre
  else if (lineSensorValue[2]>threshold){
    stopZumo();
    delay(500);
    backAway();
    avoidTapeR();
  }
}

//tager 4 samples per step og tager gennemsnittet, hjælper med præcis data
int readLightAverage(int samples = 4) { 
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum = sum + veml.readWhite();
    delay(10); 
  }
  return (int)(sum / samples);
}


//tager 12 steps og peger til højeste step
void checkOgTurn() {
  bestLux = 0;
  bestIndex = 0;

  for (int i = 0; i < totalSteps; i++) {
    int lux = readLightAverage(); //gennemsnitlig værdi
    if (lux > bestLux) {
      bestLux = lux; //sætter den nye bedste værdi
      bestIndex = i; //gemmer steppet
    }
    turnByAngle(30);
    delay(50);
  }
  if (bestLux<=100){     //tal skal kalibreres
    turnByAngle(-85);
    startZumo();
    delay(2000);
    stopZumo();
    turnByAngle(90);
    checkOgTurn();
  }
//drejer til højeste lux step + tager den korteste vej
  else{
  float angleToBest = bestIndex * 30;
  if (angleToBest > 180.0){ angleToBest =angleToBest - 360.0;}
  turnByAngle(angleToBest);
  delay(50);
  }
}




void targetFound(){
  stopZumo();
  oled.clear();
  oled.gotoXY(0,1);
  oled.print("Target");
  oled.gotoXY(1,1);
  oled.print("Found");
  stopProgram();
}


//movement
void turnRightAround(){ //der skal måske tilføjes noget med hvis endnu en obstacle står i vejen
    turnByAngle(90); //drej til højre
    startZumo();
    readProx();
    while (left>2){
    readProx();}

    stopZumo();

    turnByAngle(-85);//venstre
    startZumo();
    readProx();
    delay(1000); //vent på sensoren detecter obstacle
    while (left>2){
    readProx();}
    delay(1500); //være sikker på den kommer forbi obstacle

    stopZumo();

    turnByAngle(-85);//venstre
    startZumo();
    delay(300); //vent på sensoren detecter obstacle
    while (left<5){
    readProx();}
    delay(500); //juster ift obstacle, men skal egentlig sørge for den kommer hen i "midten" af obstacle i slutningen. Man kan også lade være med at have delayet.
    
    stopZumo();
    
    turnByAngle(90); //drej til højre
    delay(1000); 
    State = driveToLight;
    return;
}

//movement
void avoidTapeL(){
  turnByAngle(90);
  startZumo();
  delay(500);
  stopZumo();
  turnByAngle(-85);
  startZumo();
  delay(1000);
  stopZumo();
  recalibrate();
}

//movement
void avoidTapeR(){
  turnByAngle(-85);
  startZumo();
  delay(500);
  stopZumo();
  turnByAngle(90);
  startZumo();
  delay(1000);
  stopZumo();
  recalibrate();
}


//kører mod lys i en bestemt tid, avoider obstacles hvis det er der er nogen
void driveModLys() {

  int driveTime = 1500;
  unsigned long start = millis(); //millis er den mængde tid i sek der er gået siden zumoen blev tændt <|
                                                                              //                        |
  startZumo();                                                                //                        |
                                                                              //                        |
  while (millis() - start < driveTime) { //mængde tid mellem disse to linjer:                         <|
    //line
    readLines();
    checkForLines();
    
  if (lineSensorValue[0] < thresholdWhite || lineSensorValue[2] < thresholdWhite) {
    stopZumo();
    backAway();
    if (lineSensorValue[0] < thresholdWhite){ turnByAngle(60); }
    else if (lineSensorValue[2] < thresholdWhite){ turnByAngle(-60);}
    return;
}

    //prox +  us
    readProx();
    if (centerLeft > 4 || centerRight > 4) {
      readUS();
      if (distance > 0 && distance <= 7) {
        stopZumo();
        State = avoiding;
        return;
      }
   }
   
    delay(50);
  }

  stopZumo(); // stopper efter de 1,5 sek
  recalibrate();
}




//efter den har fundet retningen til et lys, men den har avoidet obstacle eller andet
void recalibrate(){

  bestLux = 0; 
  int lux = 0;
  int angleToBest = 0;           
  bestIndex = 0;

  //-2
  for (int i = 0; i < 3; i++) {
    turnByAngle(-20);
    delay(50);
  }


  for (int currentState = 0; currentState < 5; currentState++) {
    lux = readLightAverage();
    if (lux > bestLux) {
      bestLux = lux;
      bestIndex = currentState;
    }
    turnByAngle(20);
    delay(50);
  }
 
  angleToBest = -60 + (bestIndex * 20); //-60 fordi vi drejede 3 * 20 grader til venstre i starten, og vi gemmer bestIndex værdien som steppet hvor lyset var højst, uden ville der være
  // bias til venstre
  turnByAngle(angleToBest);
  delay(50);

  if (bestLux>targetLux){
    targetFound();
  }
  
  int tjekLys = readLightAverage();
  
  if (tjekLys <= bestLux - 500){
      stopZumo();
      recalibrate();
  }
}




void setup(){
  prox.initThreeSensors();
  lineSensors.initThreeSensors();
  setupGyro();
  veml.begin();
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //checkStartObstacle();
}



void loop() {

    switch (State) {
    case scanForLight:
      stopZumo();
      checkOgTurn();
      if (bestLux < 5) { //tjekker om der overhovedet er noget
        delay(100);
        State = scanForLight;
      } 
      else {
        State = driveToLight;
      }
      break;
      
    case driveToLight:
      driveModLys();
      break;
      
    case avoiding:
      turnRightAround();
      break;
      
  }
}
