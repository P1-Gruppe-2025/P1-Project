#include <Zumo32U4.h>

Zumo32U4OLED OLED;
Zumo32U4ProximitySensors ProximitySensors;
Zumo32U4Motors Motors;

const int MOVE_SPEED = 80;
const int MINIMUM_DISTANCE_THRESHOLD_IN_CENTIMETERS = 5;

void setup(){
  ProximitySensors.initThreeSensors();
  OLED.clear();
  Motors.setSpeeds(MOVE_SPEED,MOVE_SPEED);
}

void loop(){
  ProximitySensors.read();
  int LeftCounts = ProximitySensors.countsLeftWithLeftLeds();
  int CenterLeftCounts = ProximitySensors.countsFrontWithLeftLeds();
  int CenterRightCounts = ProximitySensors.countsFrontWithRightLeds();
  int RightCounts = ProximitySensors.countsRightWithRightLeds();

  OLED.gotoXY(0,0);

  OLED.print(LeftCounts); //venstre sensor
  OLED.print(" ");

  OLED.print(CenterLeftCounts); //venstre sensor i midten
  OLED.print(" ");

  OLED.print(CenterRightCounts); //højre sensor i midten
  OLED.print(" ");

  OLED.print(RightCounts); //højre sensor 
  OLED.print(" ");

  if (CenterLeftCounts < MINIMUM_DISTANCE_THRESHOLD_IN_CENTIMETERS && CenterRightCounts < MINIMUM_DISTANCE_THRESHOLD_IN_CENTIMETERS) continue; // Become a never nester, try not to indent, it ruins code flow.
  Motors.setSpeeds(0,0);
}
