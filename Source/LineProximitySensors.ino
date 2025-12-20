#include <Zumo32U4.h>

Zumo32U4OLED OLED;
Zumo32U4ProximitySensors ProximitySensors;
Zumo32U4Motors Motors;
Zumo32U4LineSensors LineSensors;

//erklærer mængden af linesensors
uint16_t LineSensorLuminances[3];

//til linesensor
const int BACK_TIME_IN_MILLISECONDS = 375;
const int MOVE_SPEED = 80;  // Adjusted from 100 to be consistent with the other code files.
const int BAUD_RATE = 115200;
const int BLACK_THRESHOLD = 1500;
const int GRAY_THRESHOLD = 600;
const int MINIMUM_DISTANCE_THRESHOLD_IN_CENTIMETERS = 5;

//til prox sensor
int LeftCounts;
int CenterLeftCounts;
int CenterRightCounts;
int RightCounts;

// Assigns the counts variables to the ones measured by the ProximitySensors.
void ReadProximityCounts() {
  ProximitySensors.read();
  LeftCounts = ProximitySensors.countsLeftWithLeftLeds();
  CenterLeftCounts = ProximitySensors.countsFrontWithLeftLeds();
  CenterRightCounts = ProximitySensors.countsFrontWithRightLeds();
  RightCounts = ProximitySensors.countsRightWithRightLeds();
}

// Checks for obstacles using the proximity sensors.
void CheckForObstacles() {
  Serial.println("Checking for Left and Right obstacles.");
  if (CenterLeftCounts <= MINIMUM_DISTANCE_THRESHOLD_IN_CENTIMETERS && CenterRightCounts <= MINIMUM_DISTANCE_THRESHOLD_IN_CENTIMETERS) return;

  Serial.print("Detected obstacle.");
  OLED.gotoXY(0, 0);
  OLED.print("Obstacle");
  delay(200); // Artifical delay.. AGAIN? WHY?
  Motors.setSpeeds(0, 0);
  PermanentlyStopProgram();
}

// I would refactor this in a heartbeat, but since this is just a standard ol' guideline checkup, this will stay.
void CheckForLines() {
  //venstre
  if (LineSensorLuminances[0] > BLACK_THRESHOLD) {
  Motors.setSpeeds(0, 0);
    Serial.print("Venstre har ramt");
    OLED.clear();
    OLED.gotoXY(0, 0);
    OLED.print("Venstre");
    OLED.display();
    delay(500);
    BackComfortableDistanceAwayFromObstacle();
  }
  //center
  else if (LineSensorLuminances[1] > GRAY_THRESHOLD) {
  Motors.setSpeeds(0, 0);
    Serial.print("Center har ramt");
    OLED.clear();
    OLED.gotoXY(0, 0);
    OLED.print("Center");
    OLED.display();
    delay(500);
    BackComfortableDistanceAwayFromObstacle();
  }
  //højre
  else if (LineSensorLuminances[2] > BLACK_THRESHOLD) {
  Motors.setSpeeds(0, 0);
    Serial.print("Højre har ramt");
    OLED.clear();
    OLED.gotoXY(0, 0);
    OLED.print("Højre");
    OLED.display();
    delay(500);
    BackComfortableDistanceAwayFromObstacle();
  }
}

// Moves backwards for BACK_TIME_IN_MILLISECONDS amount of time, then stops and waits 500ms (why?)
void BackComfortableDistanceAwayFromObstacle() {
  Motors.setSpeeds(-MOVE_SPEED, -MOVE_SPEED);
  delay(BACK_TIME_IN_MILLISECONDS);
  Motors.setSpeeds(0, 0);  // Also deemed the StopZumo function useless, as it's a wrapper for another function, making it useless.
  delay(500);              // Artifical delay? I need an explanation.
}

// This is a really strange way of stopping the program, but if it works it works.
void PermanentlyStopProgram() {
  while (true)
    ;
}

// Sets up the Proximity- and Linesensors, as well as the serial monitors and motors.
void setup() {
  ProximitySensors.initThreeSensors();
  LineSensors.initThreeSensors();
  Serial.begin(BAUD_RATE);
  Motors.setSpeeds(MOVE_SPEED, MOVE_SPEED);
}

// Gets called every 200ms(Why?)
void loop() {
  ReadProximityCounts();
  CheckForObstacles();
  LineSensors.read(LineSensorLuminances);  // Deemed the readLines function useless, as it's a 'facade' for just calling another function, making it useless.
  CheckForLines();
  delay(200);  // Also artifical? why are these delays here without explanation?
}
