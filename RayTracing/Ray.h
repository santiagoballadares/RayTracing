#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * Ray class
 */
class Ray {
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(
		const glm::vec3 &origin,
		const glm::vec3 &direction
	) :
		origin(origin),
		direction(direction)
	{}

	//Returns the position of the ray at time t
	glm::vec3 operator() (const float &t) const {
		return origin + direction*t;
	}
};