#ifndef PLANE_H
#define PLANE_H

#include "Figure.h"

class Plane : public Figure {
public:
	Plane() = default;
	Plane(const Vector3f& center, GLfloat scale_x, GLfloat scale_z, Color color, Material material);

	void setMaterial(Material material);
	void setVisibility(bool visibility);

	Material getMaterial() const;
	bool getVisibility() const;

	void draw() const;
	void drawFrame() const;
	bool isHit(Ray ray, std::vector<Collision>& collisions) const;
	bool isSimpleHit(Ray ray) const;

private:
	Vector3f center;
	GLfloat scale_x;
	GLfloat scale_z;

	Color color;
	Material material;

	bool visibility;
};

#endif