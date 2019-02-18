#pragma once
#include "Physics.h"

int checkSide(Physics one, Physics two);

glm::vec3 calcIntersectiondepth(Physics one, Physics two);

glm::vec3 calcDirection(glm::vec3 cur_vel);

GLboolean checkCollision(Physics &one, Physics &two);