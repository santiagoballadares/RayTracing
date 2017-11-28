#pragma once

#include "Ray.h"
#include "IntersectInfo.h"
#include "Material.h"
#include "Utils.h"

/**
 * Object class
 * 
 * The father class of all the objects displayed. Some features would be shared 
 * between objects, others will be overloaded.
 */
class Object {
protected:
	glm::mat4 m_transform;
	Material m_material;

public:
	Object(
		const glm::mat4 &transform = glm::mat4(1.0f),
		const Material &material = Material()
	) :
		m_transform(transform),
		m_material(material)
	{}

	virtual bool Intersect(const Ray &ray, IntersectInfo &info) const { return true; }
	glm::vec3 Position() const { return glm::vec3(m_transform[3][0], m_transform[3][1], m_transform[3][2]); }
	const Material *MaterialPtr() const { return &m_material; }
};