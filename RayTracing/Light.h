#pragma once

/**
 * Light class
 */
class Light {
public:
	glm::vec3 position;
	float intensity;

	Light() :
		position(glm::vec3(0.0f, 0.0f, 0.0f)),
		intensity(1.0f)
	{}

	Light(
		const glm::vec3 &position,
		const float intensity = 1.0f
	) :
		position(position),
		intensity(intensity)
	{}
};