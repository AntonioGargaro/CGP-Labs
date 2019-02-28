#pragma once
#include "Global.h"
#include <iostream>
#include <vector>
#include <random>
#include <time.h>       /* time */
// Helper graphic libraries
#include <GL/glew.h>
#include <chrono> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "Shapes.h"
#include "Physics.h"
#include "Collision.h"

class Physics;

struct Particle {
	Physics* physicsAttr;

	GLfloat maxLife = 5.0f;
	GLfloat timeAlive = 0.0f;
	std::chrono::steady_clock::time_point timeBorn;

};


class ParticleEmitter {
private:
	Particle prts[MaxParticles];
	
	glm::vec3 pos;

	// Acceleration for the
	glm::vec3 gravity;

	// Use chrono for decimal time points
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

	// Generator from random float
	std::mt19937							generator;

	float x_from;
	float x_to;

	float y_from;
	float y_to;

	float z_from;
	float z_to;


	float getRand(float from, float to);

	float x_getRand();

	float y_getRand();

	float z_getRand();


	


public:
	// Class Constructor
	ParticleEmitter();

	Cube *bombPntr;
	std::vector<Physics*> allParticlePhysics;

	// Set pointer to floor for collisions
	Physics* floor;

	void start(glm::vec3 position);


	void update(float deltatime);

	void updatePos(glm::vec3 _pos);

	float compareTime(std::chrono::steady_clock::time_point t0);

};