#include <AccelStepper.h>
#include <MultiStepper.h>
#include <StackArray.h>
#include <MemoryFree.h>
#include "StepperShield.h"
#include "MazeMap.h"

StepperShield motors;
MazeMap maze;
byte curDirection = NORTH;

struct Position{
  byte direction;
  short x;
  short y;
};
void setup(){
  Serial.begin(9600);
  Serial.println("**************************");
  motors.printBoolStatus();

}


void randomSearch(){

  while(1){
    motors.updateSensors();
    motors.printBoolStatus();

    //ifBack only option turn around and go back one
    if(motors.front && motors.left && motors.right){
      motors.moveBackOne();
      motors.rotate90right();
      motors.rotate90right();
    }
    else{
      switch (random(3)) {
        case 0:
          if(!motors.front){
            motors.moveForwardOne();
          }
          break;
        case 1:
          if(!motors.left){
            motors.rotate90left();
            motors.moveForwardOne();
          }
          break;
        case 2:
          if(!motors.right){
            motors.rotate90right();
            motors.moveForwardOne();
          }
      }
    }
  }
}


int incrementHeading(byte dir){
  motors.printBoolStatus();

  switch(dir){
    case NORTH:
      return EAST;
    case EAST:
      return SOUTH;
    case SOUTH:
      return WEST;
    case WEST:
      return NORTH;
    }
    return 0;
}

void setDirection(byte direction){
  motors.printBoolStatus();

  //line up direction
  while(direction != curDirection){
    motors.rotate90right();
    curDirection = incrementHeading(curDirection);
  }
}

void moveRobot(byte direction){
  motors.printBoolStatus();

    setDirection(direction);
    motors.moveForwardOne();
}

void updatemaze(short x,short y){
  motors.printBoolStatus();

  setDirection(NORTH);
  motors.updateSensors();
  motors.printBoolStatus();
  if(motors.front){
    Serial.println("closing north");
    maze.closeWall(x,y,NORTH);
  }
  if(motors.left){
    Serial.println("closing WEST");
    maze.closeWall(x,y,WEST);
  }
  if(motors.right){
    Serial.println("closing EAST");

    maze.closeWall(x,y,EAST);
  }
  if(motors.back){
    Serial.println("closing SOUTH");
    maze.closeWall(x,y,SOUTH);
  }
};
//
// /*
// NORTH->EAST
// east -> SOUTH
// south->WEST
// WEST -> NORTH
// */



Position newPosition(short x,short y){
  motors.printBoolStatus();

  Position pos;
  pos.x = x;
  pos.y = y;
  return pos;
};


Position copyPosition(Position p, byte dir){
  motors.printBoolStatus();

  Position np=newPosition(p.x,p.y);
  np.direction = dir;
  return np;
}

boolean positionsMatch(Position a,Position b){
  motors.printBoolStatus();

  return a.x == b.x && a.y == b.y;
}

void explore(){
  motors.printBoolStatus();

  StackArray <Position> moves;
  StackArray <Position> possibleMoves;
  Position curPos = newPosition(0,0);
  moves.push(curPos);
  maze.visit(curPos.x,curPos.y);
  boolean moved = true;

  while (true) {
    motors.printBoolStatus();

    Serial.print(moved);
    Serial.println(" looping");
    updatemaze(curPos.x,curPos.y);
    Serial.println("done updating maze");

    if(moved){
      Serial.println("looking at available moves");
      //push available moves onto the stack;
      // if(!maze.wallPresent(curPos.x,curPos.y,WEST) && !maze.visited(curPos.x-1,curPos.y)){
      if(!motors.left && !maze.visited(curPos.x-1,curPos.y)){
        Serial.println("canMoveWest");
        possibleMoves.push(copyPosition(curPos,WEST));
      }
      // if(!maze.wallPresent(curPos.x,curPos.y,NORTH) && !maze.visited(curPos.x,curPos.y-1)){
      if(!motors.front && !maze.visited(curPos.x,curPos.y-1)){
        Serial.println("canMoveNORTH");
        possibleMoves.push(copyPosition(curPos,NORTH));
      }
      // if(!maze.wallPresent(curPos.x,curPos.y,EAST) && !maze.visited(curPos.x+1,curPos.y)){
      if(!motors.right && !maze.visited(curPos.x+1,curPos.y)){
        Serial.println("canMoveEAST");
        possibleMoves.push(copyPosition(curPos,EAST));
      }
      // if(!maze.wallPresent(curPos.x,curPos.y,SOUTH) && !maze.visited(curPos.x,curPos.y+1)){
      if(!motors.back && !maze.visited(curPos.x,curPos.y+1)){
        Serial.println("canMoveSOUTH");
        possibleMoves.push(copyPosition(curPos,SOUTH));
      }
      Serial.println(" done looking at available moves");

    }
    else{
      Serial.println("looks like we didnt move much");
    }
    if (positionsMatch(possibleMoves.peek(),curPos)) {
      Serial.println("moving now");
      Position p = possibleMoves.pop();
      switch(p.direction){
        case NORTH:
          p.y--;
          break;
        case SOUTH:
          p.y++;
          break;
        case WEST:
          p.x--;
          break;
        case EAST:
          p.x++;
          break;
      }
      maze.visit(p.x,p.y);
      moveRobot(p.direction);
      moves.push(p);
      curPos = p;
      moved = true;
    }
    else{
      Serial.println("didnt move");
      curPos = moves.pop();
      moved = false;
    }
  }
}

void go(){
  motors.moveForwardOne();
  motors.rotate90left();
  motors.moveForwardOne();
  motors.moveForwardOne();
  motors.rotate90right();
  motors.moveForwardOne();
  motors.rotate90left();
  motors.moveForwardOne();
  motors.moveForwardOne();
  motors.rotate90right();
  motors.moveForwardOne();

}

void loop(){
  go();
  // motors.rotate90right();
  // motors.rotate90right();
  // motors.rotate90right();
  // motors.rotate90right();
  // motors.rotate90right();
  // motors.rotate90right();
  // motors.rotate90right();
  // motors.rotate90right();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // // motors.rotate90left();
  // while(1);
  // explore();
  randomSearch();
  motors.updateSensors();
  // motors.printBoolStatus();
  motors.printStatus();
  delay(100);
}
