#pragma once

#include "Object.h"

/**
 * Quad class
 */
class Quad : public Object {
public:
	glm::vec3 v0, v1, v2, v3;

	//Vertices must be given in anticlock order
	Quad(
		const Material &material = Material(),
		const glm::vec3 &v0 = glm::vec3(0.0f),
		const glm::vec3 &v1 = glm::vec3(0.0f),
		const glm::vec3 &v2 = glm::vec3(0.0f),
		const glm::vec3 &v3 = glm::vec3(0.0f)
	) :
		Object(glm::mat4(1.0f), material),
		v0(v0),
		v1(v1),
		v2(v2),
		v3(v3)
	{}

	//To figure out if the Ray hit this object.
	virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
};

/**
 * Quad intersection using barycentric coordinates
 */
bool Quad::Intersect(const Ray &ray, IntersectInfo &info) const {
	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

	float NdotRD = glm::dot(normal, ray.direction);
	if (std::abs(NdotRD) < 0.0001f) {
		return false;
	}

	float distance = glm::dot(normal, v0);
	float NdotRO = glm::dot(normal, ray.origin);

	float t = (distance - NdotRO) / NdotRD;
	if (t < 0.0f) {
		return false;
	}

	glm::vec3 result = ray.origin + ray.direction * t;

	glm::vec3 barycentricA = Utils::BarycentricOptimized(result, v0, v1, v2);
	glm::vec3 barycentricB = Utils::BarycentricOptimized(result, v0, v2, v3);
	if ((barycentricA.x < 0.0f || barycentricA.x > 1.0f ||
		 barycentricA.y < 0.0f || barycentricA.y > 1.0f ||
		 barycentricA.z < 0.0f || barycentricA.z > 1.0f) &&
		(barycentricB.x < 0.0f || barycentricB.x > 1.0f ||
		 barycentricB.y < 0.0f || barycentricB.y > 1.0f ||
		 barycentricB.z < 0.0f || barycentricB.z > 1.0f)) {
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