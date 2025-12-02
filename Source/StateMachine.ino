#include <Zumo32U4.h>

enum States {
  Initialization,
  Scanning,
  Navigator,
  Avoiding,
  Reached
}

Zumo32U4OLED OLED;
Zumo32U4Buzzer Buzzer;
Zumo32U4Motors Motors;

const int BAUD_RATE = 115200;

States State = Initialization;

void setup() {
  Serial.begin(BAUD_RATE);
  OLED.clear();
}

void loop() {
  OLED.print("       ");

  switch (State) {
    case Initialization:
      // 'Calibrates' the sensors, unsure on what that means.
      // Waits for a single user input on the Zumo32U4 (A, B or C button.)
      break;
    case Scanning:
      // Spins around in 360 degrees, by scanning for the highest lux in the surrounding area.
      break;
    case Navigator:
      Motors.setSpeeds(200, 200);
      // Check for obstacles or tape.
      // Check for white tape, if it is found then it switches to Reached.
      break;
    case Avoiding:
      // Avoids obstacles or tape, once avoided it switches back to Avoiding.
      break;
    case Reached:
      Motors.setSpeeds(0, 0);
      break;
  }
}
