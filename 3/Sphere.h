#ifndef SPHERE_H
#define SPHERE_H

#include "Figure.h"

class Sphere : public Figure {
public:
	Sphere() = default;
	Sphere(GLfloat radius, const Vector3f& position, Color color);

	void draw() const;
	void drawFrame() const;

private:
	GLfloat radius;
};

#endif