#pragma once

#include "Object.h"

//#define BARYCENTRIC

/**
 * Triangle class
 */
class Triangle : public Object {
public:
	glm::vec3 v0, v1, v2;

	//Vertices must be given in anticlock order
	Triangle(
		const Material &material = Material(),
		const glm::vec3 &v0 = glm::vec3(0.0f),
		const glm::vec3 &v1 = glm::vec3(0.0f),
		const glm::vec3 &v2 = glm::vec3(0.0f)
	) :
		Object(glm::mat4(1.0f), material),
		v0(v0),
		v1(v1),
		v2(v2)
	{}

	//To figure out if the Ray hit this object.
	virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
};

/**
 * Triangle intersection using:
 * 1) Barycentric coordinates method
 * 2) Möller-Trumbore algorithm
 */
bool Triangle::Intersect(const Ray &ray, IntersectInfo &info) const {
	float t;

	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

#ifdef BARYCENTRIC
	//Barycentric coordinates method
	float NdotRD = glm::dot(normal, ray.direction);
	if (std::abs(NdotRD) < 0.0001f) {
		return false;
	}

	float distance = glm::dot(normal, v0);
	float NdotRO = glm::dot(normal, ray.origin);

	t = (distance - NdotRO) / NdotRD;
	if (t < 0.0f) {
		return false;
	}

	glm::vec3 intPoint = ray.origin + ray.direction * t;

	glm::vec3 barycentric = Utils::BarycentricOptimized(intPoint, v0, v1, v2);
	if (barycentric.x < 0.0f || barycentric.x > 1.0f ||
		barycentric.y < 0.0f || barycentric.y > 1.0f ||
		barycentric.z < 0.0f || barycentric.z > 1.0f) {
		return false;
	}
#else
	//Möller-Trumbore algorithm
	glm::vec3 e1 = v1 - v0;
	glm::vec3 e2 = v2 - v0;

	glm::vec3 pvec = glm::cross(ray.direction, e2);
	float det = glm::dot(e1, pvec);

	if (det > -0.00001f && det < 0.00001f) {
		return false;
	}

	float invDet = 1 / det;

	glm::vec3 tvec = ray.origin - v0;
	float u = glm::dot(tvec, pvec) * invDet;

	if (u < 0.0f || u > 1.0f) {
		return false;
	}

	glm::vec3 qvec = glm::cross(tvec, e1);
	float v = glm::dot(ray.direction, qvec) * invDet;

	if (v < 0.0f || (u + v) > 1.0f) {
		return false;
	}

	t = glm::dot(e2, qvec) * invDet;

	if (t < 0.0f) {
		return false;
	}
#endif

	if (t < info.time) {
		info.time = t;
		info.hitPoint = ray.origin + (ray.direction * info.time);
		info.normal = normal;
		info.material = &m_material;
		info.closerIntersectionFound = true;
	}
	return true;
}