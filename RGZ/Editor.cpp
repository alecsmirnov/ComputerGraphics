#include "Editor.h"
//#include "KeyButtons.h"

#include <cctype>

Editor::Editor(GLint width, GLint height) { 
	this->width = width; 
	this->height = height; 
}

void Editor::start(int* argc, char* argv[]) {
	glutInit(argc, argv);

	glutInitDisplayMode(GLUT_RGB);

	glutInitWindowSize(width, height);
	glutCreateWindow(window_title);

	glutDisplayFunc(displayEvent);
	glutReshapeFunc(reshapeEvent);
	glutKeyboardFunc(keyboardEvent);
	glutMouseFunc(mouseEvent);

	glutMainLoop();
}

Editor::~Editor() { 
	std::vector<BSpline::Point>().swap(points);
}

void Editor::displayEvent() {
	glClearColor(1.0, 1.0, 1.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(5.0f);
	glColor3f(1.0, 0.0, 0.0);
	
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);
	for (auto point : points)
		glVertex2i(point.x, point.y);
	glEnd();


	std::vector<BSpline::Point>::size_type degree = 1;
	auto spline_points = BSpline::calculate(points, degree, 0.01);

	glColor3f(0.0, 0.0, 1.0);

	//glBegin(GL_POINTS);
	glBegin(GL_LINE_LOOP);
	for (auto spline_point : spline_points)
		glVertex2i(spline_point.x, spline_point.y);
	glEnd();

	glFinish();
}

void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
}

void Editor::keyboardEvent(std::uint8_t key, int x, int y) {

	glutPostRedisplay();
}

void Editor::mouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_DOWN)
		if (button == GLUT_LEFT_BUTTON)
			points.push_back({x, height - y});

	glutPostRedisplay();
}