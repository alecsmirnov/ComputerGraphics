#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3f.h"

class Camera {
public:
	Camera() : up({0.0f, 1.0f, 0.0f}), speed(0) {}
	Camera(const Vector3f& position, const Vector3f& front, const Vector3f& up, GLfloat speed = 0.0f);

	void setPosition(const Vector3f& position);
	void setFront(const Vector3f& front);
	void setUp(const Vector3f& up);
	void setSpeed(GLfloat speed);

	Vector3f getPosition() const;
	Vector3f getFront() const;
	Vector3f getUp() const;
	GLfloat getSpeed() const;

	void mouseView(GLint width, GLint height);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

private:
	void moveStraight(GLfloat direction_speed);
	void moveApart(GLfloat direction_speed);
	void rotateView(GLfloat angle, const Vector3f& up_rotate);

private:
	Vector3f position;
	Vector3f front;
	Vector3f up;

	GLfloat speed;
};

#endif