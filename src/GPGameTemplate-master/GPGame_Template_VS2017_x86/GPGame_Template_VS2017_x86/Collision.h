#pragma once
#include "Physics.h"

class Physics;

void setCubePhysics(Physics &cube);

glm::vec3 calcIntersectiondepth(Physics one, Physics two);

void calcDirection(Physics &obj, glm::vec3 interDepth);

void checkCollision(Physics &one, std::vector<Physics*> allPhysics);

void handleCollision(Physics &one, Physics &two);