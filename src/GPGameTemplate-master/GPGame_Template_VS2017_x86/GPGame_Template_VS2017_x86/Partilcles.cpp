#include "Global.h"
#include <iostream>
#include <vector>
#include <random>
#include <time.h>       /* time */
using namespace std;

// Helper graphic libraries
#include <GL/glew.h>
#include <chrono> 

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/transform.hpp>
#include "Shapes.h"



struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;

	GLfloat maxLife = 3.0f;
	GLfloat timeAlive = 0.0f;
	std::chrono::steady_clock::time_point timeBorn;

};


class ParticleEmitter {
private:
	Particle prts[MaxParticles];

	glm::vec3 pos;

	// Acceleration for the
	glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	// Use chrono for decimal time points
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

	// Generator from random float
	std::mt19937							generator;

	const float x_from = -5.0f;
	const float x_to = 5.0f;

	const float y_from = 5.0f;
	const float y_to = 25.0f;

	const float z_from = 1.0f;
	const float z_to = 5.0f;


	float getRand(float from, float to) {
		std::uniform_real_distribution<float> dis(from, to);
		return dis(generator);
	}

	float x_getRand() {
		return getRand(x_from, x_to);
	}

	float y_getRand() {
		return getRand(y_from, y_to);
	}

	float z_getRand() {
		return getRand(z_from, z_to);
	}


public:
	// Class Constructor
	ParticleEmitter() 
		: generator(std::random_device()()) // seed the generator
	{}

	Cube *bombPntr;

	void start(glm::vec3 position) {
		pos = position;

		cout << "started";
		GLfloat change = 1.0f;

		// Grab current time
		auto born = Time::now();

		for (int i = 0; i < MaxParticles; i++) {
			// Generate random x, y, z velocities
			float x_vel = x_getRand();
			float y_vel = y_getRand();
			float z_vel = z_getRand();
			
			// Set velocity values
			prts[i].velocity = glm::vec3(x_vel, y_vel, z_vel);

			// Init starting position of particle
			prts[i].position = glm::vec3(pos);

			// Define particle is born
			prts[i].timeBorn = born;

			// Find time particle has been alive
			prts[i].timeAlive = compareTime(prts[i].timeBorn);
		}
	}


	void update() {
		for (int i = 0; i < MaxParticles; i++) {
			// Store last update
			float lastUpdated = prts[i].timeAlive;
					   
			// Find time particle has been alive
			prts[i].timeAlive = compareTime(prts[i].timeBorn);

			if (prts[i].timeAlive > prts[i].maxLife) {
				//start(pos);
			}

			// Calculate delta time from last update
			float deltaTime = prts[i].timeAlive - lastUpdated;

			// Update position
			prts[i].position = calcPosition(prts[i].position, prts[i].velocity, deltaTime);

			// Update velocity
			prts[i].velocity = calcVelocity(prts[i].velocity, deltaTime);

			//if (prts[i].position.y < 0.0f)
				//prts[i].velocity.y *= -0.98f;
			
			// Calculate Cube position 
			glm::mat4 mv_matrix_cube =
				glm::translate(prts[i].position) *
				glm::rotate(prts[i].timeAlive, glm::vec3(3.0f, 3.0f, 3.0f)) *
				glm::mat4(1.0f);
			mv_matrix_cube *= glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
				
			// Update cubes matrix
			bombPntr[i].mv_matrix = myGraphics.viewMatrix * mv_matrix_cube;
			bombPntr[i].proj_matrix = myGraphics.proj_matrix;
		}
	}

	float compareTime(std::chrono::steady_clock::time_point t0) {
		// calc time since start
		auto t1 = Time::now();	// grab current time
		fsec fs = t1 - t0;		// find difference

		// output difference in seconds
		return fs.count();
	}

	glm::vec3 calcVelocity(glm::vec3 cur_vel, float deltaTime) {
		// Update value in cur_vel to new value
		return (cur_vel + (gravity * deltaTime));
	}

	glm::vec3 calcPosition(glm::vec3 cur_pos, glm::vec3 cur_vel, float deltaTime) {
		// Update value in cur_pos to new value	
		return cur_pos + (cur_vel * deltaTime);
	}

};