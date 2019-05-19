#include "HexagonRasterisation.h"

#include <algorithm>

static constexpr uint8_t CELL_SIZE_MIN = 2;		// Минимальный размер клетки поля
static constexpr uint8_t CELL_SIZE_MAX = 20;	// Максимальный размер клетки поля

static constexpr GLfloat LINES_WIDTH = 0.5f;	// Ширина ланий сетки

HexagonRasterisation::RasteredHexagon::RasteredHexagon(std::vector<Point<GLint>> coords, Color color) {
	this->coords = coords;
	this->color = color;

	glut_coords = coords;
}

HexagonRasterisation::HexagonRasterisation(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	cell_size = CELL_SIZE_MIN;
	initField(cell_size);

	glut_contour = false;
}

// Установить шестиугольники на поле
void HexagonRasterisation::setHexes(std::vector<Hexagon> new_hexes) {
	if (!new_hexes.empty()) {
		if (!hexes.empty())
			clearHexes();

		// Перевод координат шестиугольников в растровое представление 
		for (auto hexagon : new_hexes)
			hexes.push_back(fromGlutToRaster(hexagon));

		for (GLint i = 0; i != field_size.x; ++i)
			for (GLint j = 0; j != field_size.y; ++j)
				for (GLint k = 0; k != hexes.size(); ++k)
					for (GLint m = 0; m != Hexagon::vertex_count; ++m)
						if (isHit(hexes[k].coords[m], field[i][j]))
							hexes[k].coords[m] = { i, j };
	}
}

// Изменить размер растрового поля
void HexagonRasterisation::resizeField(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	clearField();
	initField(cell_size);
}

// Вывести растровое поле
void HexagonRasterisation::printField(std::vector<RasteredHexagon>::size_type current_hexagon) const {
	if (!hexes.empty()) {
		for (std::vector<RasteredHexagon>::size_type i = 0; i != hexes.size(); ++i)
			if (i != current_hexagon)
				printHexagon(i, getColor(ColorElem::BLACK));

		printHexagon(current_hexagon, getColor(ColorElem::RED));
	}

	printGrid();
}

// Увеличить сетку растрового поля
void HexagonRasterisation::increaseGrid() {
	if (cell_size != CELL_SIZE_MAX)
		++cell_size;

	initField(cell_size);
}

// Уменьшить сетку растрового поля
void HexagonRasterisation::decreaseGrid() {
	if (cell_size != CELL_SIZE_MIN)
		--cell_size;

	initField(cell_size);
}

// Показать контур оригинальной фигуры
void HexagonRasterisation::showGlutCountour() {
	glut_contour = true;
}

// Скрыть контур оригинальной фигуры
void HexagonRasterisation::hideGlutCountour() {
	glut_contour = false;
}

HexagonRasterisation::~HexagonRasterisation() {
	clearField();
}

// Инициализировать сетку поля
void HexagonRasterisation::initField(uint8_t cell_size) {
	this->cell_size = cell_size;

	field_size.x = width  / cell_size + 1;
	field_size.y = height / cell_size + 1;

	field = new Pixel*[field_size.x];
	for (GLint i = 0; i != field_size.x; ++i)
		field[i] = new Pixel[field_size.y];

	for (GLint i = 0; i != field_size.x; ++i)
		for (GLint j = 0; j != field_size.y; ++j) {
			field[i][j].A = { (i - 1) * cell_size, (j - 1) * cell_size };
			field[i][j].C = {  i * cell_size,		j * cell_size };
		}
}

// Вывести шестиугольник на поле
void HexagonRasterisation::printHexagon(std::vector<RasteredHexagon>::size_type hexagon_num, Color border_color) const {
	auto point_max = findMaxCoord(hexes[hexagon_num]);
	auto point_min = findMinCoord(hexes[hexagon_num]);

	for (GLint i = point_min.x; i != point_max.x; ++i)
		for (GLint j = point_min.y; j != point_max.y; ++j)
			if (isInside({ i, j }, hexes[hexagon_num]))
				setPixel({ i, j }, hexes[hexagon_num].color);

	for (uint8_t i = 0; i != Hexagon::vertex_count; ++i) {
		uint8_t i_plus = (i + 1) % Hexagon::vertex_count;

		drawLine(hexes[hexagon_num].coords[i], hexes[hexagon_num].coords[i_plus], border_color);
	}

	if (glut_contour) {
		glColor3ub(0, 191, 255);

		glBegin(GL_LINE_LOOP);
		for (uint8_t i = 0; i != Hexagon::vertex_count; ++i)
			glVertex2i(hexes[hexagon_num].glut_coords[i].x, hexes[hexagon_num].glut_coords[i].y);
		glEnd();
	}
}

// Вывести сетку поля
void HexagonRasterisation::printGrid() const {
	glLineWidth(LINES_WIDTH);
	glColor3ub(170, 170, 170);

	glBegin(GL_LINES);
	for (GLint i = 0; i != field_size.x; ++i) {
		glVertex2i(i * cell_size, 0);
		glVertex2i(i * cell_size, height);
	}

	for (GLint j = 0; j != field_size.y; ++j) {
		glVertex2i(0, j * cell_size);
		glVertex2i(width, j * cell_size);
	}
	glEnd();
}

// Очистить шестиугольники на поле
void HexagonRasterisation::clearHexes() {
	std::vector<RasteredHexagon>().swap(hexes);
}

// Очистить поле
void HexagonRasterisation::clearField() {
	for (GLint i = 0; i != field_size.x; ++i)
		delete[] field[i];

	delete[] field;
}

// Проверка попадания точки шестиугольника в пиксель
bool HexagonRasterisation::isHit(Point<GLint> point, Pixel pixel) const {
	return pixel.A.x < point.x && pixel.A.y < point.y &&		
		   point.x <= pixel.C.x && point.y <= pixel.C.y;
}

// Проверка точки на принадлежность полю
bool HexagonRasterisation::isFieldCell(Point<GLint> point) const {
	return 0 <= point.x && point.x < width &&
		   0 <= point.y && point.y < height;
}

// Получить точку границы поля
Point<GLint> HexagonRasterisation::getBorderPoint(Point<GLint> point) const {
	Point<GLint> border_point = { point.x, point.y };

	if (point.x < 0)
		border_point.x = 1;

	if (width <= point.x)
		border_point.x = width - 1;

	if (point.y < 0)
		border_point.y = 1;

	if (height <= point.y)
		border_point.y = height - 1;

	return border_point;
}

// Преобразовать шестиугольник из Glut в растр
HexagonRasterisation::RasteredHexagon HexagonRasterisation::fromGlutToRaster(const Hexagon& hexagon) const {
	auto coords = hexagon.getCoords();

	auto translate_coords = hexagon.getTranslateCoords();
	for (uint8_t i = 0; i != Hexagon::vertex_count; ++i)
		coords[i] = { coords[i].x + translate_coords.x,
					  coords[i].y + translate_coords.y };

	const auto PI = std::acos(-1);
	auto alpha = hexagon.getAngle() * PI / 180;

	Point<GLint> center = { hexagon.getCenter().x + translate_coords.x,
							hexagon.getCenter().y + translate_coords.y };

	for (uint8_t i = 0; i != Hexagon::vertex_count; ++i) {
		auto x = static_cast<GLint>((coords[i].x - center.x) *
				 std::cos(alpha) - (coords[i].y - center.y) * std::sin(alpha) + center.x);
		auto y = static_cast<GLint>((coords[i].x - center.x) *
				 std::sin(alpha) + (coords[i].y - center.y) * std::cos(alpha) + center.y);

		coords[i] = { x , y };
	}

	for (uint8_t i = 0; i != Hexagon::vertex_count; ++i)
		if (!isFieldCell(coords[i]))
			coords[i] = getBorderPoint(coords[i]);

	return RasteredHexagon(coords, hexagon.getColor());
}

// Закрасить пиксель
void HexagonRasterisation::setPixel(Point<GLint> point, Color color) const {
	glColor3ub(color.R, color.G, color.B);

	glRecti(field[point.x][point.y].A.x, field[point.x][point.y].A.y,
			field[point.x][point.y].C.x, field[point.x][point.y].C.y);
}

// Нарисовать линию пикселей
void HexagonRasterisation::drawLine(Point<GLint> A, Point<GLint> B, Color color) const {
	GLint delta_x = std::abs(B.x - A.x);
	GLint delta_y = std::abs(B.y - A.y);

	GLint sign_x = A.x < B.x ? 1 : -1;
	GLint sign_y = A.y < B.y ? 1 : -1;

	GLint error = delta_x - delta_y;

	setPixel(B, color);

	while (A.x != B.x || A.y != B.y) {
		setPixel(A, color);

		GLint error_2 = error * 2;

		if (error_2 > -delta_y) {
			error -= delta_y;
			A.x = A.x + sign_x;
		}

		if (error_2 < delta_x) {
			error += delta_x;
			A.y = A.y + sign_y;
		}
	}
}

// Найти маскимальную координату квадрата, описывающего шестиугольник
Point<GLint> HexagonRasterisation::findMaxCoord(const RasteredHexagon& hexagon) const {
	Point<GLint> max = { 0, 0 };

	for (auto point : hexagon.coords) {
		if (max.x < point.x)
			max.x = point.x;

		if (max.y < point.y)
			max.y = point.y;
	}

	return max;
}

// Найти минимальную координату квадрата, описывающего шестиугольник
Point<GLint> HexagonRasterisation::findMinCoord(const RasteredHexagon& hexagon) const {
	Point<GLint> min = { field_size.x, field_size.y };

	for (auto point : hexagon.coords) {
		if (point.x < min.x)
			min.x = point.x;

		if (point.y < min.y)
			min.y = point.y;
	}

	return { min.x - 1, min.y + 1 };
}

// Проверка точек на попадание в сегмент шестиугольника
bool HexagonRasterisation::onSegment(Point<GLint> p, Point<GLint> q, Point<GLint> r) const {
	auto result = false;

	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		result = true;

	return result;
}

// Получить ориентацию закраски сегмента
Orientation HexagonRasterisation::getOrientation(Point<GLint> p, Point<GLint> q, Point<GLint> r) const {
	auto result = Orientation::COLINEAR;

	if (GLint val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y))
		result = 0 < val ? Orientation::CLOCKWISE : Orientation::COUNTERCLOCKWISE;

	return result;
}

// Проверить точку на пересечение сегмента
bool HexagonRasterisation::isSegmentIntersect(Point<GLint> p_1, Point<GLint> q_1, Point<GLint> p_2, Point<GLint> q_2) const {
	auto orent_1 = getOrientation(p_1, q_1, p_2);
	auto orent_2 = getOrientation(p_1, q_1, q_2);
	auto orent_3 = getOrientation(p_2, q_2, p_1);
	auto orent_4 = getOrientation(p_2, q_2, q_1);

	if (orent_1 != orent_2 && orent_3 != orent_4)
		return true;

	if (orent_1 == Orientation::COLINEAR && onSegment(p_1, p_2, q_1))
		return true;
	if (orent_2 == Orientation::COLINEAR && onSegment(p_1, q_2, q_1))
		return true;
	if (orent_3 == Orientation::COLINEAR && onSegment(p_2, p_1, q_2))
		return true;
	if (orent_4 == Orientation::COLINEAR && onSegment(p_2, q_1, q_2))
		return true;

	return false;
}

// Проверить, принадлежит ли точка шестиугольнику
bool HexagonRasterisation::isInside(Point<GLint> point, const RasteredHexagon& hexagon) const {
	GLint count = 0;

	constexpr GLint INF_X = 100000;
	Point<GLint> extreme_point = { INF_X, point.y };

	for (uint8_t i = 0; i != Hexagon::vertex_count; ++i) {
		uint8_t next = (i + 1) % Hexagon::vertex_count;

		if (isSegmentIntersect(hexagon.coords[i], hexagon.coords[next], point, extreme_point)) {
			if (getOrientation(hexagon.coords[i], point, hexagon.coords[next]) == Orientation::COLINEAR)
				return onSegment(hexagon.coords[i], point, hexagon.coords[next]);

			if (point.y != hexagon.coords[i].y)
				count++;
		}
	}

	return count % 2 == 1;
}