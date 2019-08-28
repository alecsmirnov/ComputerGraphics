#include "Line.h"

// Получить цвет линии
Color Line::getColor() const { 
	return color; 
}

// Получить ширину линии
GLfloat Line::getWidth() const { 
	return width; 
}

// Получить тип линии
LineType Line::getType() const { 
	return type; 
}

// Задать цвет линии
void Line::setColor(Color color) { 
	this->color = color; 
}

// Задать цвет линии
void Line::setColor(GLfloat R, GLfloat G, GLfloat B) { 
	color = {R, G, B}; 
}

// Задать ширину линии
void Line::setWidth(GLfloat width) { 
	this->width = width; 
}

// Задать тип линии
void Line::setType(LineType type) { 
	this->type = type; 
}

// Добавить элемент в конец линии
void Line::pushBack(Point point) {
	coords.push_back(point); 
}

// Удалить последний элемент линии
void Line::popBack() {
	coords.pop_back();
}

// Удалить точку
void Line::deletePoint(std::vector<Point>::size_type point_num) {
	if (1 < coords.size())
		coords.erase(coords.begin() + point_num);
	else
		clear();
}

// Очистить линию
void Line::clear() {
	std::vector<Point>().swap(coords);
}

Line::~Line() { 
	clear();
}

// Формирование начального итератора линии
Line::iterator Line::begin() noexcept { 
	return coords.begin(); 
}

// Формирование начального константного итератора линии
Line::const_iterator Line::cbegin() const noexcept { 
	return coords.cbegin(); 
}

// Формирование конечного итератора линии
Line::iterator Line::end() noexcept { 
	return coords.end(); 
}

// Формирование конечного константного итератора линии
Line::const_iterator Line::cend() const noexcept { 
	return coords.cend();
}

// Получить размер линии
std::vector<POINT>::size_type Line::size() const {
	return coords.size(); 
};

// Перегрузка операции взятия из скобок
Point& Line::operator[](std::vector<Point>::size_type i) {
	return coords[i]; 
}

// Перегрузка операции присвоения скобкам
Point Line::operator[](std::vector<Point>::size_type i) const {
	return coords[i]; 
}