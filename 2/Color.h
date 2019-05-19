#ifndef COLOR_H
#define COLOR_H

#include <GL/freeglut.h>

// Стандартные цвета для цветовой палитры
enum class ColorElem {
	WHITE,
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE,
	DARK_BLUE,
	VIOLET,
	BLACK
};

// Класс, отвечающий за представление цвета в палитре RGB
struct Color {
	Color() = default;
	Color(GLubyte R, GLubyte G, GLubyte B) { this->R = R; this->G = G, this->B = B; }

	class {		// Setter/Getter в стиле C# для поля R (обращение к полю через .)
	public:
		// Установить значение R типа GLubyte
		GLubyte& operator=(const GLubyte& R) { return this->R = R; }
		// Взять значение R типа GLubyte
		operator GLubyte() const { return R; }
	private:
		GLubyte R;
	} R;

	class {		// Setter/Getter в стиле C# для поля G (обращение к полю через .)
	public:
		// Установить значение G типа GLubyte
		GLubyte& operator=(const GLubyte& G) { return this->G = G; }
		// Взять значение G типа GLubyte
		operator GLubyte() const { return G; }
	private:
		GLubyte G;
	} G;

	class {		// Setter/Getter в стиле C# для поля B (обращение к полю через .)
	public:
		// Установить значение B типа GLubyte
		GLubyte& operator=(const GLubyte& B) { return this->B = B; }
		// Взять значение B типа GLubyte
		operator GLubyte() const { return B; }
	private:
		GLubyte B;
	} B;
};

// Функция получения стандартного цвета по его имени
static inline Color getColor(ColorElem color_elem) {
	Color color;

	switch (color_elem) {
		case ColorElem::WHITE:	   color = Color(255, 255, 255); break;
		case ColorElem::RED:       color = Color(255, 0,   0);	 break;
		case ColorElem::ORANGE:    color = Color(255, 165, 0);   break;
		case ColorElem::YELLOW:    color = Color(255, 255, 0);   break;
		case ColorElem::GREEN:	   color = Color(0,   128, 0);	 break;
		case ColorElem::BLUE:	   color = Color(30,  144, 255); break;
		case ColorElem::DARK_BLUE: color = Color(0,   0,   139); break;
		case ColorElem::VIOLET:    color = Color(128, 0,   128); break;
		case ColorElem::BLACK:	   color = Color(0,   0,   0);	 break;
	}

	return color;
}

#endif