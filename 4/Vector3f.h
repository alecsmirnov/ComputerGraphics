#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <GL/freeglut.h>

class Vector3f {
public:
	Vector3f() : x(0), y(0), z(0) {}
	Vector3f(GLfloat x, GLfloat y, GLfloat z);

	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	void setCoords(GLfloat x, GLfloat y, GLfloat z);
	void setCoords(const Vector3f& vec);

	GLfloat getX() const;
	GLfloat getY() const;
	GLfloat getZ() const;

private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs);
Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs);
Vector3f operator*(const Vector3f& lhs, GLfloat val);
Vector3f operator/(const Vector3f& lhs, GLfloat val);

Vector3f cross(const Vector3f& vec_a, const Vector3f& vec_b);
GLfloat scalarProduct(const Vector3f& vec_a, const Vector3f& vec_b);
GLfloat norm(const Vector3f& vec);
Vector3f normalize(const Vector3f& vec);
Vector3f polygonNormal(const Vector3f& vec_a, const Vector3f& vec_b, const Vector3f& vec_c);

#endif
