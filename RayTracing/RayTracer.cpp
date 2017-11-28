#include "RayTracer.h"

/**
 * Wrapper function uses a global to remember the object
 */
RayTracer* objectToHandleRenderCall = NULL;

/**
 * Since a pointer-to-member-function is different from a pointer-to-function,
 * we need a Wrapper function to pass the render member-function to glutDisplayFunc
 */
void RayTracerRenderWrapper() {
	objectToHandleRenderCall->Render();
}

/**
 * Constructor
 */
RayTracer::RayTracer(int argc, char **argv) {
	Initialize(argc, argv);
}

/**
* Destructor
*/
RayTracer::~RayTracer() {
	Cleanup();
	objectToHandleRenderCall = NULL;
}

/**
 * Initialize OpenGL
 */
void RayTracer::Initialize(int argc, char **argv) {
	//Initialise OpenGL
	glutInit(&argc, argv);

	//Define the window size
	glutInitWindowSize(m_windowWidth, m_windowHeight);

	//Create the window for drawing
	glutCreateWindow(m_windowTitle.c_str());
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//Set the function the function that is called when the window must display
	objectToHandleRenderCall = this;
	glutDisplayFunc(RayTracerRenderWrapper);
}

/**
* Clean objects and lights
*/
void RayTracer::Cleanup() {
	//Objects
	for (unsigned int i = 0; i < m_objects.size(); ++i) {
		if (m_objects[i]) {
			delete m_objects[i];
		}
	}
	//Lights
	for (unsigned int i = 0; i < m_lights.size(); ++i) {
		if (m_lights[i]) {
			delete m_lights[i];
		}
	}
}

/**
 * Function for testing intersection against all the objects in the scene
 *
 * If an object is hit then the IntersectionInfo object should contain
 * the information about the intersection. Returns true if any object is hit,
 * false otherwise
 */
bool RayTracer::CheckIntersection(const Ray &ray, IntersectInfo &info) {
	bool hit = false;
	for (unsigned int i = 0; i<m_objects.size(); ++i) {
		if (m_objects[i]) {
			if (m_objects[i]->Intersect(ray, info)) {
				if (info.closerIntersectionFound) {
					info.objectIndex = i;
					info.closerIntersectionFound = false;
				}
				hit = true;
			}
		}
	}
	return hit;
	//Runing the function Intersect() of each of the objects would be one of the options.
	//Each time when intersect happens, the payload of ray will need to be updated, but that's no business of this function.
	//To make it clear, keyword const prevents the function from changing parameter ray.
}

/**
 * Function for testing intersections with shadows
 *
 * If the shadow ray hits any object then only the ambient light should be applied,
 * else full lighting should be used.
 * As soon as a hit is found we exit the loop, no more iterations are needed.
 */
bool RayTracer::CheckShadows(const Ray &ray, IntersectInfo &info) {
	bool isShadow = false;
	//Dumy info, we only care about the intersection
	IntersectInfo ii;
	for (unsigned int i = 0; i<m_objects.size(); ++i) {
		if (m_objects[i]) {
			if (info.objectIndex != i) {
				if (m_objects[i]->Intersect(ray, ii)) {
					isShadow = true;
					break;
				}
			}
		}
	}
	return isShadow;
}

/**
 * Recursive ray-casting function. It might be the most important Function in this demo cause it's the one decides the color of pixels.
 * The function CastRay() will have to deal with light(), shadow() and reflection(). The impement of them would also be important.
 *
 * This function is called for each pixel, and each time a ray is reflected/used
 * for shadow testing. The Payload object can be used to record information about
 * the ray trace such as the current color and the number of bounces performed.
 * This function should return either the time of intersection with an object
 * or minus one to indicate no intersection.
 */
float RayTracer::CastRay(Ray &ray, Payload &payload) {
	IntersectInfo info;
	Payload reflPayload = Payload();
	Payload refrPayload = Payload();

	if (CheckIntersection(ray, info)) {
		//Iterate lights
		for (unsigned int i = 0; i<m_lights.size(); ++i) {
			//Current light
			glm::vec3 lightDirection = glm::normalize(m_lights[i]->position - info.hitPoint + glm::vec3(BIAS));
			glm::vec3 eyeDirection = glm::normalize(ray.origin - info.hitPoint + glm::vec3(BIAS));

			//Shadow ray
			Ray shadowRay(info.hitPoint + info.normal*BIAS, lightDirection);

			if (CheckShadows(shadowRay, info)) {	//Is in shadow, apply ambient lighting only
				payload.color += info.material->diffuse * info.material->ambient;
			} else {	//Not in shadow, apply full lighting
				if (info.material->reflection > 0.0f && payload.numBounces < MAX_RAY_DEPTH) {
					payload.numBounces += 1;

					float facingratio = -glm::dot(ray.direction, info.normal);
					float fresneleffect = glm::mix(glm::pow(1 - facingratio, 3.0f), 1.0f, 0.1f);
					glm::vec3 reflDir = glm::normalize(ray.direction - info.normal * 2.0f * glm::dot(ray.direction, info.normal));

					Ray reflRay(info.hitPoint + info.normal*BIAS, reflDir);

					reflPayload.numBounces = payload.numBounces;
					float reflresult = CastRay(reflRay, reflPayload);

					glm::vec3 reflection = (reflresult > 0.0f) ? reflPayload.color : glm::vec3(0);
					glm::vec3 refraction = glm::vec3(0);

					//Compute refraction ray
					if (info.material->transparency > 0.0f) {
						float n1 = 1.0f;  //Index of refraction of original medium
						float n2 = 1.1f;  //Index of refraction of new medium
						float n = n1 / n2;
						float c1 = -glm::dot(info.normal, ray.direction);
						float c2 = sqrt(1.0f - n * n * (1.0f - c1 * c1));
						glm::vec3 refrDir = glm::normalize((n * ray.direction) + (n * c1 - c2) * info.normal);

						reflPayload.numBounces += 1;
						refrPayload.numBounces = reflPayload.numBounces;
						Ray refrRay(info.hitPoint - info.normal*BIAS, refrDir);
						float refrResult = CastRay(refrRay, refrPayload);
						refraction = refrPayload.color * info.material->transparency;
					}

					payload.color += (reflection * fresneleffect + refraction * (1 - fresneleffect) * info.material->reflection) * info.material->diffuse;
				}

				//Ambient
				glm::vec3 ambient = info.material->diffuse * info.material->ambient;

				//Diffuse
				float NdotL = glm::max(0.0f, glm::dot(info.normal, lightDirection));
				glm::vec3 diff = info.material->diffuse * NdotL * 0.4f * 0.7f;

				// Specular
				/*glm::vec3 R = 2.0f * info.normal * glm::max(0.0f, glm::dot(info.normal, lightDirection)) - lightDirection;
				float s = glm::pow(glm::max(0.0f, glm::dot(R, eyeDirection)), info.material->glossiness);
				glm::vec3 spec = glm::vec3(s * 1.0f) * info.material->specular;*/

				//Blinn-Phong
				glm::vec3 blinnDir = glm::normalize(lightDirection - ray.direction);
				float temp = glm::sqrt(glm::dot(blinnDir, blinnDir));
				glm::vec3 blinn = glm::vec3(0.0f);
				if (temp != 0.0f) {
					blinnDir = (1.0f / temp) * blinnDir;
					float b = glm::max(0.0f, glm::dot(blinnDir, info.normal));
					blinn = glm::pow(b, info.material->glossiness) * info.material->specular * 0.7f;
				}

				//payload.color += ambient + diff + spec;
				payload.color += ambient + diff + blinn;
			}
		}
		info.objectIndex = -1;

		return info.time;
	} else {
		payload.color = glm::vec3(0.0f);
		//The Ray from camera hits nothing so nothing will be seen. In this case, the pixel should be totally black.
		return -1.0f;
	}
}

/**
 * Render Function

 * This is the main render function, it draws pixels onto the display
 * using GL_POINTS. It is called every time an update is required.

 * This function transforms each pixel into the space of the virtual
 * scene and casts a ray from the camera in that direction using CastRay
 * And for rendering,
 * 1) Clear the screen so we can draw a new frame
 * 2) Cast a ray into the scene for each pixel on the screen and use the returned color to render the pixel
 * 3) Flush the pipeline so that the instructions we gave are performed.
 */
void RayTracer::Render() {
	//Clear OpenGL Window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Three parameters of lookat(vec3 eye, vec3 center, vec3 up).
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projMatrix = glm::perspective(45.0f, (float)m_windowWidth / (float)m_windowHeight, 1.0f, 10000.0f);

	//Using GL_POINTS mode. In this mode, every vertex specified is a point.
	//Reference https://en.wikibooks.org/wiki/OpenGL_Programming/GLStart/Tut3
	glBegin(GL_POINTS);	

	for (int x = 0; x < m_windowWidth; ++x) {
		//Cover the entire display zone pixel by pixel, but without showing.
		for (int y = 0; y < m_windowHeight; ++y) {
			float pixelX = 2 * ((x + 0.5f) / m_windowWidth) - 1;	//Actually, (pixelX, pixelY) are the relative position of the point(x, y).
			float pixelY = -2 * ((y + 0.5f) / m_windowHeight) + 1;	//The displayzone will be decribed as a 2.0f x 2.0f platform and coordinate origin is the center of the display zone.

			//Decide the direction of each of the rays
			glm::vec4 worldNear = glm::inverse(viewMatrix) * glm::inverse(projMatrix) * glm::vec4(pixelX, pixelY, -1, 1);
			glm::vec4 worldFar = glm::inverse(viewMatrix) * glm::inverse(projMatrix) * glm::vec4(pixelX, pixelY, 1, 1);
			glm::vec3 worldNearPos = glm::vec3(worldNear.x, worldNear.y, worldNear.z) / worldNear.w;
			glm::vec3 worldFarPos = glm::vec3(worldFar.x, worldFar.y, worldFar.z) / worldFar.w;

			Payload payload;
			Ray ray(worldNearPos, glm::normalize(glm::vec3(worldFarPos - worldNearPos))); //Ray(const glm::vec3 &origin, const glm::vec3 &direction)

			if (CastRay(ray, payload) > 0.0f) {
				glColor3f(payload.color.x, payload.color.y, payload.color.z);
			} else {
				glColor3f(0.2f, 0.2f, 0.2f);
			}

			glVertex3f(pixelX, pixelY, 0.0f);
		}
	}

	glEnd();
	glFlush();
}