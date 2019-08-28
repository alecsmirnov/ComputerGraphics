#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Color.h"
#include "Vector3f.h"

static constexpr GLfloat LIGHT_FRAME_COEF		 = 1.02f;
static constexpr Color   LIGHT_INNER_FRAME_COLOR = ColorElem::WHITE;
static constexpr Color   LIGHT_OUTER_FRAME_COLOR = ColorElem::DARK_BLUE;
static constexpr GLfloat LIGHT_INNER_FRAME_WIDTH = 1.0f;
static constexpr GLfloat LIGHT_OUTER_FRAME_WIDTH = 3.0f;

class LightSource {
public:
	LightSource() = default;
	LightSource(const Vector3f& position, Color color);

	void setPosition(const Vector3f& position);
	void setColor(Color color);

	Vector3f getPosition() const;
	Color getColor() const;

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void draw() const;
	void drawFrame() const;

private:
	Vector3f position;
	Color color;
};

#endif
