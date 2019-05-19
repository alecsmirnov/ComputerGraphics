#ifndef POINT_H
#define POINT_H

#include <GL/glut.h>

// Класс, отвечающий за хранение координат двумерной точки
struct Point {
	Point() = default;
	Point(GLint x, GLint y) { this->x = x; this->y = y; }

	class {		// Setter/Getter в стиле C# для поля x (обращение к полю через .)
	public:
		// Установить значение x типа GLint
		GLint& operator=(const GLint& x) { return this->x = x; }
		// Взять значение x типа GLint
		operator GLint() const { return x; }
	private:
		GLint x;
	} x;

	class {		// Setter/Getter в стиле C# для поля y (обращение к полю через .)
	public:
		// Установить значение y типа GLint
		GLint& operator=(const GLint& y) { return this->y = y; }
		// Взять значение y типа GLint
		operator GLint() const { return y; }
	private:
		GLint y;
	} y;
};

#endif