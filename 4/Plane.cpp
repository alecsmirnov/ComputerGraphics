#include "Plane.h"

#include <cmath>

static constexpr GLfloat FRAME_FAULT = 0.014f;

Plane::Plane(const Vector3f& center, GLfloat scale_x, GLfloat scale_z, Color color, Material material) {
	this->center = center;
	this->scale_x = scale_x;
	this->scale_z = scale_z;
	this->color = color;
	this->material = material;

	visibility = true;
}

void Plane::setMaterial(Material material) {
	this->material = material;
}

void Plane::setVisibility(bool visibility) {
	this->visibility = visibility;
}

Material Plane::getMaterial() const {
	return material;
}

bool Plane::getVisibility() const {
	return visibility;
}

void Plane::draw() {
	glPushMatrix();

	glScalef(scale_x, 1, scale_z);
	glTranslated(center.getX(), center.getY(), center.getZ());

	glColor3f(color.R, color.G, color.B);
	glBegin(GL_QUADS);

	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);

	glEnd();

	glPopMatrix();
}

void Plane::drawFrame() {
	glPushMatrix();
	glScalef(scale_x, 1, scale_z);
	glTranslated(center.getX(), center.getY(), center.getZ());

	auto frame = [&](GLfloat frame_width, Color frame_color) {
		glColor3f(frame_color.R, frame_color.G, frame_color.B);
		glLineWidth(frame_width);

		glBegin(GL_LINE_LOOP);
	
		glVertex3f(-1.0f * FRAME_COEF + FRAME_FAULT, 0.0f, -1.0f * FRAME_COEF + FRAME_FAULT);
		glVertex3f(-1.0f * FRAME_COEF + FRAME_FAULT, 0.0f, 1.0f * FRAME_COEF - FRAME_FAULT);
		glVertex3f(1.0f * FRAME_COEF - FRAME_FAULT, 0.0f, 1.0f * FRAME_COEF - FRAME_FAULT);
		glVertex3f(1.0f * FRAME_COEF - FRAME_FAULT, 0.0f, -1.0f * FRAME_COEF + FRAME_FAULT);

		glEnd();

		glLineWidth(1.0f);
	};

	frame(INNER_FRAME_WIDTH, INNER_FRAME_COLOR);
	frame(OUTER_FRAME_WIDTH, OUTER_FRAME_COLOR);

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