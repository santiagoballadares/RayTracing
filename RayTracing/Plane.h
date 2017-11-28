#pragma once

#include "Object.h"

/**
 * Plane class
 */
class Plane : public Object {
public:
	glm::vec3 normal, pointOnPlane;

	//The normal shows which way the plane is facing
	Plane(
		const Material &material = Material(),
		const glm::vec3 &normal = glm::vec3(0.0f),
		const glm::vec3 &pointOnPlane = glm::vec3(0.0f)
	) :
		Object(glm::translate(glm::mat4(1.0f), pointOnPlane), material),
		normal(glm::normalize(normal)),
		pointOnPlane(pointOnPlane)
	{}

	//To figure out if the Ray hit this object.
	virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
};

/**
 * Plane intersection using the Point-Normal form of a plane
 */
bool Plane::Intersect(const Ray &ray, IntersectInfo &info) const {
	float NdotRD = glm::dot(normal, ray.direction);

	if (std::abs(NdotRD) < 0.0001f) {
		return false;
	}

	glm::vec3 pointOrigin = pointOnPlane - ray.origin;
	float t = glm::dot(normal, pointOrigin) / NdotRD;

	if (t < 0.0f) {
		return false;
	}

	if (t < info.time) {
		info.time = t;
		info.hitPoint = ray.origin + (ray.direction * info.time);
		info.normal = normal;
		info.material = &m_material;
		info.closerIntersectionFound = true;
	}
	return true;
}