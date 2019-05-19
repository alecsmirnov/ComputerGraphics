#ifndef HEXAGONRASTERISATION_H
#define HEXAGONRASTERISATION_H

#include <vector>
#include <GL/freeglut.h>

#include "Hexagon.h"

// Порядок закраски растеризованного шестиугольника
enum class Orientation {	
	COLINEAR,
	CLOCKWISE,
	COUNTERCLOCKWISE,
};

// Растеризация шестиугольника
class HexagonRasterisation {
private:
	// Структура пикселя растрового поля
	struct Pixel {
		Point<GLint> A;
		Point<GLint> C;
	};

	// Структура растеризованного шестиугольника
	struct RasteredHexagon {
		RasteredHexagon(std::vector<Point<GLint>> coords, Color color);

		std::vector<Point<GLint>> coords;
		std::vector<Point<GLint>> glut_coords;
		Color color;
	};

public:
	HexagonRasterisation(GLint width = 640, GLint height = 480);

	// Установить шестиугольники на поле
	void setHexes(std::vector<Hexagon> new_hexes);

	// Изменить размер растрового поля
	void resizeField(GLint width, GLint height);
	// Вывести растровое поле
	void printField(std::vector<RasteredHexagon>::size_type current_hexagon) const;

	// Увеличить сетку растрового поля
	void increaseGrid();
	// Уменьшить сетку растрового поля
	void decreaseGrid();

	// Показать контур оригинальной фигуры
	void showGlutCountour();
	// Скрыть контур оригинальной фигуры
	void hideGlutCountour();

	~HexagonRasterisation();

private:
	// Инициализировать сетку поля
	void initField(uint8_t cell_size);

	// Вывести шестиугольник на поле
	void printHexagon(std::vector<RasteredHexagon>::size_type hexagon_num, Color border_color) const;
	// Вывести сетку поля
	void printGrid() const;

	// Очистить шестиугольники на поле
	void clearHexes();
	// Очистить поле
	void clearField();

	// Проверка попадания точки шестиугольника в пиксель
	bool isHit(Point<GLint> point, Pixel pixel) const;
	// Проверка точки на принадлежность полю
	bool isFieldCell(Point<GLint> point) const;

	// Получить точку границы поля
	Point<GLint> getBorderPoint(Point<GLint> point) const;
	// Преобразовать шестиугольник из Glut в растр
	RasteredHexagon fromGlutToRaster(const Hexagon& hexagon) const;

	// Закрасить пиксель
	void setPixel(Point<GLint> point, Color color) const;
	// Нарисовать линию пикселей
	void drawLine(Point<GLint> A, Point<GLint> B, Color color) const;

	// Найти маскимальную координату квадрата, описывающего шестиугольник
	Point<GLint> findMaxCoord(const RasteredHexagon& hexagon) const;
	// Найти минимальную координату квадрата, описывающего шестиугольник
	Point<GLint> findMinCoord(const RasteredHexagon& hexagon) const;

	// Проверка точек на попадание в сегмент шестиугольника
	bool onSegment(Point<GLint> p, Point<GLint> q, Point<GLint> r) const;
	// Получить ориентацию закраски сегмента
	Orientation getOrientation(Point<GLint> p, Point<GLint> q, Point<GLint> r) const;
	// Проверить точку на пересечение сегмента
	bool isSegmentIntersect(Point<GLint> p_1, Point<GLint> q_1, Point<GLint> p_2, Point<GLint> q_2) const;

	// Проверить, принадлежит ли точка шестиугольнику
	bool isInside(Point<GLint> point, const RasteredHexagon& hexagon) const;

public:
	GLint width;
	GLint height;

	Pixel** field;
	Point<GLint> field_size;

	uint8_t cell_size;

	std::vector<RasteredHexagon> hexes;

	bool glut_contour;
};

#endif