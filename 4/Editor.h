#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include <GL/freeglut.h>

#include "Camera.h"

// �������� ���� ���������������
class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	// ������ ������ ���������
	void start(int* argc, char* argv[]);

	~Editor();

private:
	// ����� �� �����
	static void displayEvent();
	// ��������� �������� ����
	static void reshapeEvent(GLint new_width, GLint new_height);
	// ��������� ��������� �� ����������
	static void keyboardEvent(std::uint8_t key, int x, int y);
	// ��������� ��������� �� �����
	static void mouseEvent(int button, int state, int x, int y);
	static void mouseMove(int x, int y);


	// ��������� ������ ����� ����
	static void clickEvent(int x, int y);

	// ������� ����
	static void mainMenu(int task);
	// ��������� ����
	static void menuEvent();

	static void drawGrid();

private:
	static inline const char* window_title = "Ray tracing";

	static inline GLint width;
	static inline GLint height;

	static inline Camera camera;
};

#endif