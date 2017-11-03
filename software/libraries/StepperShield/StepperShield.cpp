#include "StepperShield.h"

StepperShield::StepperShield(){
  leftMotor = AccelStepper(AccelStepper::DRIVER, LEFT_MOTOR_STEP,LEFT_MOTOR_DIR);
  rightMotor = AccelStepper(AccelStepper::DRIVER, RIGHT_MOTOR_STEP,RIGHT_MOTOR_DIR);


  pinMode(MS1, OUTPUT);
	pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);


	digitalWrite(MS1, HIGH);
	digitalWrite(MS2, HIGH);
	digitalWrite(MS3, HIGH);

  pinMode(MOTOR_ENABLE, OUTPUT);
  digitalWrite(MOTOR_ENABLE,LOW);


  rightMotor.setAcceleration(ACCELERATION);
  leftMotor.setAcceleration(ACCELERATION);
  leftMotor.setMaxSpeed(MAX_SPEED);
  rightMotor.setMaxSpeed(MAX_SPEED);
  // Then give them to MultiStepper to manage
  steppers.addStepper(leftMotor);
  steppers.addStepper(rightMotor);

  pinMode(IR0,INPUT);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  pinMode(IR4,INPUT);
  pinMode(IR5,INPUT);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
}

void StepperShield::disableMotors(){
	digitalWrite(MOTOR_ENABLE,HIGH);
}

void StepperShield::enableMotors(){
	digitalWrite(MOTOR_ENABLE,LOW);
}

void StepperShield::moveForwardOne(){
  enableMotors();
  positions[0] += STEPS_PER_SQ;
  positions[1] += STEPS_PER_SQ;
  updateClosestPositions();
	steppers.moveTo(closestPositions);
  steppers.runSpeedToPosition();
  disableMotors();
}

void StepperShield::moveBackOne(){
  enableMotors();
  positions[0] -= STEPS_PER_SQ;
  positions[1] -= STEPS_PER_SQ;
  updateClosestPositions();
	steppers.moveTo(closestPositions);
  steppers.runSpeedToPosition();
  disableMotors();
}

void StepperShield::rotate90right(){
  enableMotors();
  positions[0] += STEPS_PER_90;
  positions[1] -= STEPS_PER_90;
  updateClosestPositions();
	steppers.moveTo(closestPositions);
  steppers.runSpeedToPosition();
  disableMotors();
}

void StepperShield::rotate90left(){
  enableMotors();
  positions[0] -= STEPS_PER_90;
  positions[1] += STEPS_PER_90;
  updateClosestPositions();
	steppers.moveTo(closestPositions);
  steppers.runSpeedToPosition();
  disableMotors();
}

void StepperShield::updateClosestPositions(){
	closestPositions[0] = positions[0];
	closestPositions[1] = positions[1];
}

void StepperShield::updateSensors(){
  readSensors(IR0,IR1,LED1,frontWall,backWall);
  readSensors(IR2,IR3,LED2,frontLeftWall,frontRightWall);
  readSensors(IR4,IR5,LED3,backLeftWall,backRightWall);

  left = true;
  right = true;
  front = true;
  back = true;

  if ((frontLeftWall + backLeftWall )/2 < 20){
    left = false;
  }
  if((frontRightWall + backRightWall)/2<40){
    right = false;
  }
  if(backWall < 10){
    back = false;
  }
  if(frontWall < 10 ){
    front = false;
  }
}

void StepperShield::readSensors(short pinA,short pinB,short ledPin,short& aResult,short& bResult){
  int sumA = 0;
  int sumB = 0;
  short ambientA;
  short ambientB;

  for (size_t i = 0; i < IR_READ_COUNT ; i++) {
    ambientA = analogRead(pinA);
    ambientB = analogRead(pinB);
    digitalWrite(ledPin, HIGH);
    delay(1);
    sumA +=  ambientA-analogRead(pinA);
    sumB +=  ambientB - analogRead(pinB);
    digitalWrite(ledPin,LOW);
  }

  aResult = sumA/IR_READ_COUNT;
  bResult = sumB/IR_READ_COUNT;
}


// int readIR(int times){
//   for(int x=0;x<times;x++){
//     digitalWrite(IRemitter,LOW);           // turning the IR LEDs off to read the IR coming from the ambient
//     delay(1);                                             // minimum delay necessary to read values
//     ambientIR = analogRead(IRpin);  // storing IR coming from the ambient
//     digitalWrite(IRemitter,HIGH);          // turning the IR LEDs on to read the IR coming from the obstacle
//     delay(1);                                             // minimum delay necessary to read values
//     obstacleIR = analogRead(IRpin);  // storing IR coming from the obstacle
//     value[x] = ambientIR-obstacleIR;   // calculating changes in IR values and storing it for future average
//   }
//
//   for(int x=0;x<times;x++){        // calculating the average based on the "accuracy"
//     distance+=value[x];
//   }
//   return(distance/times);            // return the final value
// }

short StepperShield::getAverageRead(short times,short pin){
  int sum = 0;
  for (short i = 0; i < times; i++) {
    sum += analogRead(pin);
    delay(1);
  }
  return sum/times;
}

void StepperShield::printBoolStatus(){
  Serial.print("X: ");
  Serial.print(positions[0]/STEPS_PER_SQ);
  Serial.print(" Y: ");
  Serial.print(positions[1]/STEPS_PER_SQ);
  Serial.print(" LeftWall :");
  Serial.print(left);
  Serial.print(" RightWall : ");
  Serial.print(right);
  Serial.print(" Front : ");
  Serial.print(front);
  Serial.print(" Back : ");
  Serial.println(back);
}



void StepperShield::printStatus(){
  Serial.print("X: ");
  Serial.print(positions[0]/STEPS_PER_SQ);
  Serial.print(" Y: ");
  Serial.print(positions[1]/STEPS_PER_SQ);
  Serial.print(" LeftWall F:");
  Serial.print(frontLeftWall);
  Serial.print(" B:");
  Serial.print(backLeftWall);
  Serial.print(" RightWall F: ");
  Serial.print(frontRightWall);
  Serial.print(" B:");
  Serial.print(backRightWall);
  Serial.print(" Front : ");
  Serial.print(frontWall);
  Serial.print(" Back : ");
  Serial.println(backWall);
}
