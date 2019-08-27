#include "Plane.h"

#include <cmath>

Plane::Plane(const Vector3f& center, GLfloat scale_x, GLfloat scale_z, Color color, Material material) {
	this->center = center;
	this->scale_x = scale_x;
	this->scale_z = scale_z;
	this->color = color;
	this->material = material;
}

void Plane::setMaterial(Material material) {
	this->material = material;
}

Material Plane::getMaterial() const {
	return material;
}

ObjectType Plane::getType() const {
	return type;
}

void Plane::draw() {
	glPushMatrix();

	glScalef(scale_x, 1, scale_z);
	glTranslated(center.getX(), center.getY(), center.getZ());

	glBegin(GL_QUADS);
	glColor3f(color.R, color.G, color.B);

	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, -1);

	glEnd();

	glPopMatrix();
}

bool Plane::isHit(Ray ray, std::vector<Collision>& collisions) {
	bool hit = false;

	auto new_ray = transformRay(ray, center, Vector3f(scale_x, 1, scale_z));

	auto denom = new_ray.front.getY();
	auto time = -new_ray.position.getY() / denom;

	if (0.0001f <= std::fabsf(denom) && 0.0f < time) {
		auto x_hit = new_ray.position.getX() + new_ray.front.getX() * time;
		auto z_hit = new_ray.position.getZ() + new_ray.front.getZ() * time;

		if (-1.0f <= x_hit && x_hit <= 1.0f && -1.0f <= z_hit && z_hit <= 1.0f) {
			collisions.resize(1);

			collisions[0].time = time;
			collisions[0].entering = true;
			collisions[0].position = getRayPosition(ray, time);
			collisions[0].normal = Vector3f(0, 1, 0);

			hit = true;
		}
	}

	return hit;
}

bool Plane::isSimpleHit(Ray ray) {
	bool hit = false;

	auto new_ray = transformRay(ray, center, Vector3f(scale_x, 1, scale_z));

	auto denom = new_ray.front.getY();
	auto time = -new_ray.position.getY() / denom;

	if (0.0001f <= std::fabsf(denom) && 0.0f < time) {
		auto x_hit = new_ray.position.getX() + new_ray.front.getX() * time;
		auto z_hit = new_ray.position.getZ() + new_ray.front.getZ() * time;

		if (-1.0f <= x_hit && x_hit <= 1.0f && -1.0f <= z_hit && z_hit <= 1.0f && 0.0f <= time && time <= 1.0f)
			hit = true;
	}

	return hit;
}