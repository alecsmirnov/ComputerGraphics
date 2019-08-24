#include "Camera.h"

#include <cmath>

static constexpr GLfloat PI_F = 3.14159265358979f;

static constexpr GLfloat DEGREE = 1.0f;
static constexpr GLfloat DEGREE_BASE = 90.0f;

static constexpr GLfloat MOUSE_SHIFT_FACTOR = 600.0f;

Camera::Camera(const Vector3f& position, const Vector3f& front, const Vector3f& up, GLfloat speed) {
	this->position = position;
	this->front = front;
	this->up = up;
	this->speed = speed;
}

void Camera::setPosition(const Vector3f& position) {
	this->position = position;
}

void Camera::setFront(const Vector3f& front) {
	this->front = front;
}

void Camera::setUp(const Vector3f& up) {
	this->up = up;
}

void Camera::setSpeed(GLfloat speed) {
	this->speed = speed;
}

Vector3f Camera::getPosition() const {
	return position;
}

Vector3f Camera::getFront() const {
	return front;
}

Vector3f Camera::getUp() const {
	return up;
}

GLfloat Camera::getSpeed() const {
	return speed;
}

void Camera::mouseView(GLint width, GLint height) {
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);

	GLint center_x = width / 2;
	GLint center_y = height / 2;

	if (mouse_pos.x != center_x || mouse_pos.y != center_y) {
		SetCursorPos(center_x, center_y);

		GLfloat angle_x = (center_x - mouse_pos.x) / MOUSE_SHIFT_FACTOR;
		GLfloat angle_y = (center_y - mouse_pos.y) / MOUSE_SHIFT_FACTOR;

		rotateView(angle_x, up);
		rotateView(angle_y, normalize(cross(front - position, up)));
	}
}

void Camera::moveUp() {
	moveStraight(speed);
}

void Camera::moveDown() {
	moveStraight(-speed);
}

void Camera::moveLeft() {
	moveApart(speed);
}

void Camera::moveRight() {
	moveApart(-speed);
}

void Camera::moveStraight(GLfloat direction_speed) {
	auto view = front - position;

	position.setCoords(position + view * direction_speed);
	front.setCoords(front + view * direction_speed);
}

void Camera::moveApart(GLfloat direction_speed) {
	auto axis = normalize(cross(front - position, up));
	
	rotateView(PI_F / 180 * DEGREE_BASE, up);

	auto view = front - position;

	position.setX(position.getX() + view.getX() * direction_speed);
	position.setZ(position.getZ() + view.getZ() * direction_speed);

	front.setX(front.getX() + view.getX() * direction_speed);
	front.setZ(front.getZ() + view.getZ() * direction_speed);

	rotateView(-PI_F / 180 * DEGREE_BASE, up);
}

void Camera::rotateView(GLfloat angle, const Vector3f& up_rotate) {
	auto view = front - position;
	
	auto cos_a = std::cosf(angle);
	auto sin_a = std::sinf(angle);

	GLfloat new_x = (cos_a + (1 - cos_a) * up_rotate.getX() * up_rotate.getX()) * view.getX() +
					((1 - cos_a) * up_rotate.getX() * up_rotate.getY() - up_rotate.getZ() * sin_a) * view.getY() +
					((1 - cos_a) * up_rotate.getX() * up_rotate.getZ() + up_rotate.getY() * sin_a) * view.getZ();

	GLfloat new_y = ((1 - cos_a) * up_rotate.getX() * up_rotate.getY() + up_rotate.getZ() * sin_a) * view.getX() +
					(cos_a + (1 - cos_a) * up_rotate.getY() * up_rotate.getY()) * view.getY() +
					((1 - cos_a) * up_rotate.getY() * up_rotate.getZ() - up_rotate.getX() * sin_a) * view.getZ();

	GLfloat new_z = ((1 - cos_a) * up_rotate.getX() * up_rotate.getZ() - up_rotate.getY() * sin_a) * view.getX() +
					((1 - cos_a) * up_rotate.getY() * up_rotate.getZ() + up_rotate.getX() * sin_a) * view.getY() +
					(cos_a + (1 - cos_a) * up_rotate.getZ() * up_rotate.getZ()) * view.getZ();

	front.setCoords(position + Vector3f(new_x, new_y, new_z));
}