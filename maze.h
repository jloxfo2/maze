#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <stack>
#include <queue>
#include "png.h"
#include "dsets.h"

using namespace std;


class SquareMaze
{
 public:

   /*
    *  No parameter constructor
    */
  SquareMaze();


   /*
    *  Makes a new Square Maze of the given height and width.
    */
  void makeMaze(int width, int height);


   /*
    *  This uses your representation of the maze to determine whether it is possible
    *  to travel in the given direction from the square at the coordinates (x,y)
    */
  bool canTravel(int x, int y, int dir) const;


  /*
   *  Sets a specified wall within a cell to exist or not exist (based on parameter)
   */
  void setWall(int x, int y, int dir, bool exists);


   /*
    *  Solves this SquareMaze.
    */
  std::vector<int> solveMaze();


   /*
    *  Draws the maze without the solution.
    */
  PNG * drawMaze() const;


   /*
    *  This function calls drawMaze(), then solveMaze(); modifies the PNG from
    *  drawMaze() to show the solution path and the exit.
    */
  PNG * drawMazeWithSolution();


 private:

  /*
   *  nested Cell class
   */
  class Cell
  {
  public:
    // Each cell has 2 walls, right & down.
    // Left & up walls can be accessed from adjacent cells
    bool hasRightWall;
    bool hasDownWall;

    // Number corresponding to place in disjoint set
    int cellNumber;

    // Constructor -- no parameters
    Cell();

    // Constructor -- param given to initialize cellNumber
    Cell(int number);
  };


  int mazeWidth;
  int mazeHeight;
  int initialCellNum;
  vector< vector<Cell> > grid;   // grid implementation that holds the Cells 
  DisjointSets cellDSet;

  // Helper function used by makeMaze()
  void makeMazeHelper(int x, int y);


  // Helper function -- determines if a cell is within bounds and hasn't been checked yet
  bool isGood(int x, int y);


  // Helper function used by drawMaze() -- draws walls for each cell
  void drawWall(int x, int y, PNG * output) const;


  // Helper function used by solveMaze() -- checks if a cell has been visited
  bool visited(int x, int y, DisjointSets &visitedCells);
};

#endif
