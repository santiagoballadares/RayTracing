#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Material.h"

/**
 * IntersectInfo class
 *
 * It holds all the information about the ray intersection with an object.
 */
class IntersectInfo {
public:
	//The position of the intersection in 3D coordinates
	glm::vec3 hitPoint;

	//The normal vector of the surface at the point of the intersection
	glm::vec3 normal;

	//The time along the ray that the intersection occurs
	float time;

	//The material of the object that was intersected
	const Material *material;

	//Need to store the index of the object for the closest intersection
	int objectIndex;

	//Flag to change object index for a new object on a closer intersection
	bool closerIntersectionFound;

	IntersectInfo() :
		time(std::numeric_limits<float>::infinity()),
		hitPoint(0.0f),
		normal(0.0f),
		material(NULL),
		objectIndex(-1),
		closerIntersectionFound(false)
	{}

	//Overloading "operator =" for class IntersectInfo
	IntersectInfo &operator =(const IntersectInfo &rhs) {
		hitPoint = rhs.hitPoint;
		material = rhs.material;
		normal = rhs.normal;
		time = rhs.time;

		objectIndex = rhs.objectIndex;
		closerIntersectionFound = rhs.closerIntersectionFound;

		return *this;
	}
};