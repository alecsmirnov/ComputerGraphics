﻿#ifndef SPHERE_H
#define SPHERE_H

#include "Figure.h"

class Sphere : public Figure {
public:
	Sphere() = default;
	Sphere(GLfloat radius, const Vector3f& center, Color color, Material material);

	void setMaterial(Material material);
	void setVisibility(bool visibility);

	Material getMaterial() const;
	bool getVisibility() const;

	void draw() const;
	void drawFrame() const;
	bool isHit(Ray ray, std::vector<Collision>& collisions) const;
	bool isSimpleHit(Ray ray) const;

private:
	GLfloat radius;
	Vector3f center;

	Color color;
	Material material;

	bool visibility;
};

#endif