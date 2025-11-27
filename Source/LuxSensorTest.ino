#include <Zumo32U4.h>
#include <Wire.h>
#include <Adafruit_VEML7700.h>

Zumo32U4Motors motors;
Adafruit_VEML7700 veml = Adafruit_VEML7700();

const int NUM_SCAN_STEPS = 12;     // 12 steps -> 30° per step
const int ROTATE_DELAY = 250;      // kan tunes til ca. 30 grader
const int DRIVE_DELAY = 800;       // frem delay
const int FORWARD_SPEED = 200;
const int TURN_SPEED = 150;
int CORRECTION_SIZE = 2;           // adjustable drejning correction

float luxMax = 0;                  // den højeste lux målt

void coarseScan(float luxReadings[]) {
  for (int i = 0; i < NUM_SCAN_STEPS; i++) {
    luxReadings[i] = veml.readLux();

    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(ROTATE_DELAY);
    motors.setSpeeds(0, 0);
    delay(50);
  }
}

int findBrightestStep(float luxReadings[]) {
  int brightestStep = 0;
  float localMax = 0;
  for (int i = 0; i < NUM_SCAN_STEPS; i++) {
    if (luxReadings[i] > localMax) {
      localMax = luxReadings[i];
      brightestStep = i;
    }
  }
  luxMax = localMax; // opdaterer global luxMax
  return brightestStep;
}

void turnToStep(int step) {
  int stepsCW = step;
  int stepsCCW = NUM_SCAN_STEPS - step;
  if (stepsCW <= stepsCCW) {
    for (int i = 0; i < stepsCW; i++) {
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(ROTATE_DELAY);
      motors.setSpeeds(0, 0);
      delay(50);
    }
  } else {
    for (int i = 0; i < stepsCCW; i++) {
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(ROTATE_DELAY);
      motors.setSpeeds(0, 0);
      delay(50);
    }
  }
}

void setup() {
  Serial.begin(9600);
  veml.begin();
  veml.setGain(VEML7700_GAIN_1_8);
  veml.setIntegrationTime(VEML7700_IT_100MS);
}

void loop() {
  float luxReadings[NUM_SCAN_STEPS];

  //laver scanning
  coarseScan(luxReadings);
  int brightestStep = findBrightestStep(luxReadings);

  // drejer til det lyseste step
  turnToStep(brightestStep);

  // kør mod lyset, men lav også corrections
  while (true) {
    float luxBefore = veml.readLux();


    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(DRIVE_DELAY);
    motors.setSpeeds(0, 0);
    delay(50);

    float luxAfter = veml.readLux();

    // tjekker om vi har mistet lyset
    if (luxAfter <= luxBefore) {

      //kører baglæns
      motors.setSpeeds(-FORWARD_SPEED, -FORWARD_SPEED);
      delay(DRIVE_DELAY);
      motors.setSpeeds(0, 0);
      delay(50);

      // scanner igen for at finde det lyseste sted
      coarseScan(luxReadings);
      brightestStep = findBrightestStep(luxReadings);
      Serial.print("New brightest step: "); Serial.println(brightestStep);

      turnToStep(brightestStep);
      continue; // bliv ved med at køre
    }

    // stopper hvis du er tæt nok på lyset
    if (luxAfter > 1000) {  // adjust threshold for your flashlight
      motors.setSpeeds(0, 0);
      Serial.println("Reached target!");
      while(1);
    }
  }
}
