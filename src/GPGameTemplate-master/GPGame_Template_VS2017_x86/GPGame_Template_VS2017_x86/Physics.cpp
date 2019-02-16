#include <iostream>
#include <vector>
using namespace std;

// Helper graphic libraries
#include <GL/glew.h>
#include <chrono> 

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "graphics.h"
#include "shapes.h"
#include "Partilcles.cpp"



class Physics {


private:
	// Acceleration for the
	//const glm::vec3 acceleration = glm::vec3(0.0f, 0.0980f, 0.0f);

	// Use chrono for decimal time points
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

public:
	// Has touched ground
	bool touchGround = false;

	// Pointer to explosion emitter
	//ParticleEmitter explosion;

	// Required in class to prevent already defined error
	std::chrono::steady_clock::time_point t0 = Time::now();

	// define inits
	glm::vec3 position;
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 velocity;
	float mass = 3.0f;	// Default mass of 2 units

	void calcPosition(glm::vec3 cur_pos) {
		//if (touchGround) 
		//	return;
		

		// calc time since start
		auto t1 = Time::now();	// grab current time
		fsec fs = t1 - t0;		// find difference

		// output difference in seconds
		//std::cout << fs.count() << "s\n";

		// distance travelled = (acceleration x t^2) / 2
		//glm::vec3 newDistance = ((acceleration * (fs.count() * fs.count())) * 0.5f);

		// As it's gravity, remove the travelled distance from current
		position = cur_pos; // -newDistance;

	}

	

};