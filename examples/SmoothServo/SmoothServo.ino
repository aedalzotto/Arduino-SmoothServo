#include <SmoothServo.h>

SmoothServo servo1(9);
SmoothServo servo2(11);

void setup() {
  Serial.begin(38400);
  delay(500);
}

void loop() {
  servo1.setPosRef(180);
  servo2.setPosRef(180);
  while(!SmoothServo::areAllUpdated()){
    SmoothServo::updateAllServos();
    Serial.println(servo1.read());
    Serial.println(servo2.read());
  }

  while(1);

}
