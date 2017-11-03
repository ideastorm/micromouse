#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Arduino.h>


#ifndef STEPPER_SHIELD_H
#define STEPPER_SHIELD_H

#define MOTOR_PORT PORTD
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
#define LEFT_MOTOR_DIR 7
#define LEFT_MOTOR_STEP 6
#define RIGHT_MOTOR_DIR 5
#define RIGHT_MOTOR_STEP 4
#define MOTOR_ENABLE 2
#define MS1 3
#define MS2 8
#define MS3 13

#define LED1 10
#define LED2 11
#define LED3 12

#define IR0 A0
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4
#define IR5 A5

#define IR_READ_COUNT 10

// #define LEFT_SENSOR A0
// #define FRONT_SENSOR A1
// #define RIGHT_SENSOR A2
//
// #define LEFT_THRESHOLD 800
// #define RIGHT_THRESHOLD 800
// #define FRONT_THRESHOLD 800

//constants
#define ACCELERATION 10.0
#define MAX_SPEED 2000

#define STEPS_PER_SQ 3475
#define STEPS_PER_90 1815

//pin definitions
class AccelStepper;
class MultiStepper;


class StepperShield
{
public:

  bool left;
  bool right;
  bool front;
  bool back;

  short frontLeftWall;
  short backLeftWall;

  short frontRightWall;
  short backRightWall;

  short frontWall;
  short backWall;

  StepperShield();
  void moveForwardOne();
  void moveBackOne();
  void rotate90right();
  void rotate90left();
  void disableMotors();
  void enableMotors();
  void printStatus();
  void updateSensors();
  void printBoolStatus();

private:
  MultiStepper steppers;
  AccelStepper leftMotor;
  AccelStepper rightMotor;
  double positions[2];
  long closestPositions[2];
  void updateClosestPositions();
  void readSensors(short,short,short,short&,short&);
  short getAverageRead(short times, short pin);



};


#endif
