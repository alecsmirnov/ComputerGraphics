#include "Editor.h"

#include <cctype>

#include "Color.h"

// Field settings
static constexpr Color   BACKGROUND_COLOR	  = ColorElem::WHITE;

static constexpr Color	 POINTS_COLOR		  = ColorElem::RED;
static constexpr GLfloat POINTS_SIZE		  = 3.0f;

static constexpr Color   SPLINE_COLOR		  = ColorElem::BLUE;

static constexpr Color	 GRID_COLOR			  = ColorElem::GREY;
static constexpr Color	 GRID_COORDLINE_COLOR = ColorElem::BLACK;
static constexpr GLint	 GRID_CELL_SIZE		  = 10;

static constexpr GLint	 ARROW_WIDTH		  = 5;
static constexpr GLint	 ARROW_HEIGHT		  = 15;
static constexpr GLint	 DIVISION_SIZE		  = 5;

// Transformation settings
static constexpr std::uint8_t DEGREE_MIN		= 1;
static constexpr std::uint8_t DEGREE_MAX		= 8;

static constexpr GLdouble	  STEP_MIN			= 0.00001;
static constexpr GLdouble	  STEP_MAX			= 0.1;

static constexpr GLint		  SHIFT_SPEED	    = 20;
static constexpr GLfloat	  SCALE_COEF_FACTOR = 0.05f;
static constexpr GLfloat	  SCALE_COEF_MAX	= 10.0f;
static constexpr GLfloat	  SCALE_COEF_MIN	= 0.1f;

Editor::Editor(GLint width, GLint height) { 
	this->width = width; 
	this->height = height;
	shift = {0, 0};
	scale_coef = 1;
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
	clearPoints();
}

void Editor::displayEvent() {
	glClearColor(BACKGROUND_COLOR.R, BACKGROUND_COLOR.G, BACKGROUND_COLOR.B, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);

	std::vector<BSpline::Point>::size_type degree = 3;
	double step = 0.01;

	drawGrid();
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
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);
	glMatrixMode(GL_MODELVIEW);	
}

void Editor::keyboardEvent(std::uint8_t key, int x, int y) {
	switch (std::tolower(key)) {
		case W_BUTTON:	 shiftY(SHIFT_SPEED);  break;
		case S_BUTTON:	 shiftY(-SHIFT_SPEED); break;
		case A_BUTTON:	 shiftX(-SHIFT_SPEED); break;
		case D_BUTTON:	 shiftX(SHIFT_SPEED);  break;

		case ESC_BUTTON: glutLeaveMainLoop();  break;
		default:;
	}

	glutPostRedisplay();
}

void Editor::mouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_DOWN)
		switch (button) {
			case GLUT_LEFT_BUTTON:  addPoint(x, y); break;
			case GLUT_RIGHT_BUTTON: clearPoints();  break;
			default:;
		}

	glutPostRedisplay();
}

void Editor::mouseWheelEvent(int button, int dir, int x, int y) {
	if (0 < dir)
		scale(SCALE_COEF_FACTOR);
	else
		scale(-SCALE_COEF_FACTOR);

	glutPostRedisplay();
}

void Editor::drawGrid() {
	glColor3f(GRID_COLOR.R, GRID_COLOR.G, GRID_COLOR.B);

	glBegin(GL_LINES);
	for (GLint i = 0; i < width / 2; i += GRID_CELL_SIZE) {
		glVertex2i(i, -height / 2);
		glVertex2i(i, height / 2);
	}
	for (GLint i = 0; -width / 2 < i; i -= GRID_CELL_SIZE) {
		glVertex2i(i, -height / 2);
		glVertex2i(i, height / 2);
	}

	for (GLint i = 0; i < height / 2; i += GRID_CELL_SIZE) {
		glVertex2i(-width / 2, i);
		glVertex2i(width / 2, i);
	}
	for (GLint i = 0; -height / 2 < i; i -= GRID_CELL_SIZE) {
		glVertex2i(-width / 2, i);
		glVertex2i(width / 2, i);
	}
	glEnd();

	glColor3f(GRID_COORDLINE_COLOR.R, GRID_COORDLINE_COLOR.G, GRID_COORDLINE_COLOR.B);

	glBegin(GL_LINES);
	glVertex2i(0, -height / 2);
	glVertex2i(0, height / 2);

	for (GLint i = 0; i < width / 2; i += GRID_CELL_SIZE * DIVISION_SIZE) {
		glVertex2i(i, -ARROW_WIDTH);
		glVertex2i(i, ARROW_WIDTH);
	}
	for (GLint i = 0; -width / 2 < i; i -= GRID_CELL_SIZE * DIVISION_SIZE) {
		glVertex2i(i, -ARROW_WIDTH);
		glVertex2i(i, ARROW_WIDTH);
	}

	glVertex2i(width / 2 - ARROW_HEIGHT, ARROW_WIDTH);
	glVertex2i(width / 2, 0);
	glVertex2i(width / 2, 0);
	glVertex2i(width / 2 - ARROW_HEIGHT, -ARROW_WIDTH);

	glVertex2i(-width / 2, 0);
	glVertex2i(width / 2, 0);

	for (GLint i = 0; i < height / 2; i += GRID_CELL_SIZE * DIVISION_SIZE) {
		glVertex2i(-ARROW_WIDTH, i);
		glVertex2i(ARROW_WIDTH, i);
	}
	for (GLint i = 0; -height / 2 < i; i -= GRID_CELL_SIZE * DIVISION_SIZE) {
		glVertex2i(-ARROW_WIDTH, i);
		glVertex2i(ARROW_WIDTH, i);
	}

	glVertex2i(ARROW_WIDTH, height / 2 - ARROW_HEIGHT);
	glVertex2i(0, height / 2);
	glVertex2i(0, height / 2);
	glVertex2i(-ARROW_WIDTH, height / 2 - ARROW_HEIGHT);
	glEnd();

	glBegin(GL_POINTS);
	glVertex2i(0, 0);
	glEnd();
}

void Editor::drawPoints() {
	glPushMatrix();

	glScalef(scale_coef, scale_coef, 0.0f);
	glTranslated(shift.x, shift.y, 0);

	glColor3f(POINTS_COLOR.R, POINTS_COLOR.G, POINTS_COLOR.B);
	glPointSize(POINTS_SIZE);

	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);
	for (auto point : points)
		glVertex2i(point.x, point.y);
	glEnd();

	glPopMatrix();
}	

void Editor::drawSpline(std::vector<BSpline::Point>::size_type degree, double step) {
	glPushMatrix();

	glScalef(scale_coef, scale_coef, 0.0f);
	glTranslated(shift.x, shift.y, 0);

	glColor3f(SPLINE_COLOR.R, SPLINE_COLOR.G, SPLINE_COLOR.B);

	auto spline_points = BSpline::calculate(points, degree, step);

	//glBegin(GL_POINTS);
	glBegin(GL_LINE_LOOP);
	for (auto spline_point : spline_points)
		glVertex2i(spline_point.x, spline_point.y);
	glEnd();

	glPopMatrix();
}

void Editor::shiftX(GLint speed) {
	shift.x += static_cast<GLint>(speed / scale_coef);
}

void Editor::shiftY(GLint speed) {
	shift.y += static_cast<GLint>(speed / scale_coef);
}

void Editor::scale(GLfloat factor) {
	if (SCALE_COEF_MIN < scale_coef + factor && scale_coef + factor < SCALE_COEF_MAX)
		scale_coef += factor;
}

void Editor::addPoint(int x, int y) {
	points.push_back({static_cast<GLint>((x - width / 2) / scale_coef - shift.x),
					  static_cast<GLint>((height / 2 - y) / scale_coef - shift.y)});
}

void Editor::clearPoints() {
	std::vector<BSpline::Point>().swap(points);
}