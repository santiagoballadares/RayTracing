#pragma once

/**
 * Material class
 */
class Material {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float glossiness;
	float reflection;
	float transparency;

	Material() :
		ambient(1.0f),
		diffuse(1.0f),
		specular(1.0f),
		glossiness(10.0f),
		reflection(0.0f),
		transparency(0.0f)
	{}

	Material(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float glossiness,
		float reflection = 0.0f,
		float transparency = 0.0f
	) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		glossiness(glossiness),
		reflection(reflection),
		transparency(transparency)
	{}
};