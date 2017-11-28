#include "RayTracer.h"
#include "Plane.h"
#include "Triangle.h"
#include "Quad.h"
#include "Sphere.h"

int main(int argc, char **argv) {
	RayTracer *rayTracer = new RayTracer(argc, argv);

	rayTracer->AddLight(new Light(glm::vec3(-10.0f, 10.0f, 3.0f)));
	
	rayTracer->AddObject(new Plane(
		Material(glm::vec3(0.2f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f), 50.0f, 0.5f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)
	));
	
	rayTracer->AddObject(new Quad(
		Material(glm::vec3(0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f), 50.0f, 0.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 7.0f),
		glm::vec3(0.0f, 7.0f, 7.0f),
		glm::vec3(0.0f, 7.0f, 0.0f)
	));

	rayTracer->AddObject(new Quad(
		Material(glm::vec3(0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f), 50.0f, 0.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 7.0f, 0.0f),
		glm::vec3(-7.0f, 7.0f, 0.0f),
		glm::vec3(-7.0f, 0.0f, 0.0f)
	));

	rayTracer->AddObject(new Quad(
		Material(glm::vec3(0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f), 50.0f, 0.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-7.0f, 0.0f, 0.0f),
		glm::vec3(-7.0f, 0.0f, 7.0f),
		glm::vec3(0.0f, 0.0f, 7.0f)
	));

	rayTracer->AddObject(new Triangle(
		Material(glm::vec3(0.2f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f), 100.0f, 0.5f),
		glm::vec3(-2.0f, 1.5f, 3.0f),
		glm::vec3(-1.0f, 1.5f, 6.0f),
		glm::vec3(-1.5f, 6.5f, 4.5f)
	));

	rayTracer->AddObject(new Triangle(
		Material(glm::vec3(0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f), 100.0f, 0.5f, 0.7f),
		glm::vec3(-6.0f, 7.0f, 4.0f),
		glm::vec3(-8.0f, 4.0f, 7.0f),
		glm::vec3(-4.0f, 4.0f, 7.0f)
	));

	rayTracer->AddObject(new Sphere(
		Material(glm::vec3(0.2f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f), 100.0f, 0.5f),
		glm::vec3(-5.0f, 1.0f, 2.0f),
		1.5f
	));

	rayTracer->AddObject(new Sphere(
		Material(glm::vec3(0.2f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), 100.0f, 0.5f),
		glm::vec3(-7.0f, 4.0f, 5.0f),
		1.0f
	));

	rayTracer->AddObject(new Sphere(
		Material(glm::vec3(0.2f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f), 100.0f, 0.5f),
		glm::vec3(-4.0f, 1.5f, 6.0f),
		1.0f
	));

	rayTracer->Start();

	return 0;
}