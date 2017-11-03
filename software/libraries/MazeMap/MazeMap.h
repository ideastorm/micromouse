#ifndef MICROMOUSE_MAP_H
#define MICROMOUSE_MAP_H
#include "arduino.h"

/*
 * Class to represent a micromouse maze.  As you navigate the maze, you'll need to detect whether
 * walls are present, and use the closeWall method to record them.
 * After you've closed walls, use the solve method to recalculate the distances to the target.
 * Use the bestDirection method to determine the next direction to go to follow the shortest known path to the target.
 * Unexplored areas will generally be considered closer, unless there's a nearly straight shot from the start to the destination.
 */

//Direction flags.  The starting space is represented by the northwest (top left) corner of the maze.
const byte NORTH = 1;
const byte SOUTH = 2;
const byte EAST  = 4;
const byte WEST  = 8;

//Solve targets.  Target DESTINATION when searching for the center,
//target HOME when returning to the start square
const byte HOME = 0;
const byte DESTINATION = 1;

#define MAZE_SIZE 6
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
    void closeWall(short x, short y, byte direction);
    byte bestDirection(short x, short y);
    boolean isTarget(short x, short y, byte targetType);
    void solve(byte targetType);
    void print();
    String bestPath(short startX, short startY, byte targetType);
	void visit(short x, short y);
	boolean visited(short x, short y);
	void unvisit(short x, short y);
	boolean wallPresent(short x, short y, byte direction);

  private:
    boolean wallsNS[MAZE_SIZE][MAZE_SIZE + 1];
    boolean wallsEW[MAZE_SIZE + 1][MAZE_SIZE];
	boolean breadCrumbs[MAZE_SIZE][MAZE_SIZE];
    byte distances[MAZE_SIZE][MAZE_SIZE];
    void resetDistances();
    boolean updateDistances(short x, short y);
};

#endif
