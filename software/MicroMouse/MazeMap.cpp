#include "MazeMap.h"

MazeMap::MazeMap() {
  for (int x = 0; x < MAZE_SIZE; x++) {
    for (int y = 1; y < MAZE_SIZE; y++) {
      wallsNS[x][y] = false;
    }
    wallsNS[x][0] = true;
    wallsNS[x][MAZE_SIZE] = true;
  }
  for (int y = 0; y < MAZE_SIZE; y++) {
    for (int x = 1; x < MAZE_SIZE; x++) {
      wallsEW[x][y] = false;
    }
    wallsEW[0][y] = true;
    wallsEW[MAZE_SIZE][y] = true;
  }
}

boolean MazeMap::closeWall(int x, int y, int direction) {
  boolean updated = false;
  switch (direction) {
    case NORTH:
      updated = !wallsNS[x][y];
      wallsNS[x][y] = true;
      break;
    case SOUTH:
      updated = !wallsNS[x][y + 1];
      wallsNS[x][y + 1] = true;
      break;
    case EAST:
      updated = !wallsEW[x + 1][y];
      wallsEW[x + 1][y] = true;
      break;
    case WEST:
      updated = !wallsEW[x][y];
      wallsEW[x][y] = true;
      break;
  }
  return updated;
}

boolean MazeMap::wallPresent(int x, int y, int direction) {
  switch (direction) {
    case NORTH:
      return wallsNS[x][y];
      break;
    case SOUTH:
      return wallsNS[x][y + 1];
      break;
    case EAST:
      return wallsEW[x + 1][y];
      break;
    case WEST:
      return wallsEW[x][y];
      break;
  }
  return true;
}

byte MazeMap::bestDirection(int x, int y) {
  int n = y > 0 ? distances[x][y - 1] : 100;
  int s = y < MAZE_SIZE - 1 ? distances[x][y + 1] : 100;
  int w = x > 0 ? distances[x - 1][y] : 100;
  int e = x < MAZE_SIZE - 1 ? distances[x + 1][y] : 100;
  int lowest = min(min(n, s), min(e, w));
  if (lowest == n) return NORTH;
  if (lowest == s) return SOUTH;
  if (lowest == e) return EAST;
  return WEST;
}

boolean MazeMap::isTarget(int x, int y, int targetType) {
  if (targetType == HOME)
    return x == 0 && y == 0;
  else
    return (x == HALF_SIZE || x == HALF_SIZE - 1) && (y == HALF_SIZE || y == HALF_SIZE - 1);
}

void MazeMap::solve(int targetType) {
  if (targetType == HOME) {
    distances[0][0] = 0;
  } else {
    distances[HALF_SIZE][HALF_SIZE] = 0;
    distances[HALF_SIZE - 1][HALF_SIZE] = 0;
    distances[HALF_SIZE][HALF_SIZE - 1] = 0;
    distances[HALF_SIZE - 1][HALF_SIZE - 1] = 0;
  }
  boolean continueSolving;
  int passes = 0;
  Serial.println("Solving the maze");
  long startTime = millis();
  do {
    passes++;
    continueSolving = false;
    for (int x = 0; x < MAZE_SIZE; x++) {
      for (int y = 0; y < MAZE_SIZE; y++) {
        continueSolving |= updateDistances(x, y);
      }
    }
  } while (continueSolving);
  String message = "Solved the maze in ";
  message = message + passes;
  message = message + " passes (";
  long delta = millis() - startTime;
  message += delta;
  message += " milliseconds)";
  Serial.println(message);
}

byte* MazeMap::bestPath(int startX, int startY, int targetType) {
  int x = startX;
  int y = startY;
  solve(targetType);
  int pathIndex = 0;
  while (!isTarget(x, y, targetType)) {
    byte nextDir = bestDirection(x, y);
    path[pathIndex++] = nextDir;
    switch (nextDir) {
      case NORTH:
        y--;        
        break;
      case SOUTH:
        y++;
        break;
      case EAST:
        x++;
        break;
      case WEST:
        x--;
        break;
    }
  }
  path[pathIndex] = 255;
  return path;
}

boolean MazeMap::updateDistances(int x, int y) {
  int startDistance = distances[x][y];
  if (startDistance > 0) {
    int n = y > 0 ? (wallPresent(x, y, NORTH) ? 100 : distances[x][y - 1]) : 100;
    int s = y < MAZE_SIZE - 1 ? (wallPresent(x, y, SOUTH) ? 100 : distances[x][y + 1]) : 100;
    int w = x > 0 ? (wallPresent(x, y, WEST) ? 100 : distances[x - 1][y]) : 100;
    int e = x < MAZE_SIZE - 1 ? (wallPresent(x, y, EAST) ? 100 : distances[x + 1][y]) : 100;
    distances[x][y] = min(min(n, s), min(e, w)) + 1;
  }
  return startDistance != distances[x][y];
}

void MazeMap::resetDistances() {
  for (int x = 0; x < MAZE_SIZE; x++) {
    for (int y = 0; y < MAZE_SIZE; y++) {
      distances[x][y] = MAZE_SIZE * 2;
    }
  }
}

void MazeMap::print() {
  {
    for (byte i = 0; i < 2 * MAZE_SIZE + 1; i++)
    {
      for (byte j = 0; j < 2 * MAZE_SIZE + 1; j++)
      {
        //Add Horizontal Walls
        if (i % 2 == 0 && j % 2 == 1)
        {
          if (wallsNS[j / 2][i / 2] == true)
          {
            Serial.print(" ---");
          }
          else
          {
            Serial.print("    ");
          }
        }

        //Add Vertical Walls
        if (i % 2 == 1 && j % 2 == 0)
        {
          if (wallsEW[j / 2][i / 2] == true)
          {
            Serial.print("|");
          }
          else
          {
            Serial.print(" ");
          }
        }

        //Add Flood Fill Values
        if (i % 2 == 1 && j % 2 == 1)
        {
          byte value = distances[(j - 1) / 2][(i - 1) / 2];
          if (value >= 100)
          {
            Serial.print(value);
          }
          else
          {
            Serial.print(" ");
            Serial.print(value);
          }
          if (value < 10)
          {
            Serial.print(" ");
          }
        }
      }
      Serial.print("\n");
    }
    Serial.print("\n");
  }
}

