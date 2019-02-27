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

// MAIN GRAPHICS OBJECT
Graphics	myGraphics;		// Runing all the graphics in this object

// DEMO OBJECTS PHYSICS
Physics		spherePhysics;
Physics		sphere2Physics;
Physics		sphere3Physics;
Physics		sphere4Physics;

Physics		wallBox1Physics(glm::vec3(2.0f, 2.0f, 2.0f));	// Define size
Physics		wallBox2Physics(glm::vec3(2.0f, 9.0f, 2.0f));	// Define size
Physics		wallBox3Physics(glm::vec3(2.0f, 2.0f, 2.0f));	// Define size

Physics		floorPhysics(glm::vec3(1000.0f, 0.001f, 1000.0f));

std::vector<Physics*> allPhysics = { 
	&spherePhysics, &sphere2Physics, 
	&sphere3Physics, &sphere4Physics,
	&wallBox1Physics, &wallBox2Physics, 
	&wallBox3Physics, &floorPhysics
};


// DEMO OBJECTS
Sphere		mySphere;
Sphere		mySphere2;
Sphere		mySphere3;
Sphere		mySphere4;

Arrow		arrowX;
Arrow		arrowY;
Arrow		arrowZ;
Cube		wallCube;
Cube		myFloor;
Cube		wallBox1;
Cube		wallBox2;
Cube		wallBox3;



// Some global variable to do the animation.
float t = 0.001f;			// Global variable for animation
float rot = 0.1f;			// Global variable for cube animation


int main()
{
	int errorGraphics = myGraphics.Init();		// Launch window and graphics context
	if (errorGraphics) return 0;				// Close if something went wrong...

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


	mySphere.Load();
	mySphere.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 

	mySphere2.Load();
	mySphere2.fillColor = glm::vec4(0.5f, 1.0f, 0.2f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 

	mySphere3.Load();
	mySphere2.fillColor = glm::vec4(0.1f, 0.8f, 0.12f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 

	mySphere4.Load();
	mySphere4.fillColor = glm::vec4(0.7f, 0.2f, 0.5f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 


	arrowX.Load(); arrowY.Load(); arrowZ.Load();
	arrowX.fillColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); arrowX.lineColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	arrowY.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); arrowY.lineColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	arrowZ.fillColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); arrowZ.lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	
	//the first wall box
	wallBox1.Load();
	wallBox1.fillColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); wallBox1.lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	wallBox2.Load();
	wallBox2.fillColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); wallBox2.lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	wallBox3.Load();
	wallBox3.fillColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); wallBox3.lineColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);


	myFloor.Load();
	
	myFloor.fillColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);	// Sand Colour
	myFloor.lineColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);	// Sand again


	// Define sphere physics
	spherePhysics.position = glm::vec3(-8.0f, 12.5f, 12.0f);
	spherePhysics.velocity = glm::vec3(8.0f, 0.0f, 0.0f);

	// Define sphere2 physics
	sphere2Physics.position = glm::vec3(12.0f, 12.5f, 12.0f);
	sphere2Physics.velocity = glm::vec3(-8.0f, 0.0f, 0.0f);

	// Define sphere2 physics
	sphere3Physics.position = glm::vec3(2.0f, 12.5f, 6.0f);
	sphere3Physics.velocity = glm::vec3(0.0f, 0.0f, 8.0f);

	// Define sphere2 physics
	sphere4Physics.position = glm::vec3(2.0f, 12.5f, 18.0f);
	sphere4Physics.velocity = glm::vec3(0.0f, 0.0f, -8.0f);
	
	// Define wallbox 1 attributes
	wallBox1Physics.position = glm::vec3(-2.0f, 1.0f, 12.0f);
	// Define wallbox 2 attributes
	wallBox2Physics.position = glm::vec3(2.0f, 4.5f, 12.0f);
	// Define wallbox 2 attributes
	wallBox3Physics.position = glm::vec3(2.0f, 4.5f, 19.0f);
	

	// Define floor attributes
	floorPhysics.position = glm::vec3(0.0f, 0.0f, 0.0f);

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
	GLfloat cameraSpeed = 3.0f * deltaTime;
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


void updateSceneElements() {

	glfwPollEvents();								// poll callbacks

	// Calculate frame time/period -- used for all (physics, animation, logic, etc).
	GLfloat currentTime = (GLfloat)glfwGetTime();	// retrieve timelapse
	deltaTime = currentTime - lastTime;				// Calculate delta time
	lastTime = currentTime;							// Save for next frame calculations.

	// Do not forget your ( T * R * S ) http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/


	// Update values of: 
	updateVelocity(spherePhysics, deltaTime);
	updatePosition(spherePhysics, deltaTime);
	// Calculate all possible collisions for sphere1
	checkCollision(spherePhysics, allPhysics);

	// calculate Sphere movement
	glm::mat4 mv_matrix_sphere =
		glm::translate(spherePhysics.position) *
		glm::rotate(-t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(spherePhysics.size) *
		glm::rotate(-t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	mySphere.mv_matrix = myGraphics.viewMatrix * mv_matrix_sphere;
	mySphere.proj_matrix = myGraphics.proj_matrix;


	// Update values of: 
	updateVelocity(sphere2Physics, deltaTime);
	updatePosition(sphere2Physics, deltaTime);
	// Calculate all possible collisions for sphere2
	checkCollision(sphere2Physics, allPhysics);
	
	// calculate Sphere2 movement
	glm::mat4 mv_matrix_sphere2 =
		glm::translate(sphere2Physics.position) *
		glm::rotate(-t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(sphere2Physics.size) *
		glm::rotate(-t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	mySphere2.mv_matrix = myGraphics.viewMatrix * mv_matrix_sphere2;
	mySphere2.proj_matrix = myGraphics.proj_matrix;



	// Update values of: 
	updateVelocity(sphere3Physics, deltaTime);
	updatePosition(sphere3Physics, deltaTime);
	// Calculate all possible collisions for sphere3
	checkCollision(sphere3Physics, allPhysics);
	
	// calculate Sphere2 movement
	glm::mat4 mv_matrix_sphere3 =
		glm::translate(sphere3Physics.position) *
		glm::rotate(-t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(sphere3Physics.size) *
		glm::rotate(-t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	mySphere3.mv_matrix = myGraphics.viewMatrix * mv_matrix_sphere3;
	mySphere3.proj_matrix = myGraphics.proj_matrix;



	// Update values of: 
	updateVelocity(sphere4Physics, deltaTime);
	updatePosition(sphere4Physics, deltaTime);
	// Calculate all possible collisions for sphere4
	checkCollision(sphere4Physics, allPhysics);


	// calculate Sphere2 movement
	glm::mat4 mv_matrix_sphere4 =
		glm::translate(sphere4Physics.position) *
		glm::rotate(-t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(sphere4Physics.size) *
		glm::rotate(-t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	mySphere4.mv_matrix = myGraphics.viewMatrix * mv_matrix_sphere4;
	mySphere4.proj_matrix = myGraphics.proj_matrix;



	// calculate Sphere movement
	glm::mat4 mv_matrix_wallBox1 =
		glm::translate(wallBox1Physics.position) *
		glm::scale(wallBox1Physics.size) *
		glm::mat4(1.0f);
	wallBox1.mv_matrix = myGraphics.viewMatrix * mv_matrix_wallBox1;
	wallBox1.proj_matrix = myGraphics.proj_matrix;


	glm::mat4 mv_matrix_wallBox2 =
		glm::translate(wallBox2Physics.position) *
		glm::scale(wallBox2Physics.size) *
		glm::mat4(1.0f);
	wallBox2.mv_matrix = myGraphics.viewMatrix * mv_matrix_wallBox2;
	wallBox2.proj_matrix = myGraphics.proj_matrix;


	glm::mat4 mv_matrix_wallBox3 =
		glm::translate(wallBox3Physics.position) *
		glm::scale(wallBox3Physics.size) *
		glm::mat4(1.0f);
	wallBox3.mv_matrix = myGraphics.viewMatrix * mv_matrix_wallBox3;
	wallBox3.proj_matrix = myGraphics.proj_matrix;


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
		//glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *	// already model pointing up
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



	// Calculate floor position and resize
	myFloor.mv_matrix = myGraphics.viewMatrix *
		glm::translate(floorPhysics.position) *
		glm::scale(floorPhysics.size) *
		glm::mat4(1.0f);
	myFloor.proj_matrix = myGraphics.proj_matrix;
	

	if (glfwWindowShouldClose(myGraphics.window) == GL_TRUE) quit = true; // If quit by pressing x on window.

}


void renderScene() {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();

	// Draw objects in screen
	myFloor.Draw();
	mySphere.Draw();
	mySphere2.Draw();
	mySphere3.Draw();
	mySphere4.Draw();

	wallBox1.Draw();
	wallBox2.Draw();
	wallBox3.Draw();

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
