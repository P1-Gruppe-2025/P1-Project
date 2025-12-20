#include <Zumo32U4.h>
#include <Wire.h>
#include "GyroTurn.h"

extern Zumo32U4OLED oled;
extern Zumo32U4Motors motors;
Zumo32U4IMU imu;

uint32_t turnAngle = 0;
int16_t turnRate;
int16_t gyroOffset;
uint16_t gyroLastUpdate = 0;

void setupGyro() {
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();

  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++) {
    while(!imu.gyroDataReady()) {}
    imu.readGyro();
    total += imu.g.z;
  }
  gyroOffset = total / 1024;
  turnSensorReset();
}

void turnByAngle(int target) {
  turnSensorReset();

  int direction = target > 0 ? 1 : -1;
  motors.setSpeeds(200 * direction, -200 * direction);

  while (true) {
    int32_t angle = getTurnAngleInDegrees();
    oled.clear();
    oled.print(angle);
    if (abs(angle) >= abs(target) - 10)
      break;
    delay(5);
  }

  motors.setSpeeds(80 * direction, -80 * direction);

  while (true) {
    int32_t angle = getTurnAngleInDegrees();
    if (abs(angle) >= abs(target))
      break;
    delay(5);
  }

  motors.setSpeeds(0,0);
  delay(150);
}

void turnSensorReset() {
  gyroLastUpdate = micros();
  turnAngle = 0;
}

void turnSensorUpdate() {
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;

  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;

  int32_t d = (int32_t)turnRate * dt;
  turnAngle += (int64_t)d * 14680064 / 17578125;
}

int32_t getTurnAngleInDegrees() {
  turnSensorUpdate();
  return (((int32_t)turnAngle >> 16) * 360) >> 16;
}
