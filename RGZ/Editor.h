#ifndef EDITOR_H
#define EDITOR_H

#include <string>

#include "EditorSettings.h"
#include "ClosedBSpline.h"

class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	void start(int* argc, char* argv[]);

	~Editor();

private:
	enum class ViewType {
		LINE,
		POINT
	};

private:
	static void displayEvent();
	static void reshapeEvent(GLint new_width, GLint new_height);
	static void keyboardEvent(std::uint8_t key, GLint x, GLint y);
	static void mouseEvent(GLint button, GLint state, GLint x, GLint y);
	static void mouseWheelEvent(GLint button, GLint dir, GLint x, GLint y);

	static void drawText(std::string text, GLint x, GLint y);
	static void drawInfo();
	static void drawGrid();
	static void drawPoints();
	static void drawSpline(std::uint8_t degree, GLdouble step);
	
	static void shiftX(GLint speed);
	static void shiftY(GLint speed);
	static void scale(GLfloat factor);
	static void increaseDegree();
	static void decreaseDegree();
	static void increaseStep();
	static void decreaseStep();
	static void changeView(ViewType& view);
	static void addPoint(GLint x, GLint y);
	static void clearPoints();

	static std::string floatToString(GLfloat value);
	static std::string removeTrailingZeroes(GLdouble value);

private:
	static inline const char* window_title = "Closed B-spline";

	static inline GLint width;
	static inline GLint height;
	static inline ClosedBSpline::Point shift;
	static inline GLfloat scale_coef;

	static inline std::uint8_t degree;
	static inline GLdouble step;

	static inline std::vector<ClosedBSpline::Point> points;

	static inline ViewType points_view;
	static inline ViewType spline_view;
};

#endif