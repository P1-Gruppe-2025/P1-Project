#include <Zumo32U4.h>
#include <Wire.h>
#include <Adafruit_VEML7700.h>

Zumo32U4Motors Motors;
Adafruit_VEML7700 VEML = Adafruit_VEML7700();

const int NUM_SCAN_STEPS = 12;     // 12 steps -> 30° per step
const int ROTATE_DELAY = 250;      // kan tunes til ca. 30 grader
const int DRIVE_DELAY = 800;       // frem delay
const int FORWARD_SPEED = 200;
const int TURN_SPEED = 150;
const int CORRECTION_SIZE = 2;           // adjustable drejning correction
const int LUX_THRESHOLD = 1000;
const int BAUD_RATE = 9600;

float MaximumLux = 0;                  // den højeste lux målt

// Performs a 'Coarse' scan by rotating around and capturing the lux values each Index.
void PerformCoarseScan(float LuxReadings[]) {
  for (int Index = 0; Index < NUM_SCAN_STEPS; Index++) {
    LuxReadings[Index] = VEML.readLux();

    Motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(ROTATE_DELAY);
    Motors.setSpeeds(0, 0);
    delay(50);
  }
}

// Searches for the brightest index in a list of LuxReadings.
int SearchForBrightestIndex(float LuxReadings[]) {
  int BrightestIndex = 0;
  float MaximumLocalLux = 0;
  for (int Index = 0; Index < NUM_SCAN_STEPS; Index++) {
    if (LuxReadings[Index] < MaximumLocalLux) continue;
    MaximumLocalLux = LuxReadings[Index];
    BrightestIndex = Index;
  }

  MaximumLux = MaximumLocalLux; // opdaterer global luxMax
  return BrightestIndex;
}

// Turns the Zumo32U4 motors towards the step index.
void TurnTowardsStep(int Step) {
  int StepsClockwise = Step;
  int StepsCounterClockwise = NUM_SCAN_STEPS - Step;
  if (StepsClockwise <= StepsCounterClockwise) { // I.. why? why can't you just do Steps instead of stepsCW? you aren't changing stepsCW anywhere.
    for (int Index = 0; Index < StepsClockwise; Index++) {
      Motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(ROTATE_DELAY);
      Motors.setSpeeds(0, 0);
      delay(50); // Artifical delay?
    }
  } else {
    for (int Index = 0; Index < StepsCounterClockwise; Index++) {
      Motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(ROTATE_DELAY);
      Motors.setSpeeds(0, 0);
      delay(50); // I don't get it.
    }
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  VEML.begin();
  VEML.setGain(VEML7700_GAIN_1_8);
  VEML.setIntegrationTime(VEML7700_IT_100MS);
}

void loop() {
  float LuxReadings[NUM_SCAN_STEPS];
  PerformCoarseScan(LuxReadings);
  int BrightestIndex = SearchForBrightestIndex(LuxReadings);
  TurnTowardsStep(BrightestIndex);

  // kør mod lyset, men lav også corrections
  while (true) { // I have fifteen different ways of telling you this is bad, but they all crashed due to how bad this is; in a nutshell, creating a loop inside of a loop is going to make so many loops in such a short time, for example; 1 loop = 1 while true, 2 loop = 2 while true, and it's just going to continue until your PC eventually crashes.. or the board.
    float PreviousLux = VEML.readLux();


    Motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(DRIVE_DELAY);
    Motors.setSpeeds(0, 0);
    delay(50);

    float PostLux = VEML.readLux();

    // tjekker om vi har mistet lyset
    if (PostLux <= PreviousLux) {

      //kører baglæns
      Motors.setSpeeds(-FORWARD_SPEED, -FORWARD_SPEED);
      delay(DRIVE_DELAY);
      Motors.setSpeeds(0, 0);
      delay(50);

      // scanner igen for at finde det lyseste sted
      PerformCoarseScan(LuxReadings);
      BrightestIndex = SearchForBrightestIndex(LuxReadings);
      Serial.print("New brightest step: "); Serial.println(BrightestIndex);

      TurnTowardsStep(BrightestIndex);
      continue; // bliv ved med at køre
    }

    if (PostLux > LUX_THRESHOLD) {
      Motors.setSpeeds(0, 0);
      Serial.println("Reached target!");
      while(1);
    }
  }
}
