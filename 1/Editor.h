#ifndef EDITOR_H
#define EDITOR_H

#include "Line.h"

// Класс, представлющий редактор примитива: линия
class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	// Начать работу редактора
	void start(int* argc, char* argv[]);

	~Editor();

private:
	// Отрисовка группы линий по её номеру
	static void drawGroup(std::vector<Line>::size_type group_num);
	// Отрисовка и выделение точки в текущей группе
	static void drawCurGroupPoint(std::vector<Point>::size_type point_num, GLfloat point_width);

	// Вывод на экран
	static void displayEvent();
	// Изменение размеров окна
	static void reshapeEvent(GLint new_width, GLint new_height);
	// Обработка сообщений от клавиатуры
	static void keyboardEvent(std::uint8_t key, int x, int y);
	// Обработка сообщений от мышки
	static void mouseEvent(int button, int state, int x, int y);

	// Пункт меню изменения текущей группы
	static void groupChangeMenu(int task);
	// Пункт меню изменения цвета группы
	static void groupColorMenu(int task);
	// Пункт меню изменения типа группы
	static void groupLineTypeMenu(int task);
	// Главные пункты меню
	static void mainMenu(int task);
	// Обработка меню
	static void menuEvent();

	// Добавить новую группу
	static void newGroup();
	// Удалить текущую группу
	static void deleteCurGroup();
	// Удалить выбранную точку в текущей группе
	static void deleteCurPoint();

	// Выбрать предыдущую группу
	static void prevGroup();
	// Выбрать следующую группу
	static void nextGroup();

	// Выбрать предыдущую точку в текущей группе
	static void prevPoint();
	// Выбрать следующую точку в текущей группе
	static void nextPoint();

	// Изменить координаты группы по горизонтали
	static void changeGroupXCoord(GLint x);
	// Изменить координаты группы по вертикали
	static void changeGroupYCoord(GLint y);

	// Получить ширину текущей группы (x_max - x_min)
	static GLint getGroupWidth();
	// Получить высоту текущей группы (y_max - y_min)
	static GLint getGroupHeight();

	// Увеличить ширину группы
	static void groupWidthInc();
	// Уменьшить ширину группы
	static void groupWidthDec();

	// Изменить тип группы
	static void groupLineTypeChange();

	// Включить, выключить сглаживание
	static void smoothingChange();

	// Задать цвет текущей группе
	static void setGroupColor(Color color);

private:
	static inline const char* window_title = "Line editor";

	static inline GLint width;
	static inline GLint height;

	static inline std::vector<Line> lines;
	static inline std::vector<Line>::size_type current_group;
	static inline std::vector<Point>::size_type current_point;

	static inline bool smoothing;
};

#endif