#ifndef PLANE_H
#define PLANE_H

#include "Figure.h"
#include "Color.h"

class Plane : public Figure {
public:
	Plane() = default;
	Plane(const Vector3f& center, GLfloat scale_x, GLfloat scale_z, Color color, Material material);

	void setMaterial(Material material);
	void setVisibility(bool) {}

	Material getMaterial() const;
	bool getVisibility() const { return visibility; };
	ObjectType getType() const;

	void draw();
	void drawFrame() {}
	bool isHit(Ray ray, std::vector<Collision>& collisions);
	bool isSimpleHit(Ray ray);

private:
	static constexpr ObjectType type = ObjectType::PLANE;
	static constexpr bool visibility = true;

	Vector3f center;
	GLfloat scale_x;
	GLfloat scale_z;

	Color color;
	Material material;

};

#endif