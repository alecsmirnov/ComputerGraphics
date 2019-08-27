#ifndef FIGURE_H
#define FIGURE_H

#include <array>
#include <GL/freeglut.h>

#include "Ray.h"
#include "Color.h"

static constexpr GLfloat FRAME_COEF		   = 1.02f;
static constexpr Color   INNER_FRAME_COLOR = ColorElem::WHITE;
static constexpr Color   OUTER_FRAME_COLOR = ColorElem::RED;
static constexpr GLfloat INNER_FRAME_WIDTH = 1.0f;
static constexpr GLfloat OUTER_FRAME_WIDTH = 3.0f;

constexpr GLubyte MATERIAL_SIZE = 4;

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

	virtual void draw() const = 0;
	virtual void drawFrame() const = 0;
	virtual bool isHit(Ray ray, std::vector<Collision>& collisions) const = 0;
	virtual bool isSimpleHit(Ray ray) const = 0;
};

#endif