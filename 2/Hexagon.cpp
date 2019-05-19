#include "Hexagon.h"

#include <cmath>

static constexpr std::uint8_t N = 6;

Hexagon::Hexagon(Point<GLint> center, Point<GLint> vertex) {
	calcCoordinates(center, vertex);
	angle = 0.0f;
	translate_coords = Point<GLint>(0, 0);
	scale = 1.0f;
	texture = 0;
}

Hexagon::Hexagon(Point<GLint> center, Point<GLint> vertex, Color color) {
	calcCoordinates(center, vertex);
	angle = 0.0f;
	translate_coords = Point(0, 0);
	scale = 1.0f;

	this->color = color;
	texture = 0;
}

Hexagon::Hexagon(std::vector<Point<GLint>> coords, Color color) {
	this->coords = coords;
	angle = 0.0f;
	translate_coords = Point(0, 0);
	scale = 1.0f;

	this->color = color;
	texture = 0;
}

// Установить координату сдвига
void Hexagon::setTranslateCoords(Point<GLint> translate_coords) {
	this->translate_coords = translate_coords;
}

// Установить угол наклона
void Hexagon::setAngle(GLfloat angle) {
	this->angle = angle;
}

// Установить масштаб
void Hexagon::setScale(GLfloat scale) {
	this->scale = scale;
}

// Установить цвет
void Hexagon::setColor(Color color) {
	this->color = color;
}

// Установить текстуры
void Hexagon::setTexture(GLuint texture) {
	this->texture = texture;
}

// Получить координаты
std::vector<Point<GLint>> Hexagon::getCoords() const {
	return coords;
}

// Получить координату сдвига
Point<GLint> Hexagon::getTranslateCoords() const {
	return translate_coords;
}

// Получить угол наклона
GLfloat Hexagon::getAngle() const {
	return angle;
}

// Получить масштаб
GLfloat Hexagon::getScale() const {
	return scale;
}

// Получить координату центра
Point<GLint> Hexagon::getCenter() const {
	return Point((coords[0].x + coords[3].x) / 2, (coords[0].y + coords[3].y) / 2);
}

// Получить цвет
Color Hexagon::getColor() const {
	return color;
}

// Получить текстуры
GLuint Hexagon::getTexture() const {
	return texture;
}

Hexagon::~Hexagon() {
	std::vector<Point<GLint>>().swap(coords);
}

// Расчитать координаты по центральной точки и точке радиуса
void Hexagon::calcCoordinates(Point<GLint> center, Point<GLint> vertex) {
	const auto PI = std::acos(-1);

	double R = std::sqrt(std::pow((vertex.x - center.x), 2) + std::pow((vertex.y - center.y), 2));
	double vertex_alpha = std::atan(static_cast<double>(vertex.x - center.x) / (vertex.y - center.y));

	for (uint8_t i = 0; i != vertex_count; ++i) {
		double angle = 2 * PI * i / N;

		double x = center.x + R * std::sin(vertex_alpha + angle);
		double y = center.y + R * std::cos(vertex_alpha + angle);

		coords.push_back(Point(static_cast<GLint>(x), static_cast<GLint>(y)));
	}
}

// Формирование начального итератора линии
Hexagon::iterator Hexagon::begin() noexcept {
	return coords.begin();
}

// Формирование начального константного итератора линии
Hexagon::const_iterator Hexagon::cbegin() const noexcept {
	return coords.cbegin();
}

// Формирование конечного итератора линии
Hexagon::iterator Hexagon::end() noexcept {
	return coords.end();
}

// Формирование конечного константного итератора линии
Hexagon::const_iterator Hexagon::cend() const noexcept {
	return coords.cend();
}
// Получить размер шестиугольника
std::uint8_t Hexagon::size() const {
	return coords.size() == 0 ? 0 : vertex_count;
};

// Перегрузка операции взятия из скобок
Point<GLint>& Hexagon::operator[](std::uint8_t i) {
	return coords[i];
}

// Перегрузка операции присвоения скобкам
Point<GLint> Hexagon::operator[](std::uint8_t i) const {
	return coords[i];
}