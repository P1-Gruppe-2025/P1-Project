#include <Zumo32U4.h>

Zumo32U4OLED oled;

long duration;
int distance;

const int trigPin = 9;
const int echoPin = 13;

void setup() {
 
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

Serial.begin(115200);
oled.clear();
}


void loop() {

digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);

distance = duration * 0.034 / 2;
Serial.print("Distance: ");
Serial.println(distance);
oled.gotoXY(0,0);
oled.print(distance);

}
