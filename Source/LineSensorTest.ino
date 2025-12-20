#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors Motors;
Zumo32U4LineSensors LineSensors;
Zumo32U4OLED OLED;

uint16_t LineSensorLuminances[3]; // Normally, you'd do a constant for the amount of line sensors, but since the Zumo32U4 only ever has 3 of them in the front, we don't really need to make a new variable just for that one use.

const int DELAY_IN_MILLISECONDS = 100; // If you're declaring a form of measurement, like meters, seconds etc, you should always do; "XXXX_IN_MEASUREMENT" for clarity, as type annotation won't work to explain that.
const int MOVE_SPEED = 80;
const int BAUD_RATE = 9600; // Normally, I would use 115200, as its the superior option to 9600 with literally no benefit.
const int BLACK_THRESHOLD = 1500;
const int GRAY_THRESHOLD = 600;
// I wanted to switch over to Enums, but this is just a standard refactor to adhere to coding guidelines.
const int CLEARED = 0;
const int LEFT = 1;
const int CENTER = 2;
const int RIGHT = 3;

void setup() {
  LineSensors.initThreeSensors();
  Serial.begin(BAUD_RATE);
  Motors.setSpeeds(MOVE_SPEED, MOVE_SPEED);
}

void loop() {
  OLED.clear();
  LineSensors.read(LineSensorLuminances);

  // Using Enums, we could've changed the 0 to a LEFT, making it more readable, but as said; I won't as this is a standard refactor.
  if (LineSensorLuminances[0] > BLACK_THRESHOLD) {
    OLED.print("Left");
    Serial.print("Left has hit: ");
    Serial.print(LEFT);
  }

  else if (LineSensorLuminances[1] > GRAY_THRESHOLD) {
    OLED.print("Center");
    Serial.print("Center has hit: ");
    Serial.print(CENTER);
  }

  else if (LineSensorLuminances[2] > BLACK_THRESHOLD) {
    OLED.print("Right");
    Serial.print("Right has hit: ");
    Serial.print(RIGHT);
  }

  else {
    OLED.print("Clear");
    Serial.print("Is all sensors clear: ");
    Serial.print(CLEARED);
  }
  delay(DELAY_IN_MILLISECONDS);
}
