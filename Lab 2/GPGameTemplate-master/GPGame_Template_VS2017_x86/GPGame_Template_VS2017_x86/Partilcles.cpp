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
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat maxLife = 5.0f;
	GLfloat timeAlive = 0.0f;
	std::chrono::steady_clock::time_point timeBorn;

};


class ParticleEmitter {
private:
	Particle particlesArr[MaxParticles];

	glm::vec3 pos;

	// Acceleration for the
	glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	// Use chrono for decimal time points
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

public:
	// Class Constructor
	ParticleEmitter() {}

	Cube *bombPntr;

	void start(glm::vec3 position) {
		pos = position;

		cout << "started";
		GLfloat change = 1.0f;

		// Grab current time
		auto born = Time::now();


		//this generates a random number for the x axis
		const float range_from_x = -5.0;
		const float range_to_x = 5.0;
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_real_distribution<float>  distr_x(range_from_x, range_to_x);

		//this generates a random number for the z axis
		const float range_from_z = 1.0;
		const float range_to_z = 10.0;
		std::uniform_real_distribution<float>  distr_z(range_from_z, range_to_z);

		//this generates a random number for the y axis
		const float range_from_y = 5.0;
		const float range_to_y = 15.0;
		std::uniform_real_distribution<float>  distr_y(range_from_y, range_to_y);
	


		for (int i = 0; i < MaxParticles; i++) {
			float x_vel = (float)distr_x(generator);
			float y_vel = (float)distr_y(generator);
			float z_vel = (float) distr_z(generator);

			particlesArr[i].position = glm::vec3(pos);

			particlesArr[i].velocity.x = x_vel;
			particlesArr[i].velocity.y = y_vel;
			particlesArr[i].velocity.z = z_vel;

			change += 0.01f;

			// Define particle is born
			particlesArr[i].timeBorn = born;

			// Find time particle has been alive
			particlesArr[i].timeAlive = calcTimeSinceStart(particlesArr[i].timeBorn);

		}
	}

	void update() {

		for (int i = 0; i < MaxParticles; i++) {
			// Store last update
			float lastUpdated = particlesArr[i].timeAlive;


			// Find time particle has been alive
			particlesArr[i].timeAlive = calcTimeSinceStart(particlesArr[i].timeBorn);

			particlesArr[i].position = calcPosition(particlesArr[i].position, particlesArr[i].velocity, particlesArr[i].timeAlive - lastUpdated);

			particlesArr[i].velocity = calcVelocity(particlesArr[i].velocity, particlesArr[i].timeAlive - lastUpdated);



			// Calculate Cube position 
			glm::mat4 mv_matrix_cube =
				glm::translate(particlesArr[i].position) *
				glm::mat4(1.0f);
			mv_matrix_cube *= glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
				
			
			bombPntr[i].mv_matrix = myGraphics.viewMatrix * mv_matrix_cube;
			bombPntr[i].proj_matrix = myGraphics.proj_matrix;
		}
	}

	float calcTimeSinceStart(std::chrono::steady_clock::time_point t0) {
		// calc time since start
		auto t1 = Time::now();	// grab current time
		fsec fs = t1 - t0;		// find difference


		// output difference in seconds
		return fs.count();

	}


	glm::vec3 calcVelocity(glm::vec3 cur_vel, float timeAlive) {
		// Update value in cur_vel to new value
		return (cur_vel + (gravity * timeAlive));
	}


	glm::vec3 calcPosition(glm::vec3 cur_pos, glm::vec3 cur_vel, float timeAlive) {

		// Update value in cur_pos to new value
		glm::vec3 new_pos = cur_pos + (cur_vel * timeAlive);

		

		// As it's gravity, remove the travelled distance from current
		//glm::vec3 position = new_pos;

		

		return new_pos;
	}

};