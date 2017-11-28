#ifndef RayTracer_h_
#define RayTracer_h_

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include <GL/glut.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Ray.h"
#include "IntersectInfo.h"
#include "Payload.h"
#include "Object.h"
#include "Light.h"

#define _USE_MATH_DEFINES
#define MAX_RAY_DEPTH 5

/**
 * Ray Tracer class
 */
class RayTracer {
private:
	const float BIAS = 0.01f;

	std::vector<Light*>  m_lights;
	std::vector<Object*> m_objects;

	std::string m_windowTitle = "Ray Tracer";
	int m_windowWidth  = 800;
	int m_windowHeight = 600;

public:
	RayTracer(int argc, char **argv);
	~RayTracer();

	void AddLight(Light *light)		{ m_lights.push_back(light); }
	void AddObject(Object *object)	{ m_objects.push_back(object); }
	
	void Start() { glutMainLoop(); }

	void Render();

private:
	void Initialize(int argc, char **argv);
	void Cleanup();
	
	bool CheckIntersection(const Ray &ray, IntersectInfo &info);
	bool CheckShadows(const Ray &ray, IntersectInfo &info);
	float CastRay(Ray &ray, Payload &payload);
};
#endif /* RayTracer_h_ */