/*
  SmoothServo.cpp
  Smooth movements extension for Arduino Servo library

  @autor Angelo Dalzotto (angelodalzotto97@gmail.com)
  @version 1.1 17/09/21

  THE BEER-WARE LICENSE: As long as you retain this notice you can do whatever
  you want with this stuff. If we meet some day, and you think this stuff is
  worth it, you can buy me a beer in return. This software has no warranty.
*/

#include "SmoothServo.h"

uint8_t SmoothServo::smoothServoCount = 0;
SmoothServo* SmoothServo::smoothServoList[MAX_SERVOS];


/**
  Updates all servos instanced in the class based on the time passed since
  the reference position was set.
*/
void SmoothServo::updateAllServos(){
  for(int i = 0; i < smoothServoCount; i++){
    smoothServoList[i]->updateOut();
  }
  for(int i = 0; i < smoothServoCount; i++){
    smoothServoList[i]->write(smoothServoList[i]->posOut);
  }
}

/**
  Returns if all servos instanced in the class are in the final desired position

  @return true if all servos are updated
*/
bool SmoothServo::areAllUpdated(){
  for(int i = 0; i < smoothServoCount; i++){
    if(!smoothServoList[i]->isUpdated())
      return false;
  }
  return true;
}

/**
  Instances a new servo.

  @param pin Wich   Arduino pin the servo is attached
  @param posInit    Initial position for the servo in degrees (default: 90)
  @param timeRefMax Time taken in ms to the servo move 180º (default: 2000)
  @param timeRefMin Time taken in ms to the servo move 5º (default: 500)
*/
SmoothServo::SmoothServo(int pin, uint8_t posInit,
                        unsigned long long int timeRefMax,
                        unsigned long long int timeRefMin)
{
  smoothServoList[smoothServoCount] = this;
  smoothServoCount++;
  attach(pin);

  write(posInit);

  this->timeRefMin = timeRefMin;
  this->timeRefMax = timeRefMax;
  posRef = posOut = posInit;
  updated = true;
}


/**
  Instantly writes position to servo. Substitutes the Servo library write()

  @param angle the desired position
*/
void SmoothServo::write(int angle){
  if(angle > 180 || angle < 0) return;

  posOut = angle;
  Servo::write(angle);
}

/**
  Sets the desired final position for the servo

  @param posRef the desired position
*/
void SmoothServo::setPosRef(uint8_t posRef){
  if(posRef < 0 || posRef > 180) return;

  float timeSecs, timeSquare;
  int deltaPos;
  posNow = posOut;
  deltaPos = posRef - posNow;

  timeRef = (timeRefMax-timeRefMin)*abs(deltaPos)/180 + timeRefMin;
  timeSecs = (float)timeRef/1000.0;
  timeSquare = timeSecs*timeSecs;
  coef2 = 3.0*(float)deltaPos/timeSquare;
  coef3 = -2.0*(float)deltaPos/(timeSquare*timeSecs);
  updated = false;
  timeInit = millis();
}

/**
  Updates the position of the servo based on time since the reference was set
*/
void SmoothServo::updatePos(){
  updateOut();
  write(posOut);
}

/**
  Just updates the posOut variable without writing to the servo
*/
void SmoothServo::updateOut(){
  if(updated) return;

  if((millis() - timeInit) >= timeRef){
    posOut = posRef;
    updated = true;
  }else
    posOut = calculateOut();

}

/**
  Calculates the out position in degrees

  @return The degree calculated
*/
uint8_t SmoothServo::calculateOut(){
  float timePass = (float)(millis()-timeInit)/1000.0;
  float timeSquare = timePass*timePass;
  return posNow + coef2*timeSquare + coef3*timeSquare*timePass;
}

/**
  If the servo has already moved to the final position

  @return true if the servo is updated
*/
bool SmoothServo::isUpdated(){
  return updated;
}

int SmoothServo::read(){
  return Servo::read();
}

bool SmoothServo::attached(){
  return Servo::attached();
}
