#include "Physics.h"

// Define gravity on y-axis
glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);


Physics::Physics() {
	touchGround = false;					// Has touched ground
	mass = 3.0f;							// Default mass of 2 units
	size = glm::vec3(1.0f, 1.0f, 1.0f);
}


Physics::Physics(glm::vec3 new_size) {
	Physics::Physics();
	size = new_size;
}


// Update value in cur_vel to new value
void updateVelocity(Physics &obj, float deltaTime) {
	obj.velocity =  obj.velocity + (gravity * deltaTime);
}


// Update value in cur_pos to new value	
void updatePosition(Physics &obj, float deltaTime) {
	obj.position = obj.position + (obj.velocity * deltaTime);
}
