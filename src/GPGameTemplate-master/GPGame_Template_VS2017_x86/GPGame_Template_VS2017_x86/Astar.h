#pragma once
#include <queue>
#include <limits>
#include <cmath>

class Node
{
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority

public:
	Node(int xp, int yp, int d, int p);

	int getxPos() const;
	int getyPos() const;
	int getLevel() const;
	int getPriority() const;

	void updatePriority(const int & xDest, const int & yDest);

	// give better priority to going strait instead of diagonally
	void nextLevel(const int & i); // i: direction

	// Estimation function for the remaining distance to the goal.
	const int & estimate(const int & xDest, const int & yDest) const;
};

int** createGrid(int n, int m);

std::string runAstar(int** grid);