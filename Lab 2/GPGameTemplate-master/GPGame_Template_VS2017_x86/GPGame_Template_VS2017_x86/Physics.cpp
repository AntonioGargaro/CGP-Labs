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

// Acceleration for the
const glm::vec3 acceleration(0.0f, 0.0981f, 0.0f);

// Use chrono for decimal time points
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;


class Physics {
	public:
		// Required in class to prevent already defined error
		std::chrono::steady_clock::time_point t0 = Time::now();

		// define inits
		glm::vec3 position;
		glm::vec3 velocity;
		float mass = 2.0f;	// Default mass of 2 units

	void calcPosition(glm::vec3 cur_pos) {
		// calc time since start
		auto t1 = Time::now();	// grab current time
		fsec fs = t1 - t0;		// find difference

		// output difference in seconds
		std::cout << fs.count() << "s\n";

		// distance travelled = (acceleration x t^2) / 2
		glm::vec3 newDistance = ((acceleration * (fs.count() * fs.count())) * 0.5f);

		// As it's gravity, remove the travelled distance from current
		position = cur_pos - newDistance;

		// If touching ground, reset
		if (position.y < 0.5f) {
			t0 = Time::now();	// reset first time
			position.y = 10.5f;	// put to default height
		}
	}




};