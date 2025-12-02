/*
  To use in a .ino file, you will need to have this line of code at the top;
  #include "Gyro_Angles.h"

  This way, it introduces the functions and variables directly into your .ino file.
  You can make a new file by pressing the + new tab in the IDE editor, I am unaware of how it is in the earlier editors.

  Example:
  Zumo32U4IMU imu;
  Zumo32U4Motors motors;

  void setup()
  {
    Serial.begin(9600);
    SetupGyroscope(imu);
    ResetGyroscope();
  }

  void loop()
  {
    UpdateGyroscope(imu);
  
    int32_t Degrees = GetTurnAngleInDegrees();
    //Serial.println(Degrees);
    if (Degrees < 90) {
      motors.setSpeeds(-400, 400);
    } else {
      motors.setSpeeds(0, 0);
    }
  }
*/

#pragma once

#include <Zumo32U4.h>
#include <Wire.h>

const int32_t TURN_ANGLE_45 = 0x20000000; // 45 degrees (536870912)
const int32_t TURN_ANGLE_90 = TURN_ANGLE_45 * 2; // 90 Degrees (1073741824)
const int32_t TURN_ANGLE_1 = (TURN_ANGLE_45 + 22) / 45; // 1 Degree (11930465)
const int CALIBRATION_STEPS = 1024; // How many steps the gyroscope should take for calibration.

uint32_t TurnAngle = 0;
int16_t TurnRate;
int16_t CalibrationOffset;
uint16_t LastUpdate = 0;

// Resets the Gyroscope to its basic state (no turn angle and resets the LastUpdate timer.)
void ResetGyroscope() {
  LastUpdate = micros();
  TurnAngle = 0;
}

// Updates the IMU gyro by calculating its current angle.
void UpdateGyroscope(Zumo32U4IMU& IMU) {
  IMU.readGyro();
  TurnRate = IMU.g.z - CalibrationOffset;
  
  uint16_t CurrentTime = micros();
  uint16_t DeltaTime = CurrentTime - LastUpdate;
  LastUpdate = CurrentTime;
  
  int32_t TurnSinceLastUpdate = (int32_t)TurnRate * DeltaTime;
  TurnAngle += (int64_t)TurnSinceLastUpdate * 14680064 / 17578125;
}

// Converts TurnAngle into Degrees.
int32_t GetTurnAngleInDegrees() {
  return (((int32_t)TurnAngle >> 16) * 360) >> 16;
}

// Initializes the IMU, requires it to be inserted as an argument.
void SetupGyroscope(Zumo32U4IMU& IMU) {
  Wire.begin();
  IMU.init();
  IMU.enableDefault();
  IMU.configureForTurnSensing();

  int32_t Total = 0;
  for (uint16_t i = 0; i < CALIBRATION_STEPS; i++) {
    while (!IMU.gyroDataReady()) {}
    IMU.readGyro();

    Total += IMU.g.z;
  }
  
  CalibrationOffset = Total / CALIBRATION_STEPS;
}
