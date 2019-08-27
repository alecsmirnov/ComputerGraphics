#ifndef SPHERE_H
#define SPHERE_H

#include "Figure.h"
#include "Color.h"

class Sphere : public Figure {
public:
	Sphere() = default;
	Sphere(GLfloat radius, const Vector3f& center, Color color, Material material);

	void setMaterial(Material material);
	void setVisibility(bool visibility);

	Material getMaterial() const;
	bool getVisibility() const;
	ObjectType getType() const;

	void draw();
	void drawFrame();
	bool isHit(Ray ray, std::vector<Collision>& collisions);
	bool isSimpleHit(Ray ray);

private:
	static constexpr ObjectType type = ObjectType::SPHERE;

	GLfloat radius;
	Vector3f center;

	Color color;
	Color frame_color;
	Material material;

	bool visibility;
};

#endif