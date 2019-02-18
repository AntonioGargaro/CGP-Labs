#include "Collision.h"

int checkSide(Physics one, Physics two) {
	if (two.position.y + (two.size.y * 0.5) + one.size.y <= (one.position.y + (one.size.y * 0.5))) {
		cout << "on top";
		return 1;
	}
	
	if (two.position.x + (two.size.x * 0.5) + one.size.x <= (one.position.x + (one.size.x * 0.5))) {
		cout << "on top";
		return 4;
	}

	return 0;
}

glm::vec3 calcIntersectiondepth(Physics one, Physics two) {
	float x_inter = (one.size.x * 0.5) - (one.position.x - (two.position.x + (two.size.x * 0.5)));
	float y_inter = (one.size.y * 0.5) - (one.position.y - (two.position.y + (two.size.y * 0.5)));
	float z_inter = (one.size.z * 0.5) - (one.position.z - (two.position.z + (two.size.z * 0.5)));

	one.position.y = one.position.y + y_inter;
	if (checkSide(one, two) == 1) {
		return glm::vec3(0.0f, y_inter, 0.0f);
	}

	one.position.x = one.position.x + x_inter;
	if (checkSide(one, two) == 4) {
		return glm::vec3(x_inter, 0.0f, 0.0f);
	}

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

GLboolean checkCollision(Physics &one, Physics &two) {
	bool cX = one.position.x + (one.size.x * 0.5) + (two.size.x * 0.5) >= two.position.x &&
		two.position.x + (two.size.x * 0.5) + (one.size.x * 0.5) >= one.position.x;

	bool cY = one.position.y + (one.size.y * 0.5) + (two.size.y * 0.5) >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;

	bool cZ = one.position.z + (one.size.z * 0.5) + (two.size.z * 0.5) >= two.position.z &&
		two.position.z + (two.size.z * 0.5) + (one.size.z * 0.5) >= one.position.z;

	return cX && cY && cZ;
}

