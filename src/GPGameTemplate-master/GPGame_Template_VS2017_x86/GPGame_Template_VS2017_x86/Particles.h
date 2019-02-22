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

struct Boid {
	Physics* physicsAttr;
	glm::vec3 leaderPos;
	glm::vec4 leaderColour;
	bool isLeader;

	float max_speed = 2.0f;
	float max_force;

	glm::vec3 targetLocation;

	glm::vec4 fillColour;

	std::chrono::steady_clock::time_point lastUpdate;
};


class ParticleEmitter {
private:
	Boid boid[MaxParticles];
	std::vector<Boid*> boid_leaders;
	
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

	float leader_from = 0.0f;
	float leader_to = 1.0f;


	float getRand(float from, float to);

	float x_getRand();
	float y_getRand();
	float z_getRand();

	glm::vec3 getTargetLoc();

	bool makeLeader();


public:
	// Class Constructor
	ParticleEmitter();

	// Cube pointer Constructor
	ParticleEmitter(Cube *_cubes);

	Cube *cubes;

	// Set pointer to floor for collisions
	Physics* floor;

	void start(glm::vec3 position);

	void update(float dt);

	float compareTime(std::chrono::steady_clock::time_point t0);

	glm::vec3 closestLeaderPos(Boid &boid);

};