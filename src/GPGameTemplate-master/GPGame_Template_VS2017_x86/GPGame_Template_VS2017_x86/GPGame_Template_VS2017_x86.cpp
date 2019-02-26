// Simplified Renderer application for GP course
// Features:
// Reduced OpenGL version from 4.5 to 3.3 to allow it to render in older laptops.
// Added Shapes library for rendering cubes, spheres and vectors.
// Added examples of matrix multiplication on Update.
// Added resize screen and keyboard callbacks.
// Added FPS camera functionality
// Update 2019/01 updated libraries and created project for VS2017 including directory dependant links to libraries.

// Suggestions or extra help please do email me at S.Padilla@hw.ac.uk

// Standard C++ libraries
#include <iostream>
#include <vector>

using namespace std;

// Helper graphic libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "Global.h"
#include "graphics.h"
#include "shapes.h"
#include "Collision.h"
#include "Physics.h"
#include "Astar.h"



// MAIN FUNCTIONS
void startup();
void updateCamera();
void updateSceneElements();
void renderScene();

// CALLBACK FUNCTIONS
void onResizeCallback(GLFWwindow* window, int w, int h);	
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);

// VARIABLES
bool		quit = false;
float		deltaTime = 0.0f;	// Keep track of time per frame.
float		lastTime = 0.0f;	// variable to keep overall time.
bool		keyStatus[1024];	// Hold key status.
bool		touched =false;


int**		grid;


// MAIN GRAPHICS OBJECT
Graphics	myGraphics;		// Runing all the graphics in this object

Sphere		Player;
Physics		PlayerPhysics;
bool		PlayerInit = false;

Arrow		arrowX;
Arrow		arrowY;
Arrow		arrowZ;

Cube		cubeGrids[(ROW * COL)];
string		playerRoute = "";


// Some global variable to do the animation.
float t = 0.001f;			// Global variable for animation
float rot = 0.1f;			// Global variable for cube animation

int c_i = 0;				// Var for route iterator
float sum_dt = 0.0f;		// Calc total time passed for route


int main()
{
	int errorGraphics = myGraphics.Init();		// Launch window and graphics context
	if (errorGraphics) return 0;				// Close if something went wrong...

	while (playerRoute == "") {
		grid = createGrid(ROW, COL);
		playerRoute = runAstar(grid);
	}


	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).



	// MAIN LOOP run until the window is closed
	while (!quit){

		// Update the camera tranform based on interactive inputs or by following a predifined path.
		updateCamera();

		// Update position, orientations and any other relevant visual state of any dynamic elements in the scene.
		updateSceneElements();

		// Render a still frame into an off-screen frame buffer known as the backbuffer.
		renderScene();

		// Swap the back buffer with the front buffer, making the most recently rendered image visible on-screen.
		glfwSwapBuffers(myGraphics.window);		// swap buffers (avoid flickering and tearing)
	
	} 


	myGraphics.endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void startup() {
	// Keep track of the running time
	GLfloat currentTime = (GLfloat)glfwGetTime();	// retrieve timelapse
	deltaTime = currentTime;						// start delta time
	lastTime = currentTime;							// Save for next frame calculations.

	// Callback graphics and key update functions - declared in main to avoid scoping complexity.
	// More information here : https://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);			// Set callback for resize
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);					// Set Callback for keys
	glfwSetMouseButtonCallback(myGraphics.window, onMouseButtonCallback);	// Set callback for mouse click
	glfwSetCursorPosCallback(myGraphics.window, onMouseMoveCallback);		// Set callback for mouse move
	glfwSetScrollCallback(myGraphics.window, onMouseWheelCallback);			// Set callback for mouse wheel.

	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float)myGraphics.windowWidth / (float)myGraphics.windowHeight;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	

	for (int i = 0; i <ROW*COL; i++) {
		cubeGrids[i].Load();
	}

	// fillout the grid matrix with obstacles
	//grid[1][1] = 1;
	//grid[0][2] = 1;

	// Colours of blocks
	glm::vec4 start = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 end = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 empty = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 route = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 obstacle = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	// Set colours for cubes based
	// on generated grid
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {

			if (grid[i][j] == 0)		// Empty blocks
				cubeGrids[(i*COL) + j].fillColor = empty;

			else if (grid[i][j] == 1)	// Obstacle blocks
				cubeGrids[(i*COL) + j].fillColor = obstacle;

			else if (grid[i][j] == 2)	// Start block
				cubeGrids[(i*COL) + j].fillColor = start;

			else if (grid[i][j] == 3)	// Route that player will take
				cubeGrids[(i*COL) + j].fillColor = route;

			else						// Finish block
				cubeGrids[(i*COL) + j].fillColor = end;
			

		}
	}
	
	Player.Load();
	Player.fillColor = glm::vec4(0.7f, 0.2f, 0.5f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 


	arrowX.Load(); arrowY.Load(); arrowZ.Load();
	arrowX.fillColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); arrowX.lineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	arrowY.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); arrowY.lineColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	arrowZ.fillColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); arrowZ.lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	

	
	

	// Optimised Graphics
	myGraphics.SetOptimisations();		// Cull and depth testing
	
}

void updateCamera() {

	// calculate movement for FPS camera
	GLfloat xoffset = myGraphics.mouseX - myGraphics.cameraLastX;	
	GLfloat yoffset = myGraphics.cameraLastY - myGraphics.mouseY;	// Reversed mouse movement
	myGraphics.cameraLastX = (GLfloat)myGraphics.mouseX; 
	myGraphics.cameraLastY = (GLfloat)myGraphics.mouseY;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity; 
	yoffset *= sensitivity;

	myGraphics.cameraYaw += xoffset; 
	myGraphics.cameraPitch += yoffset;

	// check for pitch out of bounds otherwise screen gets flipped
	if (myGraphics.cameraPitch > 89.0f) myGraphics.cameraPitch = 89.0f;
	if (myGraphics.cameraPitch < -89.0f) myGraphics.cameraPitch = -89.0f;

	// Calculating FPS camera movement (See 'Additional Reading: Yaw and Pitch to Vector Calculations' in VISION) 
	glm::vec3 front;
	front.x = cos(glm::radians(myGraphics.cameraYaw)) * cos(glm::radians(myGraphics.cameraPitch));
	front.y = sin(glm::radians(myGraphics.cameraPitch));
	front.z = sin(glm::radians(myGraphics.cameraYaw)) * cos(glm::radians(myGraphics.cameraPitch));

	myGraphics.cameraFront = glm::normalize(front);

	// Update movement using the keys
	GLfloat cameraSpeed = 2.0f * deltaTime;
	if (keyStatus[GLFW_KEY_W]) myGraphics.cameraPosition += cameraSpeed * myGraphics.cameraFront;
	if (keyStatus[GLFW_KEY_S]) myGraphics.cameraPosition -= cameraSpeed * myGraphics.cameraFront;
	if (keyStatus[GLFW_KEY_A]) myGraphics.cameraPosition -= glm::normalize(glm::cross(myGraphics.cameraFront, myGraphics.cameraUp)) * cameraSpeed;
	if (keyStatus[GLFW_KEY_D]) myGraphics.cameraPosition += glm::normalize(glm::cross(myGraphics.cameraFront, myGraphics.cameraUp)) * cameraSpeed;

	// IMPORTANT PART
	// Calculate my view matrix using the lookAt helper function
	myGraphics.viewMatrix = glm::lookAt(myGraphics.cameraPosition,		// eye
		myGraphics.cameraPosition + myGraphics.cameraFront,				// centre
		myGraphics.cameraUp);											// up

}

glm::vec3 axisDirection(char dir) {
	switch (dir) {
	case '0':
		return glm::vec3(1.0f, 0.0f, 0.0f);
	case '1':
		return glm::vec3(0.0f, 0.0f, 1.0f);
	case '2':
		return glm::vec3(-1.0f, 0.0f, 0.0f);
	case '3':
		return glm::vec3(0.0f, 0.0f, -1.0f);
	default:
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

glm::vec3 rotDirection(char dir) {
	switch (dir) {
	case '0':
		return glm::vec3(0.0f, 0.0f, -1.0f);
	case '1':
		return glm::vec3(1.0f, 0.0f, 0.0f);
	case '2':
		return glm::vec3(0.0f, 0.0f, 1.0f);
	case '3':
		return glm::vec3(-1.0f, 0.0f, 0.0f);
	default:
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}


void updateSceneElements() {

	glfwPollEvents();								// poll callbacks

	// Calculate frame time/period -- used for all (physics, animation, logic, etc).
	GLfloat currentTime = (GLfloat)glfwGetTime();	// retrieve timelapse
	deltaTime = currentTime - lastTime;				// Calculate delta time
	lastTime = currentTime;							// Save for next frame calculations.

	// Do not forget your ( T * R * S ) http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/



	if (PlayerInit && c_i < playerRoute.length()) {
		sum_dt += deltaTime;

		glm::vec3 change = axisDirection(playerRoute[c_i]);
		if (sum_dt <= 1.0f){
			// e.g. 1.0 * 0.1 = 0.1
			PlayerPhysics.position += change * deltaTime;
		} else {
			float diff_dt = sum_dt - 1.0f;
			// e.g. 1.0 * (0.1 - 0.02)
			PlayerPhysics.position += change * (deltaTime - diff_dt);

			c_i++;
			change = axisDirection(playerRoute[c_i]);

			// e.g. -1.0 * (0.02)
			PlayerPhysics.position += change * diff_dt;

			sum_dt = 0.0f;
		}
	} else {
		PlayerInit = false;
		c_i = 0;
		sum_dt = 0.0f;
	}
	

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			float x_axis = (float) i;
			float y_axis = 0.0f;
			float z_axis = (float) j;

			// if block is obstacle then increase height
			if (grid[i][j] == 1)
				y_axis = 0.25f;

			if (!PlayerInit && grid[i][j] == 2) {
				// Set initial position for players start
				PlayerInit = true;
				PlayerPhysics.position = glm::vec3(x_axis, 1.0f, z_axis);
			}
			
			glm::mat4 mv_matrix_block =
				glm::translate(glm::vec3(x_axis, y_axis, z_axis)) *
				glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)) *
				glm::mat4(1.0f);

				cubeGrids[(i*COL)+j].mv_matrix = myGraphics.viewMatrix * mv_matrix_block;
				cubeGrids[(i*COL)+j].proj_matrix = myGraphics.proj_matrix;
		}
	}


	glm::mat4 mv_matrix_player =
		glm::translate(PlayerPhysics.position) *
		glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)) *
		glm::rotate(t, rotDirection(playerRoute[c_i])) *
		glm::mat4(1.0f);

	Player.mv_matrix = myGraphics.viewMatrix * mv_matrix_player;
	Player.proj_matrix = myGraphics.proj_matrix;


	//Calculate Arrows translations (note: arrow model points up)
	glm::mat4 mv_matrix_x =
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::vec3(0.2f, 0.5f, 0.2f)) *
		glm::mat4(1.0f);
	arrowX.mv_matrix = myGraphics.viewMatrix * mv_matrix_x;
	arrowX.proj_matrix = myGraphics.proj_matrix;

	glm::mat4 mv_matrix_y =
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::scale(glm::vec3(0.2f, 0.5f, 0.2f)) *
		glm::mat4(1.0f);
	arrowY.mv_matrix = myGraphics.viewMatrix * mv_matrix_y;
	arrowY.proj_matrix = myGraphics.proj_matrix;

	glm::mat4 mv_matrix_z =
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::vec3(0.2f, 0.5f, 0.2f)) *
		glm::mat4(1.0f);
	arrowZ.mv_matrix = myGraphics.viewMatrix * mv_matrix_z;
	arrowZ.proj_matrix = myGraphics.proj_matrix;


	t += 0.1;

	if (glfwWindowShouldClose(myGraphics.window) == GL_TRUE) quit = true; // If quit by pressing x on window.

}


void renderScene() {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();

	// Draw objects in screen

	Player.Draw();

	for (int i = 0; i < ROW*COL; i++) {
		cubeGrids[i].Draw();
	}

	arrowX.Draw();
	arrowY.Draw();
	arrowZ.Draw();


}


// CallBack functions low level functionality.
void onResizeCallback(GLFWwindow* window, int w, int h) {	// call everytime the window is resized
	myGraphics.windowWidth = w;
	myGraphics.windowHeight = h;

	myGraphics.aspect = (float)w / (float)h;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	// toggle showing mouse.
	if (keyStatus[GLFW_KEY_M]) myGraphics.ToggleMouse();

	// If exit key pressed.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	int mouseX = static_cast<int>(x);
	int mouseY = static_cast<int>(y);

	myGraphics.mouseX = mouseX;
	myGraphics.mouseY = mouseY;

	// helper variables for FPS camera
	if (myGraphics.cameraFirstMouse) {
		myGraphics.cameraLastX = (GLfloat)myGraphics.mouseX; myGraphics.cameraLastY = (GLfloat)myGraphics.mouseY; myGraphics.cameraFirstMouse = false;
	}
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}
