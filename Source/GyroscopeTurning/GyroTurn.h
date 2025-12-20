#ifndef GYROTURN_H
#define GYROTURN_H

#include <Zumo32U4.h>

void setupGyro();
void turnByAngle(int target);
void turnSensorReset();
void turnSensorUpdate();
int32_t getTurnAngleInDegrees();

#endif
