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

	allParticlePhysics = {};
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


		allParticlePhysics.insert(allParticlePhysics.end(), prts[i].physicsAttr);

		// Init starting position of particle
		prts[i].physicsAttr->position = pos;

		prts[i].physicsAttr->position.y += 1.0f;

		// Define particle is born
		prts[i].timeBorn = born;

		// Find time particle has been alive
		prts[i].timeAlive = compareTime(prts[i].timeBorn);

		prts[i].maxLife = getRand(2.0f, 7.0f);
	}
}


void ParticleEmitter::updatePos(glm::vec3 _pos) {
	pos = _pos;
}


void ParticleEmitter::update(float deltatime) {
	for (int i = 0; i < MaxParticles; i++) {
		// Store last update
		float lastUpdated = prts[i].timeAlive;
					   
		// Find time particle has been alive
		prts[i].timeAlive = compareTime(prts[i].timeBorn);

		float transp = prts[i].timeAlive / prts[i].maxLife;

		bombPntr[i].fillColor = glm::vec4(1.0f, 0.4f, 1.0f, 1.0f - transp);
		bombPntr[i].lineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f - transp);

		// Calculate delta time from last update
		float dt = prts[i].timeAlive - lastUpdated;

		// Update velocity
		updateVelocity(*prts[i].physicsAttr, dt);
		// Update position
		updatePosition(*prts[i].physicsAttr, dt);

		if (prts[i].timeAlive > prts[i].maxLife) {
			// Generate random x, y, z velocities
			float x_vel = x_getRand();
			float y_vel = y_getRand();
			float z_vel = z_getRand();

			// Set velocity values
			allParticlePhysics[i]->velocity = glm::vec3(x_vel, y_vel, z_vel);

			// Init starting position of particle
			allParticlePhysics[i]->position = glm::vec3(pos);
			allParticlePhysics[i]->position.y += 0.6f;

			// Define particle is born
			prts[i].timeBorn = Time::now();

			// Find time particle has been alive
			prts[i].timeAlive = compareTime(prts[i].timeBorn);

			prts[i].maxLife = getRand(2.0f, 7.0f);
			bombPntr[i].fillColor = glm::vec4(1.0f, 0.4f, 1.0f, 1.0f);
			bombPntr[i].lineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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

