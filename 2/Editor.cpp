#define _CRT_SECURE_NO_WARNINGS
#include "Editor.h"
#include "KeyButtons.h"

#include <cmath>
#include <cctype>

// Ширина контура шестиугольника
static constexpr GLfloat BORDER_WIDTH                 = 0.1f;
// Ширина внешнего контура выбранного шестиугольника
static constexpr GLfloat SELECTED_EXTERN_BORDER_WIDTH = 8.0f;
// Ширина внутреннего контура выбранного шестиугольника
static constexpr GLfloat SELECTED_INTERN_BORDER_WIDTH = 6.0f;

// Ширина внешнего контура указателя
static constexpr GLfloat POINTER_EXTERN_BORDER_WIDTH = 6.0f;
// Ширина внутреннего контура указателя
static constexpr GLfloat POINTER_INTERN_BORDER_WIDTH = 4.0f;

// Угол наклона при повороте шестиугольника
static constexpr GLfloat HEXAGON_ROTATE_DEGREE = 4.0f;
// Шаг перемещения шестиугольника по осям X, Y
static constexpr GLint   HEXAGON_MOVE_STEP	   = 10;
// Фактор изменения масштаба шестиугольника
static constexpr GLfloat HEXAGON_SCALE_FACTOR  = 0.08f;

Editor::Editor(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	current_hexagon = 0;

	smoothing = true;

	color_operation = ColorOperation::NONE;
	presentation_view = PresentationView::GLUT;

	pointer_activity = true;
	pointer_color = getColor(ColorElem::WHITE);

	field = new HexagonRasterisation(width, height);
}

// Добавить текстуру к набору текстур шестиугольников
void Editor::addTexture(const char* image) {
	texture_loader.addTexture(image);
}

// Начать работу редактора
void Editor::start(int* argc, char* argv[]) {
	glutInit(argc, argv);					// Инициализация библиотеки

	glutInitDisplayMode(GLUT_RGB);			// Установка режима отображения 

	glutInitWindowSize(width, height);		// Размер окна
	glutCreateWindow(window_title);			// Создание окна с заголовком

	initTextureLoader();

	glutDisplayFunc(displayEvent);			// Установка функции отображения
	glutReshapeFunc(reshapeEvent);			// Установка функции изменения окна
	glutKeyboardFunc(keyboardEvent);		// Установка функции обработки клавиатуры
	glutMouseFunc(mouseEvent);				// Установка функции обработки мышки

	menuEvent();							// Вызов выпадающего меню

	glutMainLoop();							// Выход в главный цикл
}

Editor::~Editor() {
	std::vector<Hexagon>().swap(hexes);

	delete field;
}

// Инициализация загрузчика текстур
void Editor::initTextureLoader() {
	const auto PI = std::acos(-1);
	const auto angle = PI / Hexagon::vertex_count;

	std::vector<Point<GLfloat>> texture_coords(Hexagon::vertex_count);

	texture_coords[0] = { 0.5f, 1.0f };
	texture_coords[1] = { 0.5f + 0.5f * static_cast<GLfloat>(std::cos(angle)), 0.75f };
	texture_coords[2] = { 0.5f + 0.5f * static_cast<GLfloat>(std::cos(angle)), 0.25f };
	texture_coords[3] = { 0.5f, 0.0f };
	texture_coords[4] = { 0.5f - 0.5f * static_cast<GLfloat>(std::cos(angle)), 0.25f };
	texture_coords[5] = { 0.5f - 0.5f * static_cast<GLfloat>(std::cos(angle)), 0.75f };

	texture_loader.setCoordnates(texture_coords);

	texture_loader.initAddedTextures();

	auto textures = texture_loader.getTextures();
	textures.insert(textures.begin(), 0);

	texture_loader.setTextures(textures);
}

// Вывод на экран
void Editor::displayEvent() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_COLOR_LOGIC_OP);

	switch (color_operation) {
		case ColorOperation::NONE: glLogicOp(GL_COPY); break;
		case ColorOperation::AND:  glLogicOp(GL_AND);  break;
		case ColorOperation::NAND: glLogicOp(GL_NAND); break;
	}

	switch (presentation_view) {
		case PresentationView::GLUT: {
			smoothing ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);

			glEnable(GL_POLYGON_SMOOTH);
			glEnable(GL_POINT_SMOOTH);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			for (std::vector<Hexagon>::size_type i = 0; i != hexes.size(); ++i)
				if (i != current_hexagon)
					drawHexagon(i);

			drawCurrentHexagon();
			drawPointer();

			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);
			glDisable(GL_POINT_SMOOTH);
			break;
		}
		case PresentationView::RASTER: {
			field->setHexes(hexes);
			field->printField(current_hexagon);
			break;
		}
	}

	glDisable(GL_COLOR_LOGIC_OP);

	glFinish();
}

// Обработка сообщений от клавиатуры
void Editor::keyboardEvent(std::uint8_t key, int x, int y) {
	switch (std::tolower(key)) {
		case ONE_BUTTON:   changeObjectColor(ColorElem::WHITE);	    break;
		case TWO_BUTTON:   changeObjectColor(ColorElem::RED);	    break;
		case THREE_BUTTON: changeObjectColor(ColorElem::ORANGE);	break;
		case FOUR_BUTTON:  changeObjectColor(ColorElem::YELLOW);	break;
		case FIVE_BUTTON:  changeObjectColor(ColorElem::GREEN);	    break;
		case SIX_BUTTON:   changeObjectColor(ColorElem::BLUE);	    break;
		case SEVEN_BUTTON: changeObjectColor(ColorElem::DARK_BLUE); break;
		case EIGHT_BUTTON: changeObjectColor(ColorElem::VIOLET);	break;
		case NINE_BUTTON:  changeObjectColor(ColorElem::BLACK);	    break;
		case ZERO_BUTTON:  color_operation = ColorOperation::NONE;  break;
		case MINUS_BUTTON: color_operation = ColorOperation::AND;   break;
		case EQUAL_BUTTON: color_operation = ColorOperation::NAND;  break;
		case Q_BUTTON:	   prevHexagon(); break;
		case E_BUTTON:	   nextHexagon(); break;
		case W_BUTTON:	   moveCurrentHexagonYAxis(HEXAGON_MOVE_STEP);   break;
		case S_BUTTON:	   moveCurrentHexagonYAxis(-HEXAGON_MOVE_STEP);  break;
		case A_BUTTON:	   moveCurrentHexagonXAxis(-HEXAGON_MOVE_STEP);  break;
		case D_BUTTON:	   moveCurrentHexagonXAxis(HEXAGON_MOVE_STEP);   break;
		case Z_BUTTON:	   scaleCurrentHexagon(-HEXAGON_SCALE_FACTOR);   break;
		case X_BUTTON:	   scaleCurrentHexagon(HEXAGON_SCALE_FACTOR);    break;
		case C_BUTTON:	   rotateCurrentHexagon(-HEXAGON_ROTATE_DEGREE); break;
		case V_BUTTON:	   rotateCurrentHexagon(HEXAGON_ROTATE_DEGREE);  break;
		case SPACE_BUTTON: changeCurrentHexagonTexture(); break;
		case ENTER_BUTTON: changePresentationView();	  break;
		case R_BUTTON:	   field->increaseGrid();   break;
		case F_BUTTON:	   field->decreaseGrid();   break;
		case DEL_BUTTON:   deleteCurrentHexagon();  break;
		case ESC_BUTTON:   pointer_activity = true; break;
		case TAB_BUTTON:   smoothing = !smoothing;  break;
		default:; 
	}

	glutPostRedisplay();
}

// Изменение размеров окна
void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW); 

	if (presentation_view == PresentationView::RASTER)
		field->resizeField(width, height);
}

// Обработка сообщений от мышки
void Editor::mouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON)
			clickEvent(x, height - y);

		glutPostRedisplay();
	}
}

// Обработка левого клика мыши
void Editor::clickEvent(int x, int y) {
	if (pointer_activity) {
		pointer = Point<GLint>(x, y);
		pointer_activity = false;
	}
	else {
		if (pointer.x != x && pointer.y != y) {
			hexes.push_back({ pointer, Point<GLint>(x, y), pointer_color });
			current_hexagon = hexes.size() - 1;
		}

		pointer_activity = true;
	}
}

// Меню изменения текущего шестиугольника
void Editor::hexagonChangeMenu(int task) {
	switch (task) {
		case 0: prevHexagon(); break;	
		case 1: nextHexagon(); break;		
	}

	glutPostRedisplay();
}

// Меню изменения цвета шестиугольника
void Editor::hexagonColorMenu(int task) {
	switch (task) {
		case 0: changeObjectColor(ColorElem::WHITE);	 break;
		case 1: changeObjectColor(ColorElem::RED);		 break;
		case 2: changeObjectColor(ColorElem::ORANGE);	 break;
		case 3: changeObjectColor(ColorElem::YELLOW);	 break;
		case 4: changeObjectColor(ColorElem::GREEN);	 break;
		case 5: changeObjectColor(ColorElem::BLUE);		 break;
		case 6: changeObjectColor(ColorElem::DARK_BLUE); break;
		case 7: changeObjectColor(ColorElem::VIOLET);	 break;
		case 8: changeObjectColor(ColorElem::BLACK);	 break;
	}

	glutPostRedisplay();
}

// Меню изменения масштаба шестиугольника
void Editor::hexagonScaleMenu(int task) {
	switch (task) {
		case 0: scaleCurrentHexagon(HEXAGON_SCALE_FACTOR);  break;
		case 1: scaleCurrentHexagon(-HEXAGON_SCALE_FACTOR); break;
	}

	glutPostRedisplay();
}

// Меню изменения операций с цветом 
void Editor::colorOperationMenu(int task) {
	switch (task) {
		case 0: color_operation = ColorOperation::NONE; break;
		case 1: color_operation = ColorOperation::AND;  break;
		case 2: color_operation = ColorOperation::NAND; break;
	}

	glutPostRedisplay();
}

// Меню работы с растеризацией
void Editor::fieldRasterisationMenu(int task) {
	switch (task) {
		case 0: changePresentationView();  break;
		case 1: field->increaseGrid();	   break;
		case 2: field->decreaseGrid();	   break;
		case 3: field->showGlutCountour(); break;
		case 4: field->hideGlutCountour(); break;
	}

	glutPostRedisplay();
}

// Главное меню
void Editor::mainMenu(int task) {
	switch (task) {	
		case 0: changeCurrentHexagonTexture(); break;
		case 1: deleteCurrentHexagon();		   break;
		case 2:	changePresentationView();	   break;
		case 3: smoothing = !smoothing;		   break;
	}

	glutPostRedisplay();
}

// Обработка меню
void Editor::menuEvent() {
	auto hexagon_change = glutCreateMenu(hexagonChangeMenu);
	glutAddMenuEntry("Prev hexagon", 0);
	glutAddMenuEntry("Next hexagon", 1);

	auto hexagon_color = glutCreateMenu(hexagonColorMenu);
	glutAddMenuEntry("White",     0);
	glutAddMenuEntry("Red",       1);
	glutAddMenuEntry("Orange",    2);
	glutAddMenuEntry("Yellow",	  3);
	glutAddMenuEntry("Green",	  4);
	glutAddMenuEntry("Blue",      5);
	glutAddMenuEntry("Dark blue", 6);
	glutAddMenuEntry("Violet",    7);
	glutAddMenuEntry("Black",     8);

	auto hexagon_scale = glutCreateMenu(hexagonScaleMenu);
	glutAddMenuEntry("Zoom +", 0);
	glutAddMenuEntry("Zoom -", 1);

	auto field_color_operation = glutCreateMenu(colorOperationMenu);
	glutAddMenuEntry("NONE",	0);
	glutAddMenuEntry("AND",		1);
	glutAddMenuEntry("NOT AND", 2);

	auto field_rasterisation = glutCreateMenu(fieldRasterisationMenu);
	glutAddMenuEntry("Rasterisation ON/OFF", 0);
	glutAddMenuEntry("Grid +",				 1);
	glutAddMenuEntry("Grid -",				 2);
	glutAddMenuEntry("Countour ON",			 3);
	glutAddMenuEntry("Countour OFF",		 4);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("Hexagon switching",	 hexagon_change);
	glutAddSubMenu("Hexagon color",		 hexagon_color);
	glutAddSubMenu("Hexagon scale",		 hexagon_scale);
	glutAddMenuEntry("Hexagon texture",	 0);
	glutAddMenuEntry("Remove hexagon",	 1);
	glutAddSubMenu("Color operation",	 field_color_operation);
	glutAddSubMenu("Rasterisation",		 field_rasterisation);
	glutAddMenuEntry("Smoothing ON/OFF", 3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Нарисовать вершины с указанным режимом
void Editor::drawVertices(GLenum mode, const Hexagon& hexagon) {
	glBegin(mode);

	for (uint8_t i = 0; i != hexagon.size(); ++i)
		glVertex2i(hexagon[i].x, hexagon[i].y);

	glEnd();
}

// Нарисовать полигон
void Editor::drawPolygon(std::vector<Hexagon>::size_type hex_number) {
	auto texture = hexes[hex_number].getTexture();

	if (texture == 0) {
		auto color = hexes[hex_number].getColor();
		glColor3ub(color.R, color.G, color.B);

		drawVertices(GL_POLYGON, hexes[hex_number]);
	}
	else {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glColor3ub(255, 255, 255);

		glBegin(GL_POLYGON);

		auto texture_coords = texture_loader.getCoordinates();
		for (uint8_t i = 0; i != hexes[hex_number].size(); ++i) {
			glTexCoord2f(texture_coords[i].x, texture_coords[i].y);
			glVertex2i(hexes[hex_number][i].x, hexes[hex_number][i].y);
		}

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

// Нарисовать шестиугольник под указанным номером
void Editor::drawHexagon(std::vector<Hexagon>::size_type hex_number) {
	auto center = hexes[hex_number].getCenter();
	auto move = hexes[hex_number].getTranslateCoords();
	auto scale = hexes[hex_number].getScale();

	glPushMatrix();

	glTranslatef(static_cast<GLfloat>(center.x + move.x), static_cast<GLfloat>(center.y + move.y), 0.0f);
	glRotatef(hexes[hex_number].getAngle(), 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, 0.0f);
	glTranslatef(static_cast<GLfloat>(-center.x), static_cast<GLfloat>(-center.y), 0.0f);

	drawPolygon(hex_number);

	glDisable(GL_COLOR_LOGIC_OP);
	glLineWidth(BORDER_WIDTH);
	glColor3ub(0, 0, 0);
	drawVertices(GL_LINE_LOOP, hexes[hex_number]);
	glEnable(GL_COLOR_LOGIC_OP);

	glPopMatrix();
}

// Нарисовать текущий шестиугольник
void Editor::drawCurrentHexagon() {
	if (hexes.size()) {
		auto center = hexes[current_hexagon].getCenter();
		auto move = hexes[current_hexagon].getTranslateCoords();
		auto scale = hexes[current_hexagon].getScale();

		glPushMatrix();

		glTranslatef(static_cast<GLfloat>(center.x + move.x), static_cast<GLfloat>(center.y + move.y), 0.0f);
		glRotatef(hexes[current_hexagon].getAngle(), 0.0f, 0.0f, 1.0f);
		glScalef(scale, scale, 0.0f);
		glTranslatef(static_cast<GLfloat>(-center.x), static_cast<GLfloat>(-center.y), 0.0f);
	
		drawPolygon(current_hexagon);

		glDisable(GL_COLOR_LOGIC_OP);
		if (!pointer_activity) {
			glLineWidth(BORDER_WIDTH);
			glColor3ub(0, 0, 0);
			drawVertices(GL_LINE_LOOP, hexes[current_hexagon]);
		}
		else {
			glLineWidth(SELECTED_EXTERN_BORDER_WIDTH);
			glColor3ub(0, 0, 0);
			drawVertices(GL_LINE_LOOP, hexes[current_hexagon]);

			glLineWidth(SELECTED_INTERN_BORDER_WIDTH);
			glColor3ub(204, 0, 0);
			drawVertices(GL_LINE_LOOP, hexes[current_hexagon]);
		}
		glEnable(GL_COLOR_LOGIC_OP);

		glPopMatrix();
	}
}

// Нарисовать указатель
void Editor::drawPointer() {
	if (!pointer_activity) {
		glDisable(GL_COLOR_LOGIC_OP);
		glColor3ub(0, 0, 0);
		glPointSize(POINTER_EXTERN_BORDER_WIDTH);

		glBegin(GL_POINTS);
		glVertex2i(pointer.x, pointer.y);
		glEnd();

		glEnable(GL_COLOR_LOGIC_OP);
		switch (color_operation) {
			case ColorOperation::AND:  glLogicOp(GL_OR);  break;
			case ColorOperation::NAND: glLogicOp(GL_NOR); break;
		}

		glColor3ub(pointer_color.R, pointer_color.G, pointer_color.B);
		glPointSize(POINTER_INTERN_BORDER_WIDTH);

		glBegin(GL_POINTS);
		glVertex2i(pointer.x, pointer.y);
		glEnd();
	}
}

// Удалить текущий шестиугольник
void Editor::deleteCurrentHexagon() {
	if (1 < hexes.size()) {
		hexes.erase(hexes.begin() + current_hexagon);
		current_hexagon = (current_hexagon + 1) % hexes.size();
	}
	else {
		std::vector<Hexagon>().swap(hexes);
		current_hexagon = 0;
	}
}

// Выбрать предыдущий шестиугольник
void Editor::prevHexagon() {
	if (1 < hexes.size())
		current_hexagon = current_hexagon == 0 ? hexes.size() - 1 : current_hexagon - 1;
}

// Выбрать сдедующий шестиугольник
void Editor::nextHexagon() {
	if (1 < hexes.size())
		current_hexagon = (current_hexagon + 1) % hexes.size();
}

// Повернуть текущий шестиугольник
void Editor::rotateCurrentHexagon(GLfloat degrees) {
	auto angle = (hexes[current_hexagon].getAngle());

	hexes[current_hexagon].setAngle(static_cast<GLfloat>(static_cast<GLint>(angle - degrees) % 360));
}

// Сдвинуть по оси Х текущий шестиугольник
void Editor::moveCurrentHexagonXAxis(GLint step) {
	auto current_coords = hexes[current_hexagon].getTranslateCoords();

	hexes[current_hexagon].setTranslateCoords(Point<GLint>(current_coords.x + step, current_coords.y));
}

// Сдвинуть по оси Y текущий шестиугольник
void Editor::moveCurrentHexagonYAxis(GLint step) {
	auto current_coords = hexes[current_hexagon].getTranslateCoords();

	hexes[current_hexagon].setTranslateCoords(Point<GLint>(current_coords.x, current_coords.y + step));
}

// Изменить масштаб текущего шестиугольника
void Editor::scaleCurrentHexagon(GLfloat factor) {
	auto current_scale = hexes[current_hexagon].getScale();

	if (0 < current_scale + factor)
		hexes[current_hexagon].setScale(current_scale + factor);
}

// Изменить текстуры текущего шестиугольника
void Editor::changeCurrentHexagonTexture() {
	if (!hexes.empty()) {
		auto textures = texture_loader.getTextures();
		auto pos = std::distance(textures.begin(), 
				   std::find(textures.begin(), textures.end(), 
							 hexes[current_hexagon].getTexture()));

		hexes[current_hexagon].setTexture(textures[(pos + 1) % textures.size()]);
	}
}

// Изменить цвет объекта (шестиугольник или указатель)
void Editor::changeObjectColor(ColorElem color_elem) {
	Color color = pointer_color = getColor(color_elem);

	if (pointer_activity)
		hexes[current_hexagon].setColor(color);
	else
		pointer_color = color;
}

// Изменить режим отображения (обычный или растр)
void Editor::changePresentationView() {
	presentation_view = presentation_view == PresentationView::GLUT ?
											 PresentationView::RASTER : 
											 PresentationView::GLUT;
}