#pragma once

#include <iostream>
#include <vector>
using namespace std;

// Helper graphic libraries
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "graphics.h"
#include "shapes.h"



class Physics {
public:
	// Default constructor
	Physics();
	// Init size
	Physics(glm::vec3 size);
	
	// Has touched ground 
	bool touchGround;

	// Define attributes
	glm::vec3 position;
	glm::vec3 size;
	glm::vec3 velocity;
	float mass;
};

// Update velocity based on current vel and time passed
void updateVelocity(Physics &obj, float deltaTime);

// Update position based on current pos, cur vel and time passed
void updatePosition(Physics &obj, float deltaTime);