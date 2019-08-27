#ifndef RAY_H
#define RAY_H

#include <vector>

#include "Vector3f.h"

struct Ray {
	Vector3f position;
	Vector3f front;
	GLubyte recurs_lvl;
};

struct Collision {
	GLfloat time;
	std::size_t object_id;
	bool entering;

	Vector3f position;
	Vector3f normal;
};

Vector3f getRayPosition(Ray ray, GLfloat time);
Ray transformRay(Ray ray, const Vector3f& shift, const Vector3f& scale);

#endif
