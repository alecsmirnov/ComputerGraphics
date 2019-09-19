#include "Sphere.h"

static constexpr GLint NODE_SIZE = 10;

Sphere::Sphere(GLfloat radius, const Vector3f& position, Color color) {
	this->radius = radius;
	this->position = position;
	this->color = color;

	texture = 0;
}

void Sphere::draw() const {	
	glEnable(GL_TEXTURE_2D);

	GLUquadricObj* quad_obj = gluNewQuadric();

	glPushMatrix();

	glTranslated(position.getX(), position.getY(), position.getZ());
	glColor3f(color.R, color.G, color.B);
	gluQuadricDrawStyle(quad_obj, GLU_FILL);
	gluQuadricTexture(quad_obj, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluSphere(quad_obj, radius, NODE_SIZE, NODE_SIZE);

	glPopMatrix();

	gluDeleteQuadric(quad_obj);

	glDisable(GL_TEXTURE_2D);
}

void Sphere::drawFrame() const {
	GLUquadricObj* quad_obj = gluNewQuadric();

	glPushMatrix();
	glTranslated(position.getX(), position.getY(), position.getZ());

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