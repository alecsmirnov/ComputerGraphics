#ifndef PRISM_H
#define PRISM_H

#include "Figure.h"
#include "Color.h"

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
	ObjectType getType() const;

	void draw();
	void drawFrame();
	bool isHit(Ray ray, std::vector<Collision>& collisions);
	bool isSimpleHit(Ray ray);

private:
	bool isInPlane(const std::array<GLfloat, 4>& x, GLfloat xp, const std::array<GLfloat, 4>& y, GLfloat yp);
	bool isPlaneHit(Ray ray, GLbyte ver1, GLbyte ver2, GLbyte ver3, GLbyte ver4, Vector3f& N, GLfloat& time);

private:
	static constexpr ObjectType type = ObjectType::PRISM;

	std::array<Vector3f, vertices> coords;
	Vector3f center;

	Color color;
	Color frame_color;
	Material material;

	bool visibility;
};

#endif