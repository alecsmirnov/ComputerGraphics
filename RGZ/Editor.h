#ifndef EDITOR_H
#define EDITOR_H

#include <GL/freeglut.h>

#include "KeyButtons.h"
#include "BSpline.h"

class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	void start(int* argc, char* argv[]);

	~Editor();

private:
	static void displayEvent();
	static void reshapeEvent(GLint new_width, GLint new_height);
	static void keyboardEvent(std::uint8_t key, int x, int y);
	static void mouseEvent(int button, int state, int x, int y);

	static void drawPoints();
	static void drawSpline(std::vector<BSpline::Point>::size_type degree, double step);

private:
	static inline const char* window_title = "Closed B-spline";

	static inline GLint width;
	static inline GLint height;
	static inline BSpline::Point shift;
	static inline GLfloat scale;

	static inline std::vector<BSpline::Point> points;
};

#endif