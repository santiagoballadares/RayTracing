#pragma once

#include "Object.h"

//#define GEOMETRIC

/**
 * Sphere class
 */
class Sphere : public Object {
public:
	float radius, radius2;

	Sphere(
		const Material &material = Material(),
		const glm::vec3 &center = glm::vec3(0.0f),
		const float &r = 1.0f
	) :
		Object(glm::translate(glm::mat4(1.0f), center), material),
		radius(r),
		radius2(r * r)
	{}

	//To figure out if the Ray hit this object.
	virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
};

/**
 * Sphere intersection using:
 * 1) Geometric method
 * 2) Analytic method
 */
bool Sphere::Intersect(const Ray &ray, IntersectInfo &info) const {
	float t1, t2;

#ifdef GEOMETRIC
	//Geometric method
	glm::vec3 l = Position() - ray.origin;
	float tca = glm::dot(l, ray.direction);
	if  (tca < 0.0f) {
		return false;
	}

	float d2 = glm::dot(l, l) - tca * tca;
	if (d2 > radius2) {
		return false;
	}

	float thc = sqrt(radius2 - d2);
	t1 = tca - thc;
	t2 = tca + thc;
#else
	//Analytic method
	glm::vec3 deltap = ray.origin - Position();
	float a = glm::dot(ray.direction, ray.direction);
	float b = glm::dot(deltap, ray.direction) * 2;
	float c = glm::dot(deltap, deltap) - radius2;

	if (!Utils::SolveQuadratic(a, b, c, t1, t2)) {
		return false;
	}
#endif

	if (t1 > t2) {
		float tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	if (t1 < 0.0f) {
		t1 = t2;
		if (t1 < 0.0f || std::isnan(t1)) {
			return false;
		}
	}

	if (t1 < info.time) {
		info.time = t1;
		info.hitPoint = ray.origin + (ray.direction * info.time);
		info.normal = glm::normalize(info.hitPoint - Position());
		info.material = &m_material;
		info.closerIntersectionFound = true;
	}
	return true;
}