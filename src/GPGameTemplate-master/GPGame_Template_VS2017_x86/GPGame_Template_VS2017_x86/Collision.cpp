#include "Collision.h"


glm::vec3 calcIntersectiondepth(Physics one, Physics two) {
	cout << "fuck\n";
	float x_inter = (one.position.x + one.size.x) - (two.position.x + two.size.x);
	float y_inter = (one.size.y * 0.5) - (one.position.y - (two.position.y + (two.size.y * 0.5)));
	float z_inter = (one.position.z + one.size.z) - (two.position.z + two.size.z);
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
	bool cX = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;

	cout << one.position.y + one.size.y; cout << "\n";
	cout << two.position.y; cout << "\n\n";

	cout << two.position.y + two.size.y; cout << "\n";
	cout << one.position.y; cout << "\n\n";

	bool cY = one.position.y + (one.size.y * 0.5) + (two.size.y * 0.5) >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;

	bool cZ = one.position.z + one.size.z >= two.position.z &&
		two.position.z + two.size.z >= one.position.z;

	return cX && cY && cZ;
}

