/**
  SmoothServo.h
  Smooth movements extension for Arduino Servo library

  @autor Angelo Dalzotto (angelodalzotto97@gmail.com)
  @version 1.1 17/09/21

  THE BEER-WARE LICENSE: As long as you retain this notice you can do whatever
  you want with this stuff. If we meet some day, and you think this stuff is
  worth it, you can buy me a beer in return. This software has no warranty.
*/

/**
  A servo is activated, attached, and set to initial position by creating an
  instance of SmoothServo class passing the pin.
  The servos are pulsed using the standard Servo library.
  The position of a servo is updated based on the time passed since the new
  desired "posRef" was set using the method setPosRef(posRef).
  Writing the actual position based on time to the servo is possible using
  the method updatePos().
  It is possible to set all new references to multiple servos and then update
  all of them simultaneously using SmoothServo::updateAllServos.

  The extended methods are:

    SmoothServo(pin, [posInit=90], [timeRefMax=2000], [timeRefMin=500])
    class constructor to initialize the servo on the desired pin, initial
    position, time in milliseconds to move from 0º-180º and time in milliseconds
    to move from 0º-5º

    setPosRef(posRef) - sets the desired final angle

    updatePos() - sets the angle of the servo based on timeRefMin

    isUpdated() - returns if the servo is in the final desired angle

    updateAllServos() - sets the angle for all declared servos

    areAllUpdated() - checks if all servos are updated
*/
#ifndef _SMOOTH_SERVO_H_
#define _SMOOTH_SERVO_H_

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Servo.h>

class SmoothServo: protected Servo{
public:
  SmoothServo(int pin, uint8_t posInit=90,
              unsigned long long int timeRefMax=2000,
              unsigned long long int timeRefMin=500);
  void setPosRef(uint8_t posRef);
  void updatePos();
  void write(int angle);
  int  read();
  bool isUpdated();
  bool attached();

  static void updateAllServos();
  static bool areAllUpdated();

private:
  unsigned long long int timeRefMin, timeRefMax; //User input times
  unsigned long long int timeInit, timeRef;      //Timing control
  uint8_t posOut, posRef, posNow;                //Position control
  double coef3; //Coefficients for
  double coef2; //calculations
  bool updated; //If the chosen position has been achieved

  static SmoothServo* smoothServoList[MAX_SERVOS]; //Static list for controlling
  static uint8_t smoothServoCount;                 //all the servos

  void updateOut();         //Just calculate the next write value
  uint8_t calculateOut();   //Function with all calculus implemented
};


#endif /* _SMOOTH_SERVO_H_ */