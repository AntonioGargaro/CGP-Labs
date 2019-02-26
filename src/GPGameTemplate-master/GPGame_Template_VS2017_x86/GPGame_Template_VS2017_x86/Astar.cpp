#include "Astar.h";

#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <random>

const int n = 20; // horizontal size of the map
const int m = 20; // vertical size size of the map
static int closed_nodes_map[n][m]; // map of closed (tried-out) nodes
static int open_nodes_map[n][m]; // map of open (not-yet-tried) nodes
static int dir_map[n][m]; // map of directions
const int dir = 4; // number of possible directions to go at any position
// if dir==4
static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};
// if dir==8
//static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
//static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

Node::Node(int xp, int yp, int d, int p) {
	xPos = xp; 
	yPos = yp; 
	level = d; 
	priority = p;
}

int Node::getxPos() const { return xPos; }
int Node::getyPos() const { return yPos; }
int Node::getLevel() const { return level; }
int Node::getPriority() const { return priority; }

void Node::updatePriority(const int & xDest, const int & yDest) {
	priority = level + estimate(xDest, yDest) * 10; //A*
}

// give better priority to going strait instead of diagonally
void Node::nextLevel(const int & i) // i: direction
{
	level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
}

// Estimation function for the remaining distance to the goal.
const int & Node::estimate(const int & xDest, const int & yDest) const
{
	static int xd, yd, d;
	xd = xDest - xPos;
	yd = yDest - yPos;

	// Euclidian Distance
	//d = static_cast<int>(sqrt(xd*xd + yd * yd));

	// Manhattan distance
	d=abs(xd)+abs(yd);

	// Chebyshev distance
	//d=max(abs(xd), abs(yd));

	return(d);
}

// Determine priority (in the priority queue)
bool operator<(const Node & a, const Node & b)
{
	return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
std::string pathFind(int** grid, const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish)
{
	static std::priority_queue<Node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static Node* n0;
	static Node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	// reset the node maps
	for (y = 0; y < m; y++) {
		for (x = 0; x < n; x++) {
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}

	// create the start node and push into list of open nodes
	n0 = new Node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

	// A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new Node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish) {
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xStart && y == yStart)) {
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty())
				pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i < dir; i++) {
			xdx = x + dx[i]; 
			ydy = y + dy[i];

			if (!(xdx<0 || xdx>n - 1 || ydy<0 || ydy>m - 1 || grid[xdx][ydy] == 1
				|| closed_nodes_map[xdx][ydy] == 1)) {
				// generate a child node
				m0 = new Node(xdx, ydy, n0->getLevel(),	n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0) {
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy] > m0->getPriority()) {
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy)) {
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

					// empty the larger size pq to the smaller one
					if (pq[pqi].size() > pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty()) {
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
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


int** randomObstacles(int** grid, int numObs, int row, int col) {

	while (numObs > 0) {
		int rand_row = getRand(0, row - 1);
		int rand_col = getRand(0, col - 1);

		grid[rand_row][rand_col] = 1;
		numObs--;

	}


	return grid;
}

std::string runAstar(int** grid)
{
	srand(time(NULL));

	// fillout the grid matrix with obstacles
	randomObstacles(grid, 80, 20, 20);

	// randomly select start and finish locations
	int xA, yA, xB, yB;
	switch (rand() % 8)	{ // change depending on size of grid
	case 0: xA = 0;
			yA = 0;
			xB = n - 1;
			yB = m - 1; 
			break;
	case 1: xA = 0;
			yA = m - 1;
			xB = n - 1;
			yB = 0; 
			break;
	case 2: xA = n / 2 - 1;
			yA = m / 2 - 1;
			xB = n / 2 + 1;
			yB = m / 2 + 1; 
			break;
	case 3: xA = n / 2 - 1;
			yA = m / 2 + 1;
			xB = n / 2 + 1;
			yB = m / 2 - 1; 
			break;
	case 4: xA = n / 2 - 1;
			yA = 0;
			xB = n / 2 + 1;
			yB = m - 1; 
			break;
	case 5: xA = n / 2 + 1;
			yA = m - 1;
			xB = n / 2 - 1;
			yB = 0; 
			break;
	case 6: xA = 0;
			yA = m / 2 - 1;
			xB = n - 1;
			yB = m / 2 + 1; 
			break;
	case 7: xA = n - 1;
			yA = m / 2 + 1;
			xB = 0;
			yB = m / 2 - 1; 
			break;
	}

	std::cout << "grid Size (X,Y): " << n << "," << m << std::endl;
	std::cout << "Start: " << xA << "," << yA << std::endl;
	std::cout << "Finish: " << xB << "," << yB << std::endl;
	// get the route
	clock_t start = clock();
	std::string route = pathFind(grid, xA, yA, xB, yB);
	if (route == "") std::cout << "An empty route generated!" << std::endl;
	clock_t end = clock();
	double time_elapsed = double(end - start);
	std::cout << "Time to calculate the route (ms): " << time_elapsed << std::endl;
	std::cout << "Route:" << std::endl;
	std::cout << route << std::endl << std::endl;

	// follow the route on the grid and display it 
	if (route.length() > 0)
	{
		int j; char c;
		int x = xA;
		int y = yA;
		grid[x][y] = 2;
		for (int i = 0;i < route.length();i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			grid[x][y] = 3;
		}
		grid[x][y] = 4;

		// display the grid with the route
		for (int y = 0;y < m;y++)
		{
			for (int x = 0;x < n;x++)
				if (grid[x][y] == 0)
					std::cout << ".";
				else if (grid[x][y] == 1)
					std::cout << "O"; //obstacle
				else if (grid[x][y] == 2)
					std::cout << "S"; //start
				else if (grid[x][y] == 3)
					std::cout << "R"; //route
				else if (grid[x][y] == 4)
					std::cout << "F"; //finish
			std::cout << std::endl;
		}
	}
	//getchar(); // wait for a (Enter) keypress  
	return(route);
}
