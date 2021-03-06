﻿#include "Editor.h"
#include "KeyButtons.h"

#include <cctype>

// Шаг изменения координат группы
static constexpr GLint COORD_STEP = 10;

// Максимальный размер группы
static constexpr GLfloat GROUP_WIDTH_MAX = 30.5;

Editor::Editor(GLint width, GLint height) { 
	this->width = width; 
	this->height = height; 
	
	current_group = 0;						// Инициализация первой группы
	current_point = 0;						// Инициализация первой точки
	lines.resize(1);	

	smoothing = false; 
}

// Начать работу редактора
void Editor::start(int* argc, char* argv[]) {
	glutInit(argc, argv);					// Инициализация библиотеки

	glutInitDisplayMode(GLUT_RGB);			// Установка режима отображения 

	glutInitWindowSize(width, height);		// Размер окна
	glutCreateWindow(window_title); // Создание окна с заголовком

	glutDisplayFunc(displayEvent);			// Установка функции отображения
	glutReshapeFunc(reshapeEvent);			// Установка функции изменения окна
	glutKeyboardFunc(keyboardEvent);		// Установка функции обработки клавиатуры
	glutMouseFunc(mouseEvent);				// Установка функции обработки мышки

	menuEvent();							// Вызов выпадающего меню

	glutMainLoop();							// Выход в главный цикл
}

Editor::~Editor() { 
	std::vector<Line>().swap(lines); 
}

// Отрисовка группы линий по её номеру
void Editor::drawGroup(std::vector<Line>::size_type group_num) {
	glColor3f(lines[group_num].getColor().R, lines[group_num].getColor().G, lines[group_num].getColor().B);
	glLineWidth(lines[group_num].getWidth());
	glLineStipple(2, static_cast<GLushort>(lines[group_num].getType()));
	smoothing ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);

	glBegin(GL_LINE_STRIP);

	for (auto coords : lines[group_num])
		glVertex2i(coords.x, height - coords.y);

	glEnd();
}

// Отрисовка и выделение точки в текущей группе
void Editor::drawCurGroupPoint(std::vector<Point>::size_type point_num, GLfloat point_width) {
	if (lines[current_group].size()) {
		glPointSize(lines[current_group].getWidth() + point_width);
		glColor3f(lines[current_group].getColor().R, 
				  lines[current_group].getColor().G, 
				  lines[current_group].getColor().B);

		glBegin(GL_POINTS);

		glVertex2i(lines[current_group][point_num].x, height - lines[current_group][point_num].y);

		glEnd();
	}
}

// Вывод на экран
void Editor::displayEvent() {
	glClearColor(1.0, 1.0, 1.0, 0);		// Белый цвет полотна
	glClear(GL_COLOR_BUFFER_BIT);		// Очищаем буфер цвета

	glEnable(GL_LINE_STIPPLE);			// Маска для создания штриховых линий

	// Отрисовка всех незамкнутых ломаных, кроме текущей группы
	for (auto i = 0; i != lines.size(); ++i)
		if (i != current_group)
			drawGroup(i);

	// Отрисовка текущей группы
	drawGroup(current_group);

	// Отрисовка и выделение текущей точки в текущей группе
	drawCurGroupPoint(current_point, 3);
	// Отрисовка и выделение последней точки текущей группы
	drawCurGroupPoint(lines[current_group].size() - 1, 5);

	glFinish();
}

// Изменение размеров окна
void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
}

// Обработка сообщений от клавиатуры
void Editor::keyboardEvent(std::uint8_t key, int x, int y) {
	switch (std::tolower(key)) {
		case SPACE_BUTTON: newGroup(); 							break;	// Добавить новую группу
		case DEL_BUTTON:   deleteCurGroup();  					break;	// Удалить текущую группу
		case BS_BUTTON:	   deleteCurPoint(); 					break;	// Удалить последнюю точку
		case Q_BUTTON: 	   prevGroup();							break;	// Выбрать предыдущую группу
		case E_BUTTON: 	   nextGroup();							break;	// Выбрать следующую группу
		case W_BUTTON: 	   changeGroupYCoord(-COORD_STEP);		break;	// Переместить группу вверх
		case S_BUTTON: 	   changeGroupYCoord(COORD_STEP);		break;	// Переместить группу вниз
		case A_BUTTON: 	   changeGroupXCoord(-COORD_STEP);		break;	// Переместить группу влево
		case D_BUTTON: 	   changeGroupXCoord(COORD_STEP);		break;	// Переместить группу вправо
		case Z_BUTTON: 	   prevPoint();							break;	// Перейти к предыдущей точкв в группе
		case X_BUTTON:	   nextPoint(); 						break;	// Перейти к следующей точке в группе
		case EQUAL_BUTTON: groupWidthInc(); 					break;	// Увеличить ширину группу
		case MINUS_BUTTON: groupWidthDec();						break;	// Уменьшить ширину группы
		case TAB_BUTTON:   groupLineTypeChange();  				break;	// Изменить тип группы
		case ENTER_BUTTON: smoothingChange(); 				    break;	// Включить, выключить сглаживание
		case ONE_BUTTON:   setGroupColor(ColorElem::RED); 	    break;	// Изменить цвет группы на красный
		case TWO_BUTTON:   setGroupColor(ColorElem::ORANGE);    break;	// Изменить цвет группы на оранжевый
		case THREE_BUTTON: setGroupColor(ColorElem::YELLOW);    break;	// Изменить цвет группы на жёлтый
		case FOUR_BUTTON:  setGroupColor(ColorElem::GREEN);     break;	// Изменить цвет группы на зелёный
		case FIVE_BUTTON:  setGroupColor(ColorElem::BLUE); 	    break;	// Изменить цвет группы на голубой
		case SIX_BUTTON:   setGroupColor(ColorElem::DARK_BLUE); break;	// Изменить цвет группы на синий
		case SEVEN_BUTTON: setGroupColor(ColorElem::VIOLET); 	break;	// Изменить цвет группы на фиолетовый
		case EIGHT_BUTTON: setGroupColor(ColorElem::BLACK); 	break;	// Изменить цвет группы на чёрный
		case ESC_BUTTON:   glutLeaveMainLoop(); 				break;	// Выйти из программы
		default:;
	}

	glutPostRedisplay();
}

// Обработка сообщений от мышки
void Editor::mouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		// Добавить новую точку в текущую группу
		if (button == GLUT_LEFT_BUTTON) {
			lines[current_group].pushBack({x, y});
			current_point = lines[current_group].size() - 1;
		}

		glutPostRedisplay();
	}
}

// Пункт меню изменения текущей группы
void Editor::groupChangeMenu(int task) {
	switch (task) {
		case 0: prevGroup(); break;		// Предыдущая группа
		case 1: nextGroup(); break;		// Следующая группа
		default:;
	}

	glutPostRedisplay();
}

// Пункт меню изменения цвета группы
void Editor::groupColorMenu(int task) {
	switch (task) {
		case 0: setGroupColor(ColorElem::RED); break;
		case 1: setGroupColor(ColorElem::ORANGE); break;
		case 2: setGroupColor(ColorElem::YELLOW); break;
		case 3: setGroupColor(ColorElem::GREEN); break;
		case 4: setGroupColor(ColorElem::BLUE); break;
		case 5: setGroupColor(ColorElem::DARK_BLUE); break;
		case 6: setGroupColor(ColorElem::VIOLET); break;
		case 7: setGroupColor(ColorElem::BLACK); break;
		default:;
	}

	glutPostRedisplay();
}

// Пункт меню изменения типа группы
void Editor::groupLineTypeMenu(int task) {
	switch (task) {
		case 0: lines[current_group].setType(LineType::SOLID); break;
		case 1: lines[current_group].setType(LineType::DOT); break;
		case 2: lines[current_group].setType(LineType::DASH); break;
		case 3: lines[current_group].setType(LineType::DOTDASH); break;
		default:;
	}

	glutPostRedisplay();
}

// Главные пункты меню
void Editor::mainMenu(int task) {
	switch (task) {
		case 0: newGroup(); break;			// Новая группа
		case 1: deleteCurPoint(); break;	// Удалить последнюю точку
		case 2: deleteCurGroup(); break;	// Удалить текущую группу
		case 3: smoothingChange(); break;	// Включить, выключить сглаживание
		default:;
	}

	glutPostRedisplay();
}

// Обработка меню
void Editor::menuEvent() {
	// Пункт изменения текущей группы
	auto group_change = glutCreateMenu(groupChangeMenu);
	glutAddMenuEntry("Previos group", 0);
	glutAddMenuEntry("Next group",    1);

	// Пункт изменения цвета группы
	auto group_color = glutCreateMenu(groupColorMenu);
	glutAddMenuEntry("Red",       0);
	glutAddMenuEntry("Orange",    1);
	glutAddMenuEntry("Yellow",    2);
	glutAddMenuEntry("Green",     3);
	glutAddMenuEntry("Blue",      4);
	glutAddMenuEntry("Dark blue", 5);
	glutAddMenuEntry("Violet",    6);
	glutAddMenuEntry("Black",     7);

	// Пункт изменения типа группы
	auto group_type = glutCreateMenu(groupLineTypeMenu);
	glutAddMenuEntry("Solid",   0);
	glutAddMenuEntry("Dot",     1);
	glutAddMenuEntry("Dash",    2);
	glutAddMenuEntry("Dotdash", 3);

	// Главное меню
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("New group",         0);
	glutAddSubMenu("Group switching",     group_change);
	glutAddSubMenu("Group color",         group_color);
	glutAddSubMenu("Group line type",     group_type);
	glutAddMenuEntry("Remove last point", 1);
	glutAddMenuEntry("Remove group",      2);
	glutAddMenuEntry("Smoothing",         3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Добавить новую группу
void Editor::newGroup() {
	lines.push_back(Line());
	current_group = lines.size() - 1;
}

// Удалить текущую группу
void Editor::deleteCurGroup() {
	if (1 < lines.size()) {
		lines.erase(lines.begin() + current_group);
		current_group = (current_group + 1) % lines.size();
	}
	else
		std::vector<Line>(1).swap(lines);
}

// Удалить выбранную точку в текущей группе
void Editor::deleteCurPoint() {
	if (1 < lines[current_group].size()) {
		lines[current_group].deletePoint(current_point);
		current_point == 0 ? lines[current_group].size() - 1 : --current_point;
	}
	else {
		lines[current_group].clear();
		current_point = 0;
	}
}

// Выбрать предыдущую группу
void Editor::prevGroup() {
	if (1 < lines.size())
		current_group = current_group == 0 ? lines.size() - 1 : current_group - 1;

	current_point = lines[current_group].size() - 1;
}

// Выбрать следующую группу
void Editor::nextGroup() {
	if (1 < lines.size())
		current_group = (current_group + 1) % lines.size();

	current_point = lines[current_group].size() - 1;
}

// Выбрать предыдущую точку в текущей группе
void Editor::prevPoint() {
	if (1 < lines[current_group].size())
		current_point = current_point == 0 ? lines[current_group].size() - 1 : current_point - 1;
}

// Выбрать следующую точку в текущей группе
void Editor::nextPoint() {
	if (1 < lines[current_group].size())
		current_point = (current_point + 1) % lines[current_group].size();
}

// Изменить координаты группы по горизонтали
void Editor::changeGroupXCoord(GLint x) {
	GLint step = x;

	// Проверка попадания линии в правую, левую границу окна
	bool border_hit = false;
	for (auto i = 0; i != lines[current_group].size() && !border_hit; ++i) {
		if (width <= lines[current_group][i].x) {	// Если правая граница -- перемещаем влево
			border_hit = true;						// и прибавляем ширину группы линий
			step = -width + getGroupWidth();
		}

		if (lines[current_group][i].x < 0) {		// Если левая граница -- перемещаем вправо
			border_hit = true;						// и отнимаем ширину группы линий
			step = width - getGroupWidth();
		}
	}

	for (auto& coords : lines[current_group])
		coords.x = coords.x + step;
}

// Изменить координаты группы по вертикали
void Editor::changeGroupYCoord(GLint y) {
	GLint step = y;

	// Проверка попадания линии в верхнюю, нижнюю границу окна
	bool border_hit = false;
	for (auto i = 0; i != lines[current_group].size() && !border_hit; ++i) {
		if (height <= lines[current_group][i].y) {	// Если нижняя граница -- перемещаем вверх
			border_hit = true;						// и прибавляем высоту группы линий
			step = -height + getGroupHeight();	
		}										

		if (lines[current_group][i].y < 0) {		// Если верхняя граница -- перемещаем вниз
			border_hit = true;						// и отнимаем высоту группы линий
			step = height - getGroupHeight();
		}
	}

	for (auto& coords : lines[current_group])		// Изменяем координаты линии
		coords.y = coords.y + step;
}

// Получить ширину текущей группы (x_max - x_min)
GLint Editor::getGroupWidth() {
	GLint x_min = width;
	GLint x_max = 0;

	// Находим x_max и x_min
	for (auto line : lines[current_group]) {
		if (line.x < x_min)
			x_min = line.x;

		if (x_max < line.x)
			x_max = line.x;
	}

	return x_max - x_min;	// Вычисляем разницу
}

// Получить высоту текущей группы (y_max - y_min)
GLint Editor::getGroupHeight() {
	GLint y_min = height;
	GLint y_max = 0;

	// Находим y_max и y_min
	for (auto line : lines[current_group]) {
		if (line.y < y_min)
			y_min = line.y;

		if (y_max < line.y)
			y_max = line.y;
	}

	return y_max - y_min;	// Вычисляем разницу
}

// Увеличить ширину группы
void Editor::groupWidthInc() {
	if (lines[current_group].getWidth() < GROUP_WIDTH_MAX)
		lines[current_group].setWidth(lines[current_group].getWidth() + 0.5f);
}

// Уменьшить ширину группы
void Editor::groupWidthDec() {
	if (1 < lines[current_group].getWidth())
		lines[current_group].setWidth(lines[current_group].getWidth() - 0.5f);
}

// Изменить тип группы
void Editor::groupLineTypeChange() {
	switch (lines[current_group].getType()) {
		case LineType::SOLID:   lines[current_group].setType(LineType::DOT); break;
		case LineType::DOT:     lines[current_group].setType(LineType::DASH); break;
		case LineType::DASH:    lines[current_group].setType(LineType::DOTDASH); break;
		case LineType::DOTDASH: lines[current_group].setType(LineType::SOLID); break;
		default:;
	}
}

// Включить, выключить сглаживание
void Editor::smoothingChange() {
	smoothing = !smoothing;
}

// Задать цвет текущей группе
void Editor::setGroupColor(Color color) {
	lines[current_group].setColor(color);
}

