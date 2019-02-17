#include "Particles.h"


ParticleEmitter::ParticleEmitter() 
	: generator(std::random_device()()) // seed the generator
{
	// Acceleration for the
	gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	x_from = -1.0f;
	x_to = 1.0f;

	y_from = 5.0f;
	y_to = 25.0f;

	z_from = -1.0f;
	z_to = 1.0f;
}


float ParticleEmitter::getRand(float from, float to) {
	std::uniform_real_distribution<float> dis(from, to);
	return dis(generator);
}

float ParticleEmitter::x_getRand() {
	return getRand(x_from, x_to);
}

float ParticleEmitter::y_getRand() {
	return getRand(y_from, y_to);
}

float ParticleEmitter::z_getRand() {
	return getRand(z_from, z_to);
}


void ParticleEmitter::start(glm::vec3 position) {
	pos = position;

	cout << "started\n";

	// Grab current time
	auto born = Time::now();

	for (int i = 0; i < MaxParticles; i++) {
		// Generate random x, y, z velocities
		float x_vel = x_getRand();
		float y_vel = y_getRand();
		float z_vel = z_getRand();
			
		prts[i].physicsAttr = new Physics;

		// Set velocity values
		prts[i].physicsAttr->velocity = glm::vec3(x_vel, y_vel, z_vel);
		prts[i].physicsAttr->size = glm::vec3(0.1f, 0.1f, 0.1f);

		// Init starting position of particle
		prts[i].physicsAttr->position = glm::vec3(pos);

		prts[i].physicsAttr->position.y += 1.0f;

		// Define particle is born
		prts[i].timeBorn = born;

		// Find time particle has been alive
		prts[i].timeAlive = compareTime(prts[i].timeBorn);
	}
}


void ParticleEmitter::update() {
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
		
		// Update velocity
		updateVelocity(*prts[i].physicsAttr, deltaTime);
		// Update position
		updatePosition(*prts[i].physicsAttr, deltaTime);

		if (prts[i].physicsAttr->position.y < 0.0f) {
			prts[i].physicsAttr->position.y = prts[i].physicsAttr->size.y * 0.51f;
		}


		if (checkCollision(floor, prts[i].physicsAttr)) {
			prts[i].physicsAttr->velocity.y *= -0.6f;
			prts[i].physicsAttr->position.y += calcIntersectiondepth(prts[i].physicsAttr, floor).y;
		}
			
		// Calculate Cube position 
		glm::mat4 mv_matrix_cube =
			glm::translate(prts[i].physicsAttr->position) *
			glm::mat4(1.0f);
		mv_matrix_cube *= glm::scale(prts[i].physicsAttr->size);
				
		// Update cubes matrix
		bombPntr[i].mv_matrix = myGraphics.viewMatrix * mv_matrix_cube;
		bombPntr[i].proj_matrix = myGraphics.proj_matrix;
	}
}

float ParticleEmitter::compareTime(std::chrono::steady_clock::time_point t0) {
	// calc time since start
	auto t1 = Time::now();	// grab current time
	fsec fs = t1 - t0;		// find difference

	// output difference in seconds
	return fs.count();
}

