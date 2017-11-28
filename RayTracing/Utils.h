#pragma once

#include "glm/glm.hpp"

#define CMP(x, y) (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

class Utils {
public:
	/**
	 * Function to solve a quadratic function
	 */
	static bool SolveQuadratic(const float &a, const float &b, const float &c, float &r1, float &r2) {
		float discr = b * b - 4 * a * c;

		if (discr < 0) {
			return false;
		}

		if (discr == 0) {
			r1 = r2 = -0.5 * b / a;
		}
		else {
			discr = sqrt(discr);

			double q = (b > 0) ?
				0.5 * (-b - discr) :
				0.5 * (-b + discr);

			r1 = q / a;
			r2 = c / q;
		}
		return true;
	}

	/**
	 * Function to calculate the barycentric coordinates of a triangle
	 */
	static glm::vec3 BarycentricOptimized(const glm::vec3 &p, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {
		glm::vec3 a = v1 - v0;
		glm::vec3 b = v2 - v0;
		glm::vec3 c = p - v0;

		float d00 = glm::dot(a, a);
		float d01 = glm::dot(a, b);
		float d11 = glm::dot(b, b);
		float d20 = glm::dot(c, a);
		float d21 = glm::dot(c, b);
		float denom = d00 * d11 - d01 * d01;

		if (CMP(denom, 0.0f)) {
			return glm::vec3();
		}

		glm::vec3 result;
		result.y = (d11 * d20 - d01 * d21) / denom;
		result.z = (d00 * d21 - d01 * d20) / denom;
		result.x = 1.0f - result.y - result.z;

		return result;
	}

	/**
	 * Function to calculate the barycentric coordinates of a triangle
	 */
	static glm::vec3 Barycentric(const glm::vec3 &p, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {
		glm::vec3 ap = p - v0;
		glm::vec3 bp = p - v1;
		glm::vec3 cp = p - v2;

		glm::vec3 ab = v1 - v0;
		glm::vec3 ac = v2 - v0;
		glm::vec3 bc = v2 - v1;
		glm::vec3 cb = v1 - v2;
		glm::vec3 ca = v0 - v2;

		glm::vec3 v = ab - Project(ab, cb);
		float a = 1.0f - (glm::dot(v, ap) / glm::dot(v, ab));

		v = bc - Project(bc, ac);
		float b = 1.0f - (glm::dot(v, bp) / glm::dot(v, bc));

		v = ca - Project(ca, ab);
		float c = 1.0f - (glm::dot(v, cp) / glm::dot(v, ca));

		return glm::vec3(a, b, c);
	}
	
	/**
	 * Function to calculate the vector projection
	 */
	static glm::vec3 Project(const glm::vec3 &length, const glm::vec3 &direction) {
		float dot = glm::dot(length, direction);
		float magSq = glm::dot(direction, direction);
		return direction * (dot / magSq);
	}

	/**
	* Function to calculate the area of a triangle
	*/
	static float CalculateTriangleArea(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {
		float l0 = sqrt((v1.x - v0.x)*(v1.x - v0.x) + (v1.y - v0.y)*(v1.y - v0.y) + (v1.z - v0.z)*(v1.z - v0.z));
		float l1 = sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y) + (v2.z - v1.z)*(v2.z - v1.z));
		float l2 = sqrt((v2.x - v0.x)*(v2.x - v0.x) + (v2.y - v0.y)*(v2.y - v0.y) + (v2.z - v0.z)*(v2.z - v0.z));

		float s = (l0 + l1 + l2) / 2.0f;
		return sqrt(s*(s - l0)*(s - l1)*(s - l2));
	}
};