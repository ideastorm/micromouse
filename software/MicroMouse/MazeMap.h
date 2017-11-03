#ifndef MICROMOUSE_MAP_H
#define MICROMOUSE_MAP_H
#include "arduino.h"

/*
 * Class to represent a micromouse maze.  As you navigate the maze, you'll need to detect whether walls are present, and use the closeWall method to record them.
 * After you've closed walls, use the solve method to recalculate the distances to the target.
 * Use the bestDirection method to determine the next direction to go to follow the shortest known path to the target.
 * Unexplored areas will generally be considered closer, unless there's a nearly straight shot from the start to the destination.
 */

//Direction flags.  The starting space is represented by the northwest (top left) corner of the maze.
const int NORTH = 1;
const int EAST  = 2;
const int SOUTH = 3;
const int WEST  = 4;

//Solve targets.  Target CENTER when searching for the center, target HOME when returning to the start square
const int HOME = 0;
const int CENTER = 1;

#define MAZE_SIZE 8
const int HALF_SIZE = MAZE_SIZE / 2;


//  - -    Maze size = 2, 2 columns (x) containing 3 rows (y) of north-south blocking walls
// | | |     3 columns (x) containing 2 rows (y) of east-west blocking walls
//  - -
// | | |
//  - -
class MazeMap
{
  public:
    MazeMap();
    boolean closeWall(int x, int y, int direction); //returns true if an update was made
    byte bestDirection(int x, int y);
    boolean isTarget(int x, int y, int targetType);
    void solve(int targetType);
    void print();
    byte* bestPath(int startX, int startY, int targetType);
  private:
    boolean wallsNS[MAZE_SIZE][MAZE_SIZE + 1];
    boolean wallsEW[MAZE_SIZE + 1][MAZE_SIZE];
    byte distances[MAZE_SIZE][MAZE_SIZE];
    byte path[MAZE_SIZE * MAZE_SIZE - 7]; //maximum path length for a valid maze
    boolean wallPresent(int x, int y, int direction);
    void resetDistances();
    boolean updateDistances(int x, int y);
};

#endif
