#ifndef FIGURE_H
#define FIGURE_H

#include "Vector3f.h"
#include "Color.h"

static constexpr GLfloat FRAME_COEF		   = 1.02f;
static constexpr Color   INNER_FRAME_COLOR = ColorElem::WHITE;
static constexpr Color   OUTER_FRAME_COLOR = ColorElem::RED;
static constexpr GLfloat INNER_FRAME_WIDTH = 1.0f;
static constexpr GLfloat OUTER_FRAME_WIDTH = 3.0f;

static constexpr GLfloat MOVE_SPEED = 0.5f;

class Figure {
public:
	virtual void setTexture(GLuint texture);

	Vector3f getPosition() const;
	Color getColor() const;
	virtual GLuint getTexture() const;

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void rotate(const Vector3f& center, const Vector3f& up);

	virtual void draw() const = 0;
	virtual void drawFrame() const = 0;

protected:
	Vector3f position;

	Color color;

	GLuint texture = 0;
};

#endif