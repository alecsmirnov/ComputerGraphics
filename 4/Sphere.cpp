#include "Sphere.h"

#include <cmath>

static constexpr GLint NODE_SIZE = 10;

static constexpr GLfloat COLLISION_TIME	= 0.00001f;

Sphere::Sphere(GLfloat radius, const Vector3f& center, Color color, Material material) {
	this->radius = radius;
	this->center = center;
	this->color = color;
	this->material = material;

	visibility = true;
}

void Sphere::setMaterial(Material material) {
	this->material = material;
}

void Sphere::setVisibility(bool visibility) {
	this->visibility = visibility;
}

Material Sphere::getMaterial() const {
	return material;
}

bool Sphere::getVisibility() const {
	return visibility;
}

void Sphere::draw() {
	GLUquadricObj* quad_obj = gluNewQuadric();

	glPushMatrix();

	glTranslated(center.getX(), center.getY(), center.getZ());
	glColor3f(color.R, color.G, color.B);
	gluQuadricDrawStyle(quad_obj, GLU_FILL);
	gluSphere(quad_obj, radius, NODE_SIZE, NODE_SIZE);

	glPopMatrix();

	gluDeleteQuadric(quad_obj);
}

void Sphere::drawFrame() {
	GLUquadricObj* quad_obj = gluNewQuadric();

	glPushMatrix();
	glTranslated(center.getX(), center.getY(), center.getZ());

	auto frame = [&](GLfloat frame_width, Color frame_color) {
		glColor3f(frame_color.R, frame_color.G, frame_color.B);
		glLineWidth(frame_width);

		gluQuadricDrawStyle(quad_obj, GLU_LINE);
		gluSphere(quad_obj, radius * FRAME_COEF, NODE_SIZE, NODE_SIZE);

		glLineWidth(1.0f);
	};

	frame(INNER_FRAME_WIDTH, INNER_FRAME_COLOR);
	frame(OUTER_FRAME_WIDTH, OUTER_FRAME_COLOR);
	
	gluDeleteQuadric(quad_obj);

	glPopMatrix();
}

bool Sphere::isHit(Ray ray, std::vector<Collision>& collisions) {
	bool hit = false;

	auto new_ray = transformRay(ray, center, Vector3f(radius, radius, radius));

	auto a = scalarProduct(new_ray.front, new_ray.front);
	auto b = scalarProduct(new_ray.position, new_ray.front);
	auto c = scalarProduct(new_ray.position, new_ray.position) - 1.0f;

	GLfloat discr = b * b - a * c;

	if (0 <= discr) {
		collisions.resize(0);

		GLfloat discr_root = std::sqrtf(discr);

		for (GLubyte i = 0; i != COLLISIONS_MAX; ++i) {
			discr_root = -discr_root;

			GLfloat time = (-b + discr_root) / a;

			if (COLLISION_TIME < time) {
				Collision collision;

				collision.time = time;
				collision.entering = true;
				collision.position = getRayPosition(ray, time);
				collision.normal = getRayPosition(new_ray, time);

				collisions.push_back(collision);

				hit = true;
			}
		}
	}

	return hit;
}

bool Sphere::isSimpleHit(Ray ray) {
	bool hit = false;

	auto new_ray = transformRay(ray, center, Vector3f(radius, radius, radius));

	auto a = scalarProduct(new_ray.front, new_ray.front);
	auto b = scalarProduct(new_ray.position, new_ray.front);
	auto c = scalarProduct(new_ray.position, new_ray.position) - 1.0f;

	GLfloat discr = b * b - a * c;

	if (0 <= discr) {
		GLfloat discr_root = std::sqrtf(discr);

		for (GLubyte i = 0; i != COLLISIONS_MAX; ++i) {
			discr_root = -discr_root;

			GLfloat time = (-b + discr_root) / a;

			if (COLLISION_TIME <= time && time <= 1.0f)
				hit = true;
		}
	}

	return hit;
}