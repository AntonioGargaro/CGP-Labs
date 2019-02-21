#pragma once
#include "Physics.h"

glm::vec3 calcIntersectiondepth(Physics one, Physics two);

void calcDirection(Physics &obj, glm::vec3 interDepth);

GLboolean checkCollision(Physics &one, Physics &two);