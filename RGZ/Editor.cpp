#include "Editor.h"

#include <cctype>
#include <iomanip>
#include <sstream>

Editor::Editor(GLint width, GLint height) { 
	this->width = width; 
	this->height = height;
	shift = {0, 0};
	scale_coef = 1;

	degree = DEGREE_MIN;
	step = STEP_MIN;

	points_view = ViewType::POINT;
	spline_view = ViewType::LINE;
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

	drawGrid();
	drawInfo();
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

void Editor::keyboardEvent(std::uint8_t key, GLint x, GLint y) {
	switch (std::tolower(key)) {
		case W_BUTTON:	   shiftY(SHIFT_SPEED);	    break;
		case S_BUTTON:	   shiftY(-SHIFT_SPEED);	break;
		case A_BUTTON:	   shiftX(-SHIFT_SPEED);	break;
		case D_BUTTON:	   shiftX(SHIFT_SPEED);	    break;

		case Q_BUTTON:	   decreaseDegree();		break;
		case E_BUTTON:	   increaseDegree();		break;

		case Z_BUTTON:	   decreaseStep();			break;
		case X_BUTTON:	   increaseStep();			break;

		case TAB_BUTTON:   changeView(points_view); break;
		case SPACE_BUTTON: changeView(spline_view); break;

		case ESC_BUTTON:   glutLeaveMainLoop();	    break;
		default:;
	}

	glutPostRedisplay();
}

void Editor::mouseEvent(GLint button, GLint state, GLint x, GLint y) {
	if (state == GLUT_DOWN)
		switch (button) {
			case GLUT_LEFT_BUTTON:  addPoint(x, y); break;
			case GLUT_RIGHT_BUTTON: clearPoints();  break;
			default:;
		}

	glutPostRedisplay();
}

void Editor::mouseWheelEvent(GLint button, GLint dir, GLint x, GLint y) {
	if (0 < dir)
		scale(SCALE_COEF_FACTOR);
	else
		scale(-SCALE_COEF_FACTOR);

	glutPostRedisplay();
}

void Editor::drawText(std::string text, GLint x, GLint y) {
	glRasterPos2i(x, y);
	for (auto symbol : text)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, symbol);
}

void Editor::drawInfo() {
	glColor3f(GRID_COORDLINE_COLOR.R, GRID_COORDLINE_COLOR.G, GRID_COORDLINE_COLOR.B);

	static constexpr GLint GRAPH_SHIFT_X = 4;
	static constexpr GLint GRAPH_SHIFT_Y = -12;

	drawText("Degree: " + std::to_string(degree), -width / 2 + GRAPH_SHIFT_X, height / 2 + GRAPH_SHIFT_Y);
	drawText("Step: " + removeTrailingZeroes(step), -width / 2 + GRAPH_SHIFT_X, height / 2 + GRAPH_SHIFT_Y * 2);

	for (GLint i = 0; i < width / 2; i += GRID_CELL_SIZE * DIVISION_SIZE)
		drawText(floatToString(1.0f * i / width / scale_coef), i + GRAPH_SHIFT_X, GRAPH_SHIFT_Y - 2);

	for (GLint i = 0; -width / 2 < i; i -= GRID_CELL_SIZE * DIVISION_SIZE)
		drawText(floatToString(1.0f * i / width / scale_coef), i + GRAPH_SHIFT_X, GRAPH_SHIFT_Y - 2);

	for (GLint i = GRID_CELL_SIZE * DIVISION_SIZE; i < height / 2; i += GRID_CELL_SIZE * DIVISION_SIZE)
		drawText(floatToString(1.0f * i / height / scale_coef), GRAPH_SHIFT_X, i + GRAPH_SHIFT_Y);

	for (GLint i = -GRID_CELL_SIZE * DIVISION_SIZE; -height / 2 < i; i -= GRID_CELL_SIZE * DIVISION_SIZE)
		drawText(floatToString(1.0f * i / height / scale_coef), GRAPH_SHIFT_X, i + GRAPH_SHIFT_Y);
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
	glVertex2i(-width / 2, 0);
	glVertex2i(width / 2, 0);

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

	glVertex2i(0, -height / 2);
	glVertex2i(0, height / 2);

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

	switch (points_view) {
		case ViewType::POINT: glBegin(GL_POINTS);	 break;
		case ViewType::LINE:  glBegin(GL_LINE_LOOP); break;
	}

	for (auto point : points)
		glVertex2i(point.x, point.y);
	glEnd();

	glPopMatrix();
}	

void Editor::drawSpline(std::uint8_t degree, GLdouble step) {
	glPushMatrix();

	glScalef(scale_coef, scale_coef, 0.0f);
	glTranslated(shift.x, shift.y, 0);

	glColor3f(SPLINE_COLOR.R, SPLINE_COLOR.G, SPLINE_COLOR.B);
	glPointSize(1.0f);

	switch (spline_view) {
		case ViewType::POINT: glBegin(GL_POINTS);	 break;
		case ViewType::LINE:  glBegin(GL_LINE_LOOP); break;
	}

	auto spline_points = ClosedBSpline::calculate(points, degree, step);

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

void Editor::increaseDegree() {
	if (degree < DEGREE_MAX)
		degree += DEGREE_COEF;
}

void Editor::decreaseDegree() {
	if (DEGREE_MIN < degree)
		degree -= DEGREE_COEF;
}

void Editor::increaseStep() {
	if (STEP_MAX / STEP_COEF < step)
		step *= STEP_COEF;
}

void Editor::decreaseStep() {
	if (step < STEP_MIN)
		step /= STEP_COEF;
}

void Editor::changeView(ViewType& view) {
	if (view == ViewType::LINE)
		view = ViewType::POINT;
	else
		view = ViewType::LINE;
}

void Editor::addPoint(GLint x, GLint y) {
	points.push_back({static_cast<GLint>((x - width / 2) / scale_coef - shift.x),
					  static_cast<GLint>((height / 2 - y) / scale_coef - shift.y)});
}

void Editor::clearPoints() {
	std::vector<ClosedBSpline::Point>().swap(points);
}

std::string Editor::floatToString(GLfloat value) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(PRECISION) << value;

	return stream.str();
}

std::string Editor::removeTrailingZeroes(GLdouble value) {
	auto str_value = std::to_string(value);
	str_value.erase(str_value.find_last_not_of('0') + 1, std::string::npos);

	return str_value;
}