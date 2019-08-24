#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include <GL/freeglut.h>

#include "Camera.h"

// Редактор поля шестиугольников
class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	// Начать работу редактора
	void start(int* argc, char* argv[]);

	~Editor();

private:
	// Вывод на экран
	static void displayEvent();
	// Изменение размеров окна
	static void reshapeEvent(GLint new_width, GLint new_height);
	// Обработка сообщений от клавиатуры
	static void keyboardEvent(std::uint8_t key, int x, int y);
	// Обработка сообщений от мышки
	static void mouseEvent(int button, int state, int x, int y);
	static void mouseMove(int x, int y);


	// Обработка левого клика мыши
	static void clickEvent(int x, int y);

	// Главное меню
	static void mainMenu(int task);
	// Обработка меню
	static void menuEvent();

	static void drawGrid();

private:
	static inline const char* window_title = "Ray tracing";

	static inline GLint width;
	static inline GLint height;

	static inline Camera camera;
};

#endif