#ifndef FIGURE_H
#define FIGURE_H

#include <array>
#include <GL/freeglut.h>

#include "Ray.h"

constexpr GLubyte MATERIAL_SIZE = 4;

enum class ObjectType {
	SPHERE,
	PRISM,
	PLANE,
};

struct Material {
	std::array<GLfloat, MATERIAL_SIZE> ambient;
	std::array<GLfloat, MATERIAL_SIZE> diffuse;
	std::array<GLfloat, MATERIAL_SIZE> specular;
	std::array<GLfloat, MATERIAL_SIZE> emission;
	GLfloat shine;
};

class Figure {
public:
	virtual void setMaterial(Material material) = 0;
	virtual void setVisibility(bool visibility) = 0;

	virtual Material getMaterial() const = 0;
	virtual bool getVisibility() const = 0;

	virtual ObjectType getType() const = 0;

	virtual void draw() = 0;
	virtual void drawFrame() = 0;
	virtual bool isHit(Ray ray, std::vector<Collision>& collisions) = 0;
	virtual bool isSimpleHit(Ray ray) = 0;
};

#endif