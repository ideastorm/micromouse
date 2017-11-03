#include <AccelStepper.h>
#include <MultiStepper.h>
#include "StepperShield.h"
#include "MazeMap.h"

StepperShield motors;
MazeMap maze;
byte currentDirection = SOUTH; //Bot should be placed such that the back and right sides of the bot face the outside walls
byte currentX = 0;
byte currentY = 0; //start in "northwest" corner
byte currentTarget = CENTER;
boolean explore = true;
boolean updatedOnThisRun = false;

void setup(){
  Serial.begin(9600);
  Serial.println("**************************");
  motors.printBoolStatus();
}

void setDirection(byte direction){
  //Directions: N1,E2,S3,W4
  motors.printBoolStatus();
  if (direction != currentDirection) {
    if ((4 + direction - currentDirection)%4 == 1) { 
      motors.rotate90right();
    } else if ((4 + direction - currentDirection)%4 == 3) {
      motors.rotate90left();
    } else if ((4 + direction - currentDirection)%4 == 2) {
      motors.rotate90right();
      motors.rotate90right();
    }
    currentDirection = direction;  
  }
}

byte frontDirection() {
  return currentDirection;
}

byte rightDirection() {
  return (currentDirection + 1) % 4;
}

byte rearDirection() {
  return (currentDirection + 2) % 4;
}

byte leftDirection() {
  return (currentDirection + 3) % 4;
}

boolean updateMaze(){
  motors.updateSensors();
  motors.printBoolStatus();
  boolean updated = false;
  if(motors.front){
    Serial.println("closing front");
    updated |= maze.closeWall(currentX,currentY,frontDirection());    
  }
  if(motors.left){
    Serial.println("closing left");
    updated |= maze.closeWall(currentX,currentY,leftDirection());
  }
  if(motors.right){
    Serial.println("closing right");
    updated |= maze.closeWall(currentX,currentY,rightDirection());
  }
  if(motors.back){
    Serial.println("closing rear");
    updated |= maze.closeWall(currentX,currentY,rearDirection());
  }
  if (updated) {
    maze.solve(currentTarget);
  }
  return updated;
};

void moveRobot(byte direction) {
  setDirection(direction);
  motors.moveForwardOne();
  switch (direction) {
    case NORTH:
      currentY--;
      break;
    case SOUTH:
      currentY++;
      break;
    case EAST:
      currentX++;
      break;
    case WEST:
      currentX--;
      break;
  }
}

void loop(){
  if (explore) {
    updatedOnThisRun |= updateMaze(); //solves the maze for the current target
    moveRobot(maze.bestDirection(currentX, currentY));
    if (maze.isTarget(currentX, currentY, currentTarget)) {
      explore = updatedOnThisRun; //if we haven't gotten an update, start the speed runs
      updatedOnThisRun = false;
      currentTarget = (currentTarget + 1) % 2;
    }
  } else {
    //"speed" runs - if you wanted to get fancy you could take the path and try to do diagonal moves
    byte* path = maze.bestPath(currentX, currentY, currentTarget); //also solves the maze for the current target
    while (*path != 255) {
      moveRobot(*path);
      path++;
    }
    currentTarget = (currentTarget + 1) % 2;
  }
}
