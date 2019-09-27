#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
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
	
	static constexpr std::uint8_t size = 8;
}

static inline Color getColorByNum(std::uint8_t num) {
	auto color = ColorElem::BLACK;

	switch (num) {
		case 0: color = ColorElem::WHITE;     break;
		case 1: color = ColorElem::RED;       break;
		case 2: color = ColorElem::ORANGE;    break;
		case 3: color = ColorElem::YELLOW;    break;
		case 4: color = ColorElem::GREEN;     break;
		case 5: color = ColorElem::BLUE;      break;
		case 6: color = ColorElem::DARK_BLUE; break;
		case 7: color = ColorElem::VIOLET;    break;
		default:;
	}

	return color;
}

#endif