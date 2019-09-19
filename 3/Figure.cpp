#include "Figure.h"

#include <cmath>

static constexpr GLfloat ROTATE_COEF = 120.0f;

void Figure::setTexture(GLuint texture) {
	this->texture = texture;
}

Vector3f Figure::getPosition() const {
	return position;
}

Color Figure::getColor() const {
	return color;
}

GLuint Figure::getTexture() const {
	return texture;
}


void Figure::moveForward() {
	position.setZ(position.getZ() + MOVE_SPEED);
}

void Figure::moveBack() {
	position.setZ(position.getZ() - MOVE_SPEED);
}

void Figure::moveLeft() {
	position.setX(position.getX() + MOVE_SPEED);
}

void Figure::moveRight() {
	position.setX(position.getX() - MOVE_SPEED);
}

void Figure::moveUp() {
	position.setY(position.getY() + MOVE_SPEED);
}

void Figure::moveDown() {
	position.setY(position.getY() - MOVE_SPEED);
}

void Figure::rotate(const Vector3f& center, const Vector3f& up) {
	if (up.getX() != 0.0f || up.getY() != 0.0f || up.getZ() != 0.0f) {
		Vector3f view = position - center;

		GLfloat cos_a = std::cosf(MOVE_SPEED / ROTATE_COEF);
		GLfloat sin_a = std::sinf(MOVE_SPEED / ROTATE_COEF);

		GLfloat new_x = (cos_a + (1 - cos_a) * up.getX() * up.getX()) * view.getX() +
						((1 - cos_a) * up.getX() * up.getY() - up.getZ() * sin_a) * view.getY() +
						((1 - cos_a) * up.getX() * up.getZ() + up.getY() * sin_a) * view.getZ();

		GLfloat new_y = ((1 - cos_a) *  up.getX() * up.getY() + up.getZ() * sin_a) * view.getX() +
						(cos_a + (1 - cos_a) * up.getY() * up.getY()) * view.getY() +
						((1 - cos_a) * up.getY() * up.getZ() - up.getX() * sin_a) * view.getZ();

		GLfloat new_z = ((1 - cos_a) *  up.getX() * up.getZ() - up.getY() * sin_a) * view.getX() +
						((1 - cos_a) * up.getY() * up.getZ() + up.getX() * sin_a) *view.getY() +
						(cos_a + (1 - cos_a) * up.getZ() * up.getZ()) * view.getZ();

		position.setCoords(center + Vector3f(new_x, new_y, new_z));
	}
}