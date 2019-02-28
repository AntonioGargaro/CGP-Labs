#include "Collision.h"

Physics *cubePhysics;

void setCubePhysics(Physics &cube) {
	cubePhysics = &cube;
}

glm::vec3 calcIntersectiondepth(Physics *one, Physics *two) {
	float x_inter = (one->position.x + one->size.x) - (two->position.x + two->size.x);
	float y_inter = (one->size.y * 0.5) - (one->position.y - (two->position.y + (two->size.y * 0.5)));
	float z_inter = (one->position.z + one->size.z) - (two->position.z + two->size.z);
	return glm::vec3(x_inter, y_inter, z_inter);
}

glm::vec3 calcDirection(glm::vec3 cur_vel) {
	glm::vec3 compass[] = {
		glm::vec3(0.0f, 1.0f, 0.0f),	// UP
		glm::vec3(0.0f, -1.0f, 0.0f),	// DOWN
		glm::vec3(1.0f, 0.0f, 0.0f),	// RIGHT
		glm::vec3(-1.0f, 0.0f, 0.0f),	// LEFT
		glm::vec3(0.0f, 0.0f, 1.0f),	// FORWARD
		glm::vec3(0.0f, 0.0f, -1.0f),	// BACKWARD
	};

	for (int i = 0; i < 6; i++) {
		float dot_product = glm::dot(glm::normalize(cur_vel), compass[i]);

	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 calcIntersectiondepth(Physics one, Physics two) {
	/* This function returns values releative to Physics 'one'.
	The underscored inter floats (e.g. _x_inter) check how much
	Physics 'two' intersects Physics 'one'. This is useful as
	Physics 'one' can intersect Physics 'two' from any position.
	This is why when Physics 'two's intersection of Physics 'one' is the
	minimum intersection depth, the underscored inter float is inversed
	(e.g. -_inter_x). This makes the depth value adjustment relative to
	the direction Physic 'one' is intersecting the Physics 'two' object at.
	 */

	float x_inter = (one.size.x * 0.5) - (one.position.x - (two.position.x + (two.size.x * 0.5)));
	float y_inter = (one.size.y * 0.5) - (one.position.y - (two.position.y + (two.size.y * 0.5)));
	float z_inter = (one.size.z * 0.5) - (one.position.z - (two.position.z + (two.size.z * 0.5)));

	float _x_inter = (two.size.x * 0.5) - (two.position.x - (one.position.x + (one.size.x * 0.5)));
	float _y_inter = (two.size.y * 0.5) - (two.position.y - (one.position.y + (one.size.y * 0.5)));
	float _z_inter = (two.size.z * 0.5) - (two.position.z - (one.position.z + (one.size.z * 0.5)));

	// Define vector to find minimum values
	glm::vec3 min_inter = glm::vec3(0.0f, 0.0f, 0.0f);
	float smallest = FLT_MAX;

	// Check amount 'one' intersects 'two'
	if (x_inter < y_inter && x_inter < z_inter) {
		if (x_inter < smallest)
			smallest = x_inter;
		min_inter = glm::vec3(x_inter, 0.0f, 0.0f);
	}

	if (y_inter < x_inter && y_inter < z_inter) {
		if (y_inter < smallest)
			smallest = y_inter;
		min_inter = glm::vec3(0.0f, y_inter, 0.0f);
	}

	if (z_inter < y_inter && z_inter < x_inter) {
		if (z_inter < smallest)
			smallest = z_inter;
		min_inter = glm::vec3(0.0f, 0.0f, z_inter);
	}

	// Check amount 'two' intersects 'one' and if it's
	// a smaller value than already found smallest
	if (_x_inter < _y_inter && _x_inter < _z_inter
		&& _x_inter < smallest) {
		min_inter = glm::vec3(-_x_inter, 0.0f, 0.0f);
	}

	if (_y_inter < _x_inter && _y_inter < _z_inter
		&& _y_inter < smallest) {
		min_inter = glm::vec3(0.0f, -_y_inter, 0.0f);
	}

	if (_z_inter < _x_inter && _z_inter < _y_inter
		&& _z_inter < smallest) {
		min_inter = glm::vec3(0.0f, 0.0f, -_z_inter);
	}

	return min_inter;
}

void calcDirection(Physics &obj, glm::vec3 interDepth) {
	if (interDepth.x != 0.0f)
		obj.velocity.x *= -1.0f;

	if (interDepth.y != 0.0f)
		obj.velocity.y *= -1.0f;

	if (interDepth.z != 0.0f)
		obj.velocity.z *= -1.0f;
}

void checkCollision(Physics &one, std::vector<Physics*> allPhysics) {
	// Loops through all physics items and checks for collisions
	for (auto item : allPhysics) {
		if (item == &one)
			continue;
		bool cX = one.position.x + (one.size.x * 0.5) + (item->size.x * 0.5) >= item->position.x &&
			item->position.x + (item->size.x * 0.5) + (one.size.x * 0.5) >= one.position.x;

		bool cY = one.position.y + (one.size.y * 0.5) + (item->size.y * 0.5) >= item->position.y &&
			item->position.y + (item->size.y * 0.5) + (one.size.y * 0.5) >= one.position.y;

		bool cZ = one.position.z + (one.size.z * 0.5) + (item->size.z * 0.5) >= item->position.z &&
			item->position.z + (item->size.z * 0.5) + (one.size.z * 0.5) >= one.position.z;

		if (cX && cY && cZ) {
			handleCollision(one, *item);
			// start explosion on first contact

			if (&one == cubePhysics) {
				if (!one.touchGround) {
					one.touchGround = true;
					one.explosion->start(one.position);
				}
			}
		}

	}
}

void handleCollision(Physics &one, Physics &two) {
	glm::vec3 interDepth = calcIntersectiondepth(one, two);
	one.position += interDepth;

	// Reduce velocity because of absorption
	one.velocity *= glm::vec3(0.75f, 0.75f, 0.75f);
	// Check bounce direction
	calcDirection(one, interDepth);
}
