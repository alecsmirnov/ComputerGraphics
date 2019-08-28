#ifndef LINE_H
#define LINE_H

#include <vector>
#include <GL/glut.h>

#include "Color.h"

// Класс, отвечающий за хранение координат двумерной точки
struct Point {
	GLint x;
	GLint y;
};

// Тип, представляющий возможные варианты линии
enum class LineType : GLushort {
	SOLID   = 0xFFFF,			// Сплошная
	DOT     = 0x0101,			// Пунктирная
	DASH    = 0x00F0,			// Тире
	DOTDASH = 0x1C47			// Штрихпунктирная
};

// Класс, представляющий контейнер, линию из точек и её характеристики
class Line {
public:
	Line() : color(ColorElem::BLACK), width(1), type(LineType::SOLID) {}

	// Получить цвет линии
	Color getColor() const;
	// Получить ширину линии
	GLfloat getWidth() const;
	// Получить тип линии
	LineType getType() const;

	// Задать цвет линии
	void setColor(Color color);
	void setColor(GLfloat R, GLfloat G, GLfloat B);
	// Задать ширину линии
	void setWidth(GLfloat width);
	// Задать тип линии
	void setType(LineType type);

	// Добавить элемент в конец линии
	void pushBack(Point point);
	// Удалить последний элемент линии
	void popBack();

	// Удалить точку
	void deletePoint(std::vector<POINT>::size_type point_num);

	// Очистить линию
	void clear();

	~Line();

public:
	// Формирование STL итераторов линии
	using line_t = std::vector<Point>;
	using iterator = line_t::iterator;
	using const_iterator = line_t::const_iterator;
	
	// Формирование начального итератора линии
	iterator begin() noexcept;
	// Формирование начального константного итератора линии
	const_iterator cbegin() const noexcept;
	// Формирование конечного итератора линии
	iterator end() noexcept;
	// Формирование конечного константного итератора линии
	const_iterator cend() const noexcept;

	// Получить размер линии
	std::vector<Point>::size_type size() const;
	// Перегрузка операции взятия из скобок
	Point& operator[](std::vector<Point>::size_type i);
	// Перегрузка операции присвоения скобкам
	Point operator[](std::vector<Point>::size_type i) const;

private:
	line_t coords;

	Color color;
	GLfloat width;
	LineType type;
};

#endif