#include <Zumo32U4.h>

Zumo32U4OLED oled;

const int BAUD_RATE = 115200;
const int TRIGONOMETRY_PIN = 9;
const int ECHO_PIN = 13;
const int INITIAL_SHOOTOUT_TIME_IN_MICROSECONDS = 2;
const int TRIGONOMETRY_CAPTURE_TIME_IN_MICROSECONDS = 10;
const float SPEED_OF_SOUND_IN_CENTIMETERS_PER_MICROSECONDS = 0.0343;

long DurationInMicroseconds;
int DistanceInCentimeters;

void setup() {
  pinMode(TRIGONOMETRY_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(BAUD_RATE);
  oled.clear();
}

void loop() {
  digitalWrite(TRIGONOMETRY_PIN, LOW);
  delayMicroseconds(INITIAL_SHOOTOUT_TIME_IN_MICROSECONDS);

  digitalWrite(TRIGONOMETRY_PIN, HIGH);
  delayMicroseconds(TRIGONOMETRY_CAPTURE_TIME_IN_MICROSECONDS);
  digitalWrite(TRIGONOMETRY_PIN, LOW);

  DurationInMicroseconds = pulseIn(ECHO_PIN, HIGH);
  DistanceInCentimeters = DurationInMicroseconds * SPEED_OF_SOUND_IN_CENTIMETERS_PER_MICROSECONDS * 0.5;
  Serial.print("Distance: ");
  Serial.println(DistanceInCentimeters);
  oled.gotoXY(0, 0);
  oled.print(DistanceInCentimeters);
}
