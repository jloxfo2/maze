#include "maze.h"


/*
 *  No parameter constructor
 */
SquareMaze::SquareMaze()
{
  mazeWidth = 0;
  mazeHeight = 0;
}


/*
 *  Makes a new Square Maze of the given height and width.
 */
void SquareMaze::makeMaze(int width, int height)
{

  // check if a maze already exists. If so, clear it and create a new one
  if (mazeHeight != 0) {
    grid.clear();

    DisjointSets z;
    cellDSet = z;
  }


  // set SquareMaze's data members
  mazeHeight = height;
  mazeWidth = width;
  cellDSet.addelements(width * height);
  

  // integer used to give each cell a number corresponding to an element in dsets
  int temp = 0;

  // set the size of the vector
  grid.resize(width);

  // create the grid of Cells
  for (int x=0; x < width; ++x) {
    grid[x].resize(height);

    for (int y=0; y<height; ++y) {
      // create a Cell in grid with appropriate number
      grid[x][y] = Cell(temp);
      temp++;
    }
  }


  // get a random location to start at in maze grid
  srand(time(NULL));
  int randomY = rand() % mazeHeight;
  int randomX = rand() % mazeWidth;
  initialCellNum = grid[randomX][randomY].cellNumber;


  // enter makeMaze() helper function
  makeMazeHelper(randomX, randomY);
}


/*
 *  makeMaze() helper function -- performs an iterative DFS to construct maze
 */
void SquareMaze::makeMazeHelper(int x, int y)
{
  srand(time(NULL));
  stack< pair<int, int> > cellStack;
  cellStack.push( pair<int, int>(x, y) );

  bool checkedAllFlag = false;

  
  while (!cellStack.empty()) {

      pair<int, int>currCell = cellStack.top();

      checkedAllFlag = false;
      x = currCell.first;
      y = currCell.second;

      int randomNeighbor = rand() % 4;
    
      for (int i=0; i<4; ++i) {

	  int random = (randomNeighbor + i) % 4;
    
	  if ( (random == 0) && isGood(x+1, y) ) {
		setWall(x, y, 0, false);
	        cellDSet.setunion(initialCellNum, grid[x+1][y].cellNumber);
	        cellStack.push( pair<int, int>(x+1, y) );
	        checkedAllFlag = true;
	        break;
	  }
	  else if ( (random == 1) && isGood(x, y+1) ) {
	        setWall(x, y, 1, false);
	        cellDSet.setunion(initialCellNum, grid[x][y+1].cellNumber);
	        cellStack.push( pair<int, int>(x, y+1) );
		checkedAllFlag = true;
		break;
	  }
	  else if ( (random == 2) && isGood(x-1, y) ) {
	  	setWall(x-1, y, 0, false);
	        cellDSet.setunion(initialCellNum, grid[x-1][y].cellNumber);
	        cellStack.push( pair<int, int>(x-1, y) );
		checkedAllFlag = true;
		break;
	  }
	  else if ( (random == 3) && isGood(x, y-1) ) {
	  	setWall(x, y-1, 1, false);
		cellDSet.setunion(initialCellNum, grid[x][y-1].cellNumber);
	        cellStack.push( pair<int, int>(x, y-1) );
		checkedAllFlag = true;
		break;
	  }
	}

	if (!checkedAllFlag) {
	    cellStack.pop();
	}
  }

  
  // recursive DFS -- didn't handle large mazes so an iterative DFS was used instead
  /*
   *
   *
   
  int randomNeighbor = rand() % 4;

  for (int i=0; i<4; ++i) {

      int random = (randomNeighbor + i) % 4;
    
      if ( (random == 0) && isGood(x+1, y) ) {
	      setWall(x, y, 0, false);
	      cellDSet.setunion(initialCellNum, grid[x+1][y].cellNumber);
	      makeMazeHelper(x+1, y);
      }
      else if ( (random == 1) && isGood(x, y+1) ) {
	      setWall(x, y, 1, false);
	      cellDSet.setunion(initialCellNum, grid[x][y+1].cellNumber);
	      makeMazeHelper(x, y+1);
      }
      else if ( (random == 2) && isGood(x-1, y) ) {
	      setWall(x-1, y, 0, false);
	      cellDSet.setunion(initialCellNum, grid[x-1][y].cellNumber);
	      makeMazeHelper(x-1, y);
      }
      else if ( (random == 3) && isGood(x, y-1) ){
	      setWall(x, y-1, 1, false);
	      cellDSet.setunion(initialCellNum, grid[x][y-1].cellNumber);
	      makeMazeHelper(x, y-1);
      }
  }
  
  *
  */
}


/*
 *  Helper function -- determines if a cell is within bounds and hasn't been checked yet
 */
bool SquareMaze::isGood(int x, int y)
{
  if ( (x < 0) || (x >= mazeWidth) )
    return false;
  else if ( (y < 0) || (y >= mazeHeight) )
    return false;

  int currCellNum = grid[x][y].cellNumber;
  
  if ( cellDSet.find(initialCellNum) == cellDSet.find(currCellNum) )
    return false;

  return true;
}
		 


/*
 *  Function determines whether it is possible to travel in the given direction
 *  from the square at the coordinates (x,y)
 */
bool SquareMaze::canTravel(int x, int y, int dir) const
{
  if (dir == 0) {
      if ( ((x+1) < 0) || ((x+1) >= mazeWidth) )
          return false;
      if (grid[x][y].hasRightWall)
      	  return false;
  }
  else if (dir == 1) {
      if ( ((y+1) < 0) || ((y+1) >= mazeHeight) )
          return false;
      if (grid[x][y].hasDownWall)
	  return false;
  }
  else if (dir == 2) {
      if ( ((x-1) < 0) || ((x-1) >= mazeWidth) )
	  return false;
      if (grid[x-1][y].hasRightWall)
          return false;
  }
  else if (dir == 3) {
      if ( ((y-1) < 0) || ((y-1) >= mazeHeight) )
          return false;
      if (grid[x][y-1].hasDownWall)
          return false;
  }

  
  return true;
}


/*
 *  Sets a specified wall within a cell to exist or not exist (based on parameter)
 */
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
  if (dir == 0) {
      grid[x][y].hasRightWall = exists;
  }
  else {
      grid[x][y].hasDownWall = exists;
  }
}


/*
 *  Solves the maze by finding the longest path from top-left corner to the bottom row
 */
std::vector<int> SquareMaze::solveMaze()
{
  // stack holds pairs containing the coordinates of each cell
  stack< pair<int, int> > cellStack;
  vector<int> path;
  vector<int> longestPath;
  int longestPathX = mazeWidth;  // holds the x-coord for current-best maze-exit
  DisjointSets visitedCells;  // DSet to group cells that have been visited

  // push the origin (0,0) on to the stack
  cellStack.push( pair<int, int>(0,0) );

  // initialize DSet
  visitedCells.addelements(mazeHeight * mazeWidth);
  

  while (!cellStack.empty()) {

      // set the cell on top of stack as the current cell
      pair<int, int> currCell = cellStack.top();
      int x = currCell.first;
      int y = currCell.second;

      // mark currCell as visited;
      visitedCells.setunion( grid[0][0].cellNumber, grid[x][y].cellNumber );


      //check the neighbors of the current Cell. If any of them haven't been visited
      // and can be traveled to, then push the first one on the stack and repeat 
      // this check on the next loop iteration.
      if (canTravel(x, y, 0) && !visited(x+1, y, visitedCells)) {
          cellStack.push( pair<int, int>(x+1, y) );
          path.push_back(0);
      }
      else if (canTravel(x, y, 1) && !visited(x, y+1, visitedCells)) {
          cellStack.push( pair<int, int> (x, y+1) );
          path.push_back(1);
      }
      else if (canTravel(x, y, 2) && !visited(x-1, y, visitedCells)) {
          cellStack.push( pair<int, int>(x-1, y) );
          path.push_back(2);
      }
      else if (canTravel(x, y, 3) && !visited(x, y-1, visitedCells)) {
          cellStack.push( pair<int, int>(x, y-1) );
          path.push_back(3);
      }
      else {
          // if current cell is on bottom row and x-coord is less than
          //smallest, check if current path is greater than longest path
          if ( y == mazeHeight - 1 ) {
              if (longestPath.size() == 0) {
                  // checks case if longestPath hasn't been initialized (size=0)
                  longestPath = path;
                  longestPathX = x;
              }
              else if (path.size() > longestPath.size()) {
                  longestPath = path;
                  longestPathX = x;
	      }
              else if (path.size() == longestPath.size() && x < longestPathX) {
                  longestPath = path;
                  longestPathX = x;
              }
          }

	  // pop the current path & cell, then iterate next cell for new paths
	  cellStack.pop();
	  if (path.size() != 0)
	      // check because stack.size = (vector.size + 1) -- off by 1
	      path.pop_back();
      }
  }

  return longestPath;
}


/*
 *  Helper function used by solveMaze() to determine if a cell has been visited
 */
bool SquareMaze::visited(int x, int y, DisjointSets &visitedCells)
{
  if ( (x < 0) || (x >= mazeWidth) )
      return false;
  else if ( (y < 0) || (y >= mazeHeight) )
      return false;

  int currCellNum = grid[x][y].cellNumber;
  int origin = grid[0][0].cellNumber;
  
  if ( visitedCells.find(origin) == visitedCells.find(currCellNum) )
      return true;

  return false;
}


/*
 *  Draws the maze without the solution.
 */
PNG * SquareMaze::drawMaze() const
{
  PNG * output = new PNG(10*mazeWidth + 1, 10*mazeHeight + 1);


  // blacken the topmost row
  for (int i=10; i < (mazeWidth*10 + 1); ++i) {
      (*output)(i, 0)->red = 0;
      (*output)(i, 0)->green = 0;
      (*output)(i, 0)->blue = 0;
  }


  // blacken the leftmost column, except for starting position (top-left)
  for (int i=0; i < (mazeHeight*10 + 1); ++i) {
      (*output)(0, i)->red = 0;
      (*output)(0, i)->green = 0;
      (*output)(0, i)->blue = 0;
  }
  
  
  for (int i=0; i < mazeWidth; ++i) {
      for (int j=0; j < mazeHeight; ++j) {
          drawWall(i, j, output);
      }
  }
  
  return output;
}


/*
 *  Helper function used by drawMaze to color the walls for each cell
 */
void SquareMaze::drawWall(int x, int y, PNG * output) const
{
  if (grid[x][y].hasRightWall) {
      for (int i=0; i<11; ++i) {
          (*output)((x+1)*10, y*10 + i)->red = 0;
          (*output)((x+1)*10, y*10 + i)->green = 0;
          (*output)((x+1)*10, y*10 + i)->blue = 0;
      }
  }

  
  if (grid[x][y].hasDownWall) {
      for (int i=0; i<11; ++i) {
          (*output)(x*10 + i, (y+1)*10)->red = 0;
          (*output)(x*10 + i, (y+1)*10)->green = 0;
          (*output)(x*10 + i, (y+1)*10)->blue = 0;
      }
  }
}


/*
 *  This function calls drawMaze(), then solveMaze(); modifies the PNG from
 *  drawMaze() to show the solution vector and the exit.
 */
PNG * SquareMaze::drawMazeWithSolution()
{
  PNG * output = drawMaze();
  vector<int> solutionPath = solveMaze();


  // coords to be incremented for every path direction -- used for drawing exit
  int exitXCoord = 0;
  int exitYCoord = 0;

  // coordinates of current position in PNG
  int x = 5;
  int y = 5;

  // loop through every direction in path; draw red line in appropriate direction
  for (size_t i=0; i < solutionPath.size(); ++i) {
      if (solutionPath[i] == 0) {
          for (int j=0; j<11; ++j) {
              (*output)(x+j, y)->red = 255;
	      (*output)(x+j, y)->green = 0;
	      (*output)(x+j, y)->blue = 0;
	  }

      x += 10;
      exitXCoord++;
      }
      else if (solutionPath[i] == 1) {
          for (int j=0; j<11; ++j) {
              (*output)(x, y+j)->red = 255;
              (*output)(x, y+j)->green = 0;
              (*output)(x, y+j)->blue = 0;
          }

      y += 10;
      exitYCoord++;
      }
      else if (solutionPath[i] == 2) {
          for (int j=0; j<11; ++j) {
              (*output)(x-j, y)->red = 255;
              (*output)(x-j, y)->green = 0;
              (*output)(x-j, y)->blue = 0;
          }

          x -= 10;
          exitXCoord--;
      }
      else if (solutionPath[i] == 3) {
          for (int j=0; j<11; ++j) {
              (*output)(x, y-j)->red = 255;
              (*output)(x, y-j)->green = 0;
              (*output)(x, y-j)->blue = 0;
          }

          y -= 10;
          exitYCoord--;
      }
  }


  // remove wall for the maze exit
  for (int k=1; k <10; ++k) {
      (*output)(exitXCoord*10 + k, (exitYCoord+1)*10)->red = 255;
      (*output)(exitXCoord*10 + k, (exitYCoord+1)*10)->green = 255;
      (*output)(exitXCoord*10 + k, (exitYCoord+1)*10)->blue = 255;
  }
  
  return output;
}


/*
 *  defaulte Cell constructor
 */
SquareMaze::Cell::Cell()
{
  hasRightWall = true;
  hasDownWall = true;
  cellNumber = 0;
}


/*
 *  Cell constructor -- includes param. for cellNumber initialization
 */
SquareMaze::Cell::Cell(int number)
{
  hasRightWall = true;
  hasDownWall = true;
  cellNumber = number;
}
