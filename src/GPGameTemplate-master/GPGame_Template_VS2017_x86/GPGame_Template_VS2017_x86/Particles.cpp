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

ParticleEmitter::ParticleEmitter(Cube *_cubes)
	: generator(std::random_device()()) // seed the generator
{
	// Acceleration for the
	gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	x_from = 2.0f;
	x_to = 5.0f;

	y_from = 20.0f;
	y_to = 22.0f;

	z_from = 2.0f;
	z_to = 5.0f;
	cubes = _cubes;

	allBoidPhysics = {};
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

bool ParticleEmitter::makeLeader() {
	if (getRand(leader_from, leader_to) > 0.15)
		return false;
	return true;
}

glm::vec3 ParticleEmitter::getTargetLoc() {
	return glm::vec3(x_getRand(), y_getRand(), z_getRand());
};

glm::vec3 posDifference(Boid boid) {
	/*
	Check the difference between target location and boid's current location.

	For example, if target's x location is 1.0f, and boid's x location is
	0.0f, starting increasing boid's x velocity by a constant until max speed
	or change in target location.
	*/
	return boid.targetLocation - boid.physicsAttr->position;
}

void updateFlockVelocity(Boid &boid, glm::vec3 posDiff, float deltaTime) {
	if (boid.physicsAttr->velocity.x > boid.max_speed) 
		boid.physicsAttr->velocity.x = boid.max_speed;
	else if (posDiff.x >= 0.0f) 
		boid.physicsAttr->velocity.x += 1.5f * deltaTime;
	else 
		boid.physicsAttr->velocity.x -= 1.5f * deltaTime;


	if (boid.physicsAttr->velocity.y > boid.max_speed) 
		boid.physicsAttr->velocity.y = boid.max_speed;
	else if (posDiff.y >= 0.0f) 
		boid.physicsAttr->velocity.y += 1.5f * deltaTime;
	else 
		boid.physicsAttr->velocity.y -= 1.5f * deltaTime;
	

	if (boid.physicsAttr->velocity.z > boid.max_speed) 
		boid.physicsAttr->velocity.z = boid.max_speed;
	else if (posDiff.z >= 0.0f) 
		boid.physicsAttr->velocity.z += 1.5f * deltaTime;
	else 
		boid.physicsAttr->velocity.z -= 1.5f * deltaTime;
	
};

Boid* ParticleEmitter::closestLeaderPos(Boid* boid) {

	float smallestSum = FLT_MAX;
	Boid* closest = boid_leaders[0];

	for (auto const& leader : boid_leaders) {
		float x_sum = 
			leader->physicsAttr->position.x - boid->physicsAttr->position.x;
		float y_sum = 
			leader->physicsAttr->position.y - boid->physicsAttr->position.y;
		float z_sum = 
			leader->physicsAttr->position.z - boid->physicsAttr->position.z;

		float totalSum = x_sum + y_sum + z_sum;

		if (totalSum < 0.0f)
			totalSum *= -1.0f;

		if (totalSum < smallestSum) {
			smallestSum = totalSum;
			closest = leader;
		}
	}

	return closest;
}

void ParticleEmitter::start(glm::vec3 position) {
	pos = position;

	
	// Make first boid leader
	boid[0].isLeader = true;

	// Get initial update time
	auto firstUpdate = Time::now();

	for (int i = 0; i < MaxParticles; i++) {
		// Generate random x, y, z velocities
		float x_vel = x_getRand();
		float y_vel = y_getRand();
		float z_vel = z_getRand();
			
		boid[i].physicsAttr = new Physics;


		if (i != 0)
			boid[i].isLeader = makeLeader();

		if (boid[i].isLeader) {
			boid_leaders.push_back(&boid[i]);

			boid[i].targetLocation = getTargetLoc();
			boid[i].lastUpdate = firstUpdate;

			boid[i].fillColour = glm::vec4(getRand(0.0f, 1.0f), getRand(0.0f, 1.0f), getRand(0.0f, 1.0f), 1.0f);
			cubes[i].fillColor = boid[i].fillColour;
				cout << i << " is leader\n";
		}
		else {
			cout << i << " is not leader\n";
		}

		// Set velocity values
		boid[i].physicsAttr->velocity = glm::vec3(x_vel, 0.0f, z_vel);
		boid[i].physicsAttr->size = glm::vec3(0.1f, 0.1f, 0.1f);

		// Init starting position of particle
		boid[i].physicsAttr->position = glm::vec3(x_vel, 1.0f, z_vel);

		boid[i].physicsAttr->position.y += 1.0f;

		allBoidPhysics.insert(allBoidPhysics.end(), boid[i].physicsAttr);
	}

	for (auto lead : boid_leaders)
		cout << lead << "\n";
}


void ParticleEmitter::update(float deltaTime) {
	for (int i = 0; i < MaxParticles; i++) {

		glm::vec3 posDiff = posDifference(boid[i]);

		// Update velocity
		updateFlockVelocity(boid[i], posDiff, deltaTime);
		// Update position
		updatePosition(*boid[i].physicsAttr, deltaTime);

		// Check for boid's collision against other boids
		checkCollision(*boid[i].physicsAttr, allBoidPhysics);

		if (boid[i].isLeader) {
			if (compareTime(boid[i].lastUpdate) > 3.0f) {
				glm::vec3 nextLoc = getTargetLoc();

				boid[i].targetLocation = nextLoc;
				boid[i].lastUpdate = Time::now();

			}
		} else {
			Boid* closest = closestLeaderPos(&boid[i]);
			boid[i].targetLocation = closest->physicsAttr->position;
			cubes[i].fillColor = closest->fillColour;
		}

			
		// Calculate Cube position 
		glm::mat4 mv_matrix_cube =
			glm::translate(boid[i].physicsAttr->position) *
			glm::mat4(1.0f);
		mv_matrix_cube *= glm::scale(boid[i].physicsAttr->size);
				
		// Update cubes matrix
		cubes[i].mv_matrix = myGraphics.viewMatrix * mv_matrix_cube;
		cubes[i].proj_matrix = myGraphics.proj_matrix;
	}
}

float ParticleEmitter::compareTime(std::chrono::steady_clock::time_point t0) {
	// calc time since start
	auto t1 = Time::now();	// grab current time
	fsec fs = t1 - t0;		// find difference

	// output difference in seconds
	return fs.count();
}

