#ifndef HEXAGON_H
#define HEXAGON_H

#include <vector>
#include <cstdint>
#include <GL/freeglut.h>

#include "Color.h"
#include "Point.h"

// Класс шестиугольника 
class Hexagon {
public:
	Hexagon() = default;
	Hexagon(Point<GLint> center, Point<GLint> vertex);
	Hexagon(Point<GLint> center, Point<GLint> vertex, Color color);
	Hexagon(std::vector<Point<GLint>> coords, Color color);

	// Установить координату сдвига
	void setTranslateCoords(Point<GLint> translate_coords);
	// Установить угол наклона
	void setAngle(GLfloat angle);
	// Установить масштаб
	void setScale(GLfloat scale);
	// Установить цвет
	void setColor(Color color);
	// Установить текстуры
	void setTexture(GLuint texture);

	// Получить координаты
	std::vector<Point<GLint>> getCoords() const;
	// Получить координату сдвига
	Point<GLint> getTranslateCoords() const;
	// Получить угол наклона
	GLfloat getAngle() const;
	// Получить масштаб
	GLfloat getScale() const;
	// Получить координату центра
	Point<GLint> getCenter() const;
	// Получить цвет
	Color getColor() const;
	// Получить текстуры
	GLuint getTexture() const;
	
	~Hexagon();

private:
	// Расчитать координаты по центральной точки и точке радиуса
	void calcCoordinates(Point<GLint> center, Point<GLint> vertex);

public:
	using hexagon_t = std::vector<Point<GLint>>;
	using iterator = hexagon_t::iterator;
	using const_iterator = hexagon_t::const_iterator;

	// Формирование начального итератора шестиугольника
	iterator begin() noexcept;
	// Формирование начального константного итератора шестиугольника
	const_iterator cbegin() const noexcept;
	// Формирование конечного итератора шестиугольника
	iterator end() noexcept;
	// Формирование конечного константного итератора шестиугольника
	const_iterator cend() const noexcept;

	// Получить размер шестиугольника
	std::uint8_t size() const;
	// Перегрузка операции взятия из скобок
	Point<GLint>& operator[](std::uint8_t i);
	// Перегрузка операции присвоения скобкам
	Point<GLint> operator[](std::uint8_t i) const;

	static constexpr uint8_t vertex_count = 6;

private:
	hexagon_t coords;

	Point<GLint> translate_coords;
	GLfloat angle;
	GLfloat scale;

	Color color;
	GLuint texture;
};

#endif