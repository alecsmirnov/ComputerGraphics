#include "LightSource.h"

#include <cmath>

static constexpr GLfloat FRAME_FAULT = 0.015f;

static constexpr GLdouble RADIUS = 0.5;
static constexpr GLint    SLICES = 4;
static constexpr GLint    STACKS = 2;

static constexpr GLfloat SPEED = 0.5f;

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

void LightSource::moveForward() {
	position.setZ(position.getZ() + SPEED);
}

void LightSource::moveBack() {
	position.setZ(position.getZ() - SPEED);
}

void LightSource::moveLeft() {
	position.setX(position.getX() + SPEED);
}

void LightSource::moveRight() {
	position.setX(position.getX() - SPEED);
}

void LightSource::moveUp() {
	position.setY(position.getY() + SPEED);
}

void LightSource::moveDown() {
	position.setY(position.getY() - SPEED);
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

void LightSource::drawFrame() const {
	GLUquadricObj* quad_obj = gluNewQuadric();

	glPushMatrix();
	glTranslated(position.getX(), position.getY(), position.getZ());

	auto frame = [&](GLfloat frame_width, Color frame_color) {
		glColor3f(frame_color.R, frame_color.G, frame_color.B);
		glLineWidth(frame_width);

		gluQuadricDrawStyle(quad_obj, GLU_LINE);
		gluSphere(quad_obj, RADIUS * LIGHT_FRAME_COEF + FRAME_FAULT, SLICES, STACKS);

		glLineWidth(1.0f);
	};

	frame(LIGHT_INNER_FRAME_WIDTH, LIGHT_INNER_FRAME_COLOR);
	frame(LIGHT_OUTER_FRAME_WIDTH, LIGHT_OUTER_FRAME_COLOR);

	gluDeleteQuadric(quad_obj);

	glPopMatrix();
}