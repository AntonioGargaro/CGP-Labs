#pragma once
#include "Global.h"
#include <queue>
#include <limits>
#include <cmath>

class Node
{
	// x and y coords
	int x;
	int y;

	// total cost already travelled to reach the node
	int cost;
	
	// heuristic = cost + manhattan distance
	int priority;

public:
	Node(int xp, int yp, int d, int p);

	int getX() const;
	int getY() const;
	int getCost() const;
	int getPriority() const;

	void updatePriority(int & xF, int & yF);

	// Estimation function for the remaining distance to the goal.
	const int & calcHeuristic(int & xF, int & yF);
};

int** createGrid(int n, int m);

std::string runAstar(int** grid);