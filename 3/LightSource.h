#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Figure.h"

class LightSource : public Figure {
public:
	LightSource() = default;
	LightSource(const Vector3f& position, Color color);

	GLuint getTexture() const;

	void draw() const;
	void drawFrame() const;
};

#endif
