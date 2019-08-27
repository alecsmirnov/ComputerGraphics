#include "Ray.h"

#include <array>
#include <cmath>

using Matrix4f = std::array<std::array<GLfloat, 4>, 4>;
using Vector4f = std::array<GLfloat, 4>;

static Matrix4f makeTransformMatrix(const Vector3f& shift, const Vector3f& scale) {
	return {scale.getX(), 0.0f,			0.0f,		  shift.getX(),
			0.0f,		  scale.getY(), 0.0f,		  shift.getY(),
			0.0f,		  0.0f,			scale.getZ(), shift.getZ(),
			0.0f,		  0.0f,			0.0f,		  1.0f};
}

static void swap(Matrix4f& matrix, Vector4f& vec, GLubyte a, GLubyte b) {
	std::swap(vec[a], vec[b]);

	for (GLubyte i = 0; i != matrix.size(); ++i)
		std::swap(matrix[a][i], matrix[b][i]);
}

static void transform(Matrix4f& matrix, Vector4f& vec, GLubyte col) {
	GLfloat max = matrix[col][col];

	GLubyte cur_row = col;
	for (GLubyte i = col + 1; i != matrix.size(); ++i)
		if (std::fabsf(max) < std::fabsf(matrix[i][col])) {
			max = matrix[i][col];
			cur_row = i;
		}

	if (cur_row != col)
		swap(matrix, vec, col, cur_row);
}

static void direct(Matrix4f& matrix, Vector4f& vec) {
	for (GLubyte i = 0; i != matrix.size(); ++i) {
		transform(matrix, vec, i);

		for (GLubyte j = i + 1; j != matrix.size(); ++j) {
			GLfloat coef = -matrix[j][i] / matrix[i][i];

			for (GLubyte k = i; k != matrix.size(); ++k)
				matrix[j][k] += matrix[i][k] * coef;

			vec[j] += vec[i] * coef;
		}
	}
}

static Vector4f gauss(Matrix4f& matrix, Vector4f& pos_vec) {
	Vector4f result;

	direct(matrix, pos_vec);

	for (GLint i = matrix.size() - 1; i >= 0; --i) {
		GLfloat sum = 0.0f;

		for (GLint j = matrix.size() - 1; j > i; --j)
			sum += matrix[i][j] * result[j];

		result[i] = (pos_vec[i] - sum) / matrix[i][i];
	}

	return result;
}

Vector3f getRayPosition(Ray ray, GLfloat time) {
	return ray.position + ray.front * time;
}

Ray transformRay(Ray ray, const Vector3f& shift, const Vector3f& scale) {
	auto matrix = makeTransformMatrix(shift, scale);

	Vector4f point = {ray.position.getX(), ray.position.getY(), ray.position.getZ(), 1.0f};
	Vector4f front = {ray.front.getX(),    ray.front.getY(),    ray.front.getZ(),    0.0f};

	auto point_sol = gauss(matrix, point);
	auto front_sol = gauss(matrix, front);

	auto new_point = Vector3f(point_sol[0], point_sol[1], point_sol[2]);
	auto new_front = Vector3f(front_sol[0], front_sol[1], front_sol[2]);

	return {new_point, new_front, 0};
}