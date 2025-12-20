 #include <Zumo32U4.h>

Zumo32U4OLED oled;
Zumo32U4ProximitySensors prox;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;

//erklærer mængden af linesensors
uint16_t lineSensorValue[3]; 

//til linesensor
int threshold = 1500;
int thresholdmid = 600;

//til prox sensor
int left;
int centerLeft;
int centerRight;
int right;



//--------------------------------Funktioner---------------------------------


//prox
void readProx(){ //læs proximity sensor og gemmer værdierne i variablerne
  prox.read();
  left = prox.countsLeftWithLeftLeds();
  centerLeft = prox.countsFrontWithLeftLeds();
  centerRight = prox.countsFrontWithRightLeds();
  right = prox.countsRightWithRightLeds();
}

//prox
void checkForObstacleInFront(){
  Serial.println("Checking for centerLeft/Right");
  if (centerLeft>5 || centerRight>5){
    Serial.print("Der er noget foran mig");
    oled.gotoXY(0,0);
    oled.print("Obstacle");
    //Her skal vi med UL vente indtil man er 'x' cm fra target og derefter stoppe. Lige nu bruger vi bare delay.
    delay(200);
    stopZumo();
    stopProgram();
  }
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
    Serial.print("Venstre har ramt");
    oled.clear();
    oled.gotoXY(0,0);
    oled.print("Venstre");
    oled.display();
    delay(500);
    backAway();
  }
  //center
  else if (lineSensorValue[1]>thresholdmid){
    stopZumo();
    Serial.print("Center har ramt");
    oled.clear();
    oled.gotoXY(0,0);
    oled.print("Center");
    oled.display();
    delay(500);
    backAway();
  }
  //højre
  else if (lineSensorValue[2]>threshold){
    stopZumo();
    Serial.print("Højre har ramt");
    oled.clear();
    oled.gotoXY(0,0);
    oled.print("Højre");
    oled.display();
    delay(500);
    backAway();
  }
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


//program
void stopProgram(){
  while(1==1);
}



void setup(){
  prox.initThreeSensors();
  lineSensors.initThreeSensors();
  Serial.begin(115200);
  motors.setSpeeds(100,100);
}


void loop(){
  readProx();
  checkForObstacleInFront();
  delay(200);
  readLines();
  checkForLines();
  delay(200);
}
