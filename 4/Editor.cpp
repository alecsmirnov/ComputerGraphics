#define _CRT_SECURE_NO_WARNINGS
#include "Editor.h"
#include "KeyButtons.h"
#include "Color.h"

#include <cmath>
#include <cctype>

static const Vector3f    CAMERA_START_POS   = {-18.0f, 0.5f, -3.0f};
static const Vector3f    CAMERA_START_FRONT = {-12.0f, 0.5f, -3.0f};
static constexpr GLfloat CAMERA_SPEED       = 0.06f;

static const Color     GRID_COLOR     = {150, 127, 255};
static constexpr GLint GRID_SIZE      = 50;
static constexpr GLint GRID_CELL_SIZE = 1;

Editor::Editor(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	camera.setPosition(CAMERA_START_POS);
	camera.setFront(CAMERA_START_FRONT);
	camera.setSpeed(CAMERA_SPEED);
}

// Начать работу редактора
void Editor::start(int* argc, char* argv[]) {
	glutInit(argc, argv);					// Инициализация библиотеки

	glutInitDisplayMode(GLUT_RGB);			// Установка режима отображения 

	glutInitWindowSize(width, height);		// Размер окна
	glutCreateWindow(window_title);			// Создание окна с заголовком

	glutDisplayFunc(displayEvent);			// Установка функции отображения
	glutReshapeFunc(reshapeEvent);			// Установка функции изменения окна
	glutKeyboardFunc(keyboardEvent);		// Установка функции обработки клавиатуры
	glutMouseFunc(mouseEvent);				// Установка функции обработки мышки
	glutPassiveMotionFunc(mouseMove);

	menuEvent();							// Вызов выпадающего меню

	glutMainLoop();							// Выход в главный цикл
}

Editor::~Editor() {
	
}

void Editor::drawGrid() {
	glColor3ub(GRID_COLOR.R, GRID_COLOR.G, GRID_COLOR.B);

	for (GLint i = -GRID_SIZE; i <= GRID_SIZE; i += GRID_CELL_SIZE) {
		glBegin(GL_LINES);

		glVertex3i(-GRID_SIZE, 0, i);
		glVertex3i(GRID_SIZE, 0, i);

		glVertex3i(i, 0, -GRID_SIZE);
		glVertex3i(i, 0, GRID_SIZE);

		glEnd();
	}
}

// Вывод на экран
void Editor::displayEvent() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	auto camera_pos = camera.getPosition();
	auto camera_front = camera.getFront();
	auto camera_up = camera.getUp();

	gluLookAt(camera_pos.getX(),   camera_pos.getY(),   camera_pos.getZ(),
			  camera_front.getX(), camera_front.getY(), camera_front.getZ(),
			  camera_up.getX(),    camera_up.getY(),    camera_up.getZ());

	drawGrid();

	glutSwapBuffers();
	glFinish();
}

// Обработка сообщений от клавиатуры
void Editor::keyboardEvent(std::uint8_t key, int x, int y) {
	switch (std::tolower(key)) {
		case Q_BUTTON: break;
		case E_BUTTON: break;
		case W_BUTTON: camera.moveUp(); break;
		case S_BUTTON: camera.moveDown(); break;
		case A_BUTTON: camera.moveLeft();  break;
		case D_BUTTON: camera.moveRight();  break;
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
	gluPerspective(60.0, width / height, 0.1, 10000.0);
	glMatrixMode(GL_MODELVIEW);
}

// Обработка сообщений от мышки
void Editor::mouseEvent(int button, int state, int x, int y) {
	//if (state == GLUT_DOWN) {
	//	if (button == GLUT_LEFT_BUTTON)
	//		clickEvent(x, height - y);

	//	glutPostRedisplay();
	//}
}

void Editor::mouseMove(int x, int y) {
	camera.mouseView(width, height);

	glutPostRedisplay();
}

// Обработка левого клика мыши
void Editor::clickEvent(int x, int y) {
	
}

// Главное меню
void Editor::mainMenu(int task) {
	glutPostRedisplay();
}

// Обработка меню
void Editor::menuEvent() {
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
}