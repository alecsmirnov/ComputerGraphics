#include "Editor.h"

#include <cctype>

#include "Color.h"

static constexpr Color   BACKGROUND_COLOR = ColorElem::WHITE;

static constexpr Color	 POINTS_COLOR = ColorElem::RED;
static constexpr GLfloat POINTS_SIZE  = 3.0f;

static constexpr Color   SPLINE_COLOR = ColorElem::BLUE;

static constexpr std::uint8_t DEGREE_MIN = 1;
static constexpr std::uint8_t DEGREE_MAX = 8;

static constexpr GLdouble STEP_MIN = 0.00001;
static constexpr GLdouble STEP_MAX = 0.1;

static constexpr GLint	 SHIFT_SPEED  = 15;
static constexpr GLfloat SCALE_FACTOR = 0.1f;

Editor::Editor(GLint width, GLint height) { 
	this->width = width; 
	this->height = height;
	shift = {0, 0};
	scale = 1;
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
	glutMouseWheelFunc(mouseWheelEvent);

	glutMainLoop();
}

Editor::~Editor() { 
	std::vector<BSpline::Point>().swap(points);
}

void Editor::displayEvent() {
	glClearColor(BACKGROUND_COLOR.R, BACKGROUND_COLOR.G, BACKGROUND_COLOR.B, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);

	glLoadIdentity();
	glScalef(scale, scale, 0.0f);
	glTranslated(shift.x, shift.y, 0);

	std::vector<BSpline::Point>::size_type degree = 3;
	double step = 0.01;

	drawPoints();
	drawSpline(degree, step);

	glFinish();
}

void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);	
}

void Editor::keyboardEvent(std::uint8_t key, int x, int y) {
	switch (std::tolower(key)) {
		case W_BUTTON:	 shift.y += SHIFT_SPEED; break;
		case S_BUTTON:	 shift.y -= SHIFT_SPEED; break;
		case A_BUTTON:	 shift.x -= SHIFT_SPEED; break;
		case D_BUTTON:	 shift.x += SHIFT_SPEED; break;

		case ESC_BUTTON: glutLeaveMainLoop();	 break;
		default:;
	}

	glutPostRedisplay();
}

void Editor::mouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_DOWN)
		if (button == GLUT_LEFT_BUTTON)
			points.push_back({x, height - y});

	glutPostRedisplay();
}

void Editor::mouseWheelEvent(int button, int dir, int x, int y) {
	if (0 < dir)
		scale += SCALE_FACTOR;
	else
		scale -= SCALE_FACTOR;

	glutPostRedisplay();
}

void Editor::drawPoints() {
	glColor3f(POINTS_COLOR.R, POINTS_COLOR.G, POINTS_COLOR.B);
	glPointSize(POINTS_SIZE);

	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);
	for (auto point : points)
		glVertex2i(point.x, point.y);
	glEnd();
}

void Editor::drawSpline(std::vector<BSpline::Point>::size_type degree, double step) {
	glColor3f(SPLINE_COLOR.R, SPLINE_COLOR.G, SPLINE_COLOR.B);

	auto spline_points = BSpline::calculate(points, degree, step);

	//glBegin(GL_POINTS);
	glBegin(GL_LINE_LOOP);
	for (auto spline_point : spline_points)
		glVertex2i(spline_point.x, spline_point.y);
	glEnd();
}