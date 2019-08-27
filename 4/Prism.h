#ifndef PRISM_H
#define PRISM_H

#include "Figure.h"

class Prism : public Figure {
public:
	static constexpr GLubyte vertices = 8;

public:
	Prism() = default;
	Prism(const std::array<Vector3f, vertices>& coords, const Vector3f& center, Color color, Material material);

	void setMaterial(Material material);
	void setVisibility(bool visibility);

	Material getMaterial() const;
	bool getVisibility() const;

	void draw() const;
	void drawFrame() const;
	bool isHit(Ray ray, std::vector<Collision>& collisions) const;
	bool isSimpleHit(Ray ray) const;

private:
	bool isInPlane(const std::array<GLfloat, 4>& x, GLfloat xp, const std::array<GLfloat, 4>& y, GLfloat yp) const;
	bool isPlaneHit(Ray ray, GLbyte ver1, GLbyte ver2, GLbyte ver3, GLbyte ver4, Vector3f& N, GLfloat& time) const;

private:
	std::array<Vector3f, vertices> coords;
	Vector3f center;

	Color color;
	Material material;

	bool visibility;
};

#endif