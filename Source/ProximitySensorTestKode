#include <Zumo32U4.h>

Zumo32U4OLED oled;
Zumo32U4ProximitySensors prox;
Zumo32U4Motors motors;


void setup(){
  prox.initThreeSensors();
  oled.clear();
  motors.setSpeeds(100,100);
}


void loop(){

  prox.read();
  int left = prox.countsLeftWithLeftLeds();
  int centerLeft = prox.countsFrontWithLeftLeds();
  int centerRight = prox.countsFrontWithRightLeds();
  int right = prox.countsRightWithRightLeds();

  oled.gotoXY(0,0);

  oled.print(left); //venstre sensor
  oled.print(" ");

  oled.print(centerLeft); //venstre sensor i midten
  oled.print(" ");

  oled.print(centerRight); //højre sensor i midten
  oled.print(" ");

  oled.print(right); //højre sensor 
  oled.print(" ");

  if (centerLeft>5 || centerRight>5){
    motors.setSpeeds(0,0);
  }
}
