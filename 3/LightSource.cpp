#include "LightSource.h"

static constexpr GLfloat FRAME_FAULT = 0.015f;

static constexpr GLdouble RADIUS = 0.5;
static constexpr GLint    SLICES = 4;
static constexpr GLint    STACKS = 2;

LightSource::LightSource(const Vector3f& position, Color color) {
	this->position = position;
	this->color = color;
}

GLuint LightSource::getTexture() const {
	return 0;
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
		gluSphere(quad_obj, RADIUS * FRAME_COEF + FRAME_FAULT, SLICES, STACKS);

		glLineWidth(1.0f);
	};

	frame(INNER_FRAME_WIDTH, INNER_FRAME_COLOR);
	frame(OUTER_FRAME_WIDTH, OUTER_FRAME_COLOR);

	gluDeleteQuadric(quad_obj);

	glPopMatrix();
}