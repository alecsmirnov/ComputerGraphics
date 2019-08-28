#ifndef COLOR_H
#define COLOR_H

#include <GL/freeglut.h>

struct Color {
	GLfloat R;
	GLfloat G;
	GLfloat B;
};

namespace ColorElem {
	static constexpr Color WHITE	 = {1.0f, 1.0f, 1.0f};
	static constexpr Color RED		 = {1.0f, 0.0f, 0.0f};
	static constexpr Color ORANGE	 = {1.0f, 0.5f, 0.0f};
	static constexpr Color YELLOW	 = {1.0f, 1.0f, 0.0f};
	static constexpr Color GREEN	 = {0.0f, 1.0f, 0.0f};
	static constexpr Color BLUE		 = {0.0f, 0.5f, 1.0f};
	static constexpr Color DARK_BLUE = {0.0f, 0.0f, 1.0f};
	static constexpr Color VIOLET	 = {0.5f, 0.5f, 0.5f};
	static constexpr Color BLACK	 = {0.0f, 0.0f, 0.0f};
}

#endif