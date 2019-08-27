#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Color.h"
#include "Vector3f.h"

class LightSource {
public:
	LightSource() = default;
	LightSource(const Vector3f& position, Color color);

	void setPosition(const Vector3f& position);
	void setColor(Color color);

	Vector3f getPosition() const;
	Color getColor() const;

	void draw();

	void rotate(GLfloat angle, const Vector3f& rotate_front);

private:
	Vector3f position;
	Color color;
};

#endif
