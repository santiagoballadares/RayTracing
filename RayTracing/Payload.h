#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * Payload class
 *
 * It stands for the light particle shot from a certain point.
 */
class Payload {
public:
	glm::vec3 color;	//Each time, intersecting with something will change the color of this Payload.
	int numBounces;		//To make the calculation not so expensive, Ray hits more times than a certain number of bounces will not be taken into consideration.

	Payload() : 
		color(0.0f),
		numBounces(0)
	{}
};