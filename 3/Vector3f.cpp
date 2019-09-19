#include "Vector3f.h"

#include <cmath>

Vector3f::Vector3f(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3f::setX(GLfloat x) {
	this->x = x;
}

void Vector3f::setY(GLfloat y) {
	this->y = y;
}

void Vector3f::setZ(GLfloat z) {
	this->z = z;
}

void Vector3f::setCoords(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3f::setCoords(const Vector3f& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

GLfloat Vector3f::getX() const {
	return x;
}

GLfloat Vector3f::getY() const {
	return y;
}

GLfloat Vector3f::getZ() const {
	return z;
}

Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs) {
	return Vector3f(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY(), lhs.getZ() + rhs.getZ());
}

Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs) {
	return Vector3f(lhs.getX() - rhs.getX(), lhs.getY() - rhs.getY(), lhs.getZ() - rhs.getZ());
}

Vector3f operator*(const Vector3f& lhs, GLfloat val) {
	return Vector3f(lhs.getX() * val, lhs.getY() * val, lhs.getZ() * val);
}

Vector3f operator/(const Vector3f& lhs, GLfloat val) {
	return Vector3f(lhs.getX() / val, lhs.getY() / val, lhs.getZ() / val);
}

Vector3f cross(const Vector3f& vec_a, const Vector3f& vec_b) {
	return Vector3f(vec_a.getY() * vec_b.getZ() - vec_a.getZ() * vec_b.getY(),
					vec_a.getZ() * vec_b.getX() - vec_a.getX() * vec_b.getZ(),
					vec_a.getX() * vec_b.getY() - vec_a.getY() * vec_b.getX());
}

GLfloat scalarProduct(const Vector3f& vec_a, const Vector3f& vec_b) {
	return vec_a.getX() * vec_b.getX() + vec_a.getY() * vec_b.getY() + vec_a.getZ() * vec_b.getZ();
}

GLfloat norm(const Vector3f& vec) {
	return std::sqrtf(scalarProduct(vec, vec));
}

Vector3f normalize(const Vector3f& vec) {
	return vec / norm(vec);
}

Vector3f polygonNormal(const Vector3f& vec_a, const Vector3f& vec_b, const Vector3f& vec_c) {
	return normalize(cross(vec_c - vec_b, vec_a - vec_b));
}