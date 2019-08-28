#include "LightSource.h"

#include <cmath>

static constexpr GLdouble RADIUS = 0.25;
static constexpr GLint    SLICES = 4;
static constexpr GLint    STACKS = 2;

LightSource::LightSource(const Vector3f& position, Color color) {
	this->position = position;
	this->color = color;
}

void LightSource::setPosition(const Vector3f& position) {
	this->position = position;
}

void LightSource::setColor(Color color) {
	this->color = color;
}

Vector3f LightSource::getPosition() const {
	return position;
}

Color LightSource::getColor() const {
	return color;
}

void LightSource::draw() const {
	GLUquadricObj* quad_obj = gluNewQuadric();

	glPushMatrix();

	glTranslated(position.getX(), position.getY(), position.getZ());
	glColor3f(color.R, color.G, color.B);
	gluQuadricDrawStyle(quad_obj, GLU_FILL);
	gluSphere(quad_obj, RADIUS, SLICES, STACKS);

	glPopMatrix();

	gluDeleteQuadric(quad_obj);
}

void LightSource::rotate(GLfloat angle, const Vector3f& rotate_front) {
	GLfloat cos_a = std::cosf(angle);
	GLfloat sin_a = std::sinf(angle);

	GLfloat new_x = (cos_a + (1 - cos_a) * rotate_front.getX() * rotate_front.getX()) * position.getX() +
					((1 - cos_a) * rotate_front.getX() * rotate_front.getY() - rotate_front.getZ() * sin_a) * position.getY() +
					((1 - cos_a) * rotate_front.getX() * rotate_front.getZ() + rotate_front.getY() * sin_a) * position.getZ();

	GLfloat new_y = ((1 - cos_a) * rotate_front.getX() * rotate_front.getY() + rotate_front.getZ() * sin_a) * position.getX() +
					(cos_a + (1 - cos_a) * rotate_front.getY() * rotate_front.getY()) * position.getY() +
					((1 - cos_a) * rotate_front.getY() * rotate_front.getZ() - rotate_front.getX() * sin_a) * position.getZ();

	GLfloat new_z = ((1 - cos_a) * rotate_front.getX() * rotate_front.getZ() - rotate_front.getY() * sin_a) * position.getX() +
					((1 - cos_a) * rotate_front.getY() * rotate_front.getZ() + rotate_front.getX() * sin_a) * position.getY() +
					(cos_a + (1 - cos_a) * rotate_front.getZ() * rotate_front.getZ()) * position.getZ();

	position.setCoords(new_x, new_y, new_z);
}