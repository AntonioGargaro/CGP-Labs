#include "Astar.h";

#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <random>

Node::Node(int xp, int yp, int d, int p) {
	x = xp; 
	y = yp; 
	cost = d; 
	priority = p;
}

int Node::getX() const {
	return x; 
}

int Node::getY() const { 
	return y; 
}

int Node::getCost() const { 
	return cost; 
}

int Node::getPriority() const {
	return priority;
}

void Node::updatePriority(int & xF, int & yF) {
	priority = cost + calcHeuristic(xF, yF) * 10; //A*
}

// Manhattan distance from cur pos to finish point
const int & Node::calcHeuristic(int & xF, int & yF) {
	// Manhattan distance
	return(abs(xF - x) + abs(yF - y));
}

// Calculate priority in the priority queue
bool operator<(Node a, Node b) {
	return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
std::string findPath(int** grid, int & xS, int & yS, int & xF, int & yF) {

	int visited_nodes[ROW][COL]; // 2d arr of visited discovered nodes
	int unvisited_nodes[ROW][COL]; // 2d arr of unvisited discovered nodes
	int dir_map[ROW][COL]; // map of directions

	const int dir = 4; // number of possible directions to go at any position
	int dir_x[dir] = { 1, 0, -1, 0 };
	int dir_y[dir] = { 0, 1, 0, -1 };

	std::priority_queue<Node> node_pq[2];	// array of node priority queues
	int index = 0;							// node_pq index

	Node* n1;
	Node* n2;


	// set un/visited nodes to none
	for (int y = 0; y < COL; y++) {
		for (int x = 0; x < ROW; x++) {
			visited_nodes[x][y] = 0;
			unvisited_nodes[x][y] = 0;
		}
	}

	// create the start node and push into list of unvisited nodes
	n1 = new Node(xS, yS, 0, 0);	// Node on start position
	n1->updatePriority(xF, yF);		// Calc its priority value (manhattan from start to end pos)
	node_pq[index].push(*n1);
	unvisited_nodes[xS][yS] = n1->getPriority(); // mark it in the unvisited discovered nodes

	// A* search
	while (!node_pq[index].empty())	{
		// get the highest priority node
		// from the unvisited discovered nodes
		n1 = new Node(
			node_pq[index].top().getX(), 
			node_pq[index].top().getY(),
			node_pq[index].top().getCost(), 
			node_pq[index].top().getPriority()
		);

		// current x and y of nodes
		int x = n1->getX(); 
		int y = n1->getY();

		node_pq[index].pop();		// remove the node from the unvisited queue
		unvisited_nodes[x][y] = 0;	// unmark it from unvisited discoved nodes
		visited_nodes[x][y] = 1;	// mark it on visited nodes

		// return from search when on finished positions
		if (x == xF && y == yF) {
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xS && y == yS)) {
				int temp = dir_map[x][y];
				char c = '0' + (temp + dir / 2) % dir;
				path = c + path;
				x += dir_x[temp];
				y += dir_y[temp];
			}

			// empty the leftover nodes
			while (!node_pq[index].empty())
				node_pq[index].pop();

			// follow the route on the grid 
			if (path.length() > 0) {
				int j; 
				char c;
				int x = xS;
				int y = yS;
				
				grid[x][y] = 2;
				
				for (int i = 0;i < path.length(); i++) {
					c = path.at(i);
					j = atoi(&c);
					x = x + dir_x[j];
					y = y + dir_y[j];
					grid[x][y] = 3;
				}
				grid[x][y] = 4;
			}
			return path;
		}

		// check possible moves all directions
		for (int i = 0; i < dir; i++) {

			int x_dir = x + dir_x[i];	// Calc next x coord
			int y_dir = y + dir_y[i];	// Calc next y coord

			if (!(
				x_dir < 0 || x_dir > ROW - 1		// x coord still inside grid
				|| y_dir < 0 || y_dir > COL - 1		// y coord still inside grid
				|| grid[x_dir][y_dir] == 1			// next position isnt an obstacle
				|| visited_nodes[x_dir][y_dir] == 1	// next position hasn't been visited
				)) {

				// Create new node of this updated position
				n2 = new Node(x_dir, y_dir, n1->getCost(),	n1->getPriority());
				n2->updatePriority(xF, yF);	// Calc its priority level to finish

				// if not in unvisited discovered list then add it
				if (unvisited_nodes[x_dir][y_dir] == 0) {
					unvisited_nodes[x_dir][y_dir] 
						= n2->getPriority();	// Set priority level

					node_pq[index].push(*n2);	// push into queue

					// mark its parent's direction into direction map
					dir_map[x_dir][y_dir] = (i + dir / 2) % dir;
				}
				else if (// if current priority is smaller than previous priority
							unvisited_nodes[x_dir][y_dir] > n2->getPriority()) {
					// update the priority info
					unvisited_nodes[x_dir][y_dir] = n2->getPriority();
					// update its parent's direction into direction map
					dir_map[x_dir][y_dir] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one node_pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(node_pq[index].top().getX() == x_dir 
							&& node_pq[index].top().getY() == y_dir)) {
						// push all nodes except current to other queue
						node_pq[1 - index].push(node_pq[index].top());	
						// then pop node from the current queue
						node_pq[index].pop();
					}

					node_pq[index].pop(); // remove the current node

					// Swap index to the smaller queue
					if (node_pq[index].size() > node_pq[1 - index].size()) 
						index = 1 - index;

					// Push remaining nodes to bigger queue
					while (!node_pq[index].empty()) {
						node_pq[1 - index].push(node_pq[index].top());
						node_pq[index].pop();
					}

					index = 1 - index;			// Swap index back to bigger queue
					node_pq[index].push(*n2);	// add the low node instead
				}
			}
		}
	}
	return ""; // no route found
}


// Create a grid of row by col
int** createGrid(int row, int col) {
	int** arr = new int*[row];
	for (int i = 0; i < row; ++i) {
		arr[i] = new int[col];
		for (int j = 0; j < col; ++j) {
			arr[i][j] = 0; // make empty map
		}
	}
	return arr;
}

int getRand(int from, int to) {
	// Generator from random float
	static std::random_device rd; // random device engine, usually based on /dev/random on UNIX-like systems
	// initialize Mersennes' twister using rd to generate the seed
	static std::mt19937 generator(rd());
	std::uniform_int_distribution<int> dis(from, to);
	return dis(generator);
}


int** randomObstacles(int** grid, int numObs) {
	while (numObs > 0) {
		int rand_row = getRand(0, ROW - 1);
		int rand_col = getRand(0, COL - 1);

		grid[rand_row][rand_col] = 1;
		numObs--;
	}
	return grid;
}

std::string runAstar(int** grid) {

	// fillout the grid matrix with obstacles
	randomObstacles(grid, 300);

	// randomly select start and finish locations
	int xS = getRand(0, (ROW / 2) - (ROW / 4)),
		yS = getRand(0, COL / 2), 
		xF = getRand((ROW / 2) + (ROW / 4), ROW - 1),
		yF = getRand(COL / 2, COL - 1);
	
	// get the route
	std::string route = findPath(grid, xS, yS, xF, yF);
	if (route == "") 
		std::cout << "No route found, trying again" << std::endl; 

	return(route);
}
