#include "Editor.h"

#include <fstream>
#include <cctype>

#include "KeyButtons.h"

struct Point {
	GLint x;
	GLint y;
};

static const Vector3f    CAMERA_START_POS   = {-18.0f, 0.5f, -3.0f};
static const Vector3f    CAMERA_START_FRONT = {-12.0f, 0.5f, -3.0f};
static constexpr GLfloat CAMERA_SPEED       = 0.08f;

static constexpr Color   GRID_COLOR         = ColorElem::BLUE;
static constexpr GLint   GRID_SIZE          = 50;
static constexpr GLint   GRID_CELL_SIZE     = 1;

static constexpr GLfloat LIGHT_DISTANCE     = 400.0f;
static constexpr GLfloat LIGHT_COEF[]       = {0.0f, 0.0f, 255.0f / (LIGHT_DISTANCE * LIGHT_DISTANCE)};

static const Vector3f    ROTATE_CENTER      = {0.0f, 0.0f, 0.0f};
static const Rotation    ROTATION_INIT      = {false, {0.0f, 0.0f, 0.0f}};

static constexpr Color   INFO_COLOR         = ColorElem::WHITE;
static constexpr Point   INFO_SHIFT			= {4, -14};

Editor::Editor(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	camera.setPosition(CAMERA_START_POS);
	camera.setFront(CAMERA_START_FRONT);
	camera.setSpeed(CAMERA_SPEED);

	current_figure = 0;

	selection = false;
	light = false;
}

void Editor::addTexture(const char* image) {
	texture_loader.addTexture(image);
}

void Editor::readInputFiles(std::string objects_filename, std::string light_sources_filename) {
	readObjectsFile(objects_filename);
	readLightSourcesFile(light_sources_filename);
}

void Editor::start(int* argc, char* argv[]) {
	glutInit(argc, argv);

	glutInitDisplayMode(GLUT_RGB);

	glutInitWindowSize(width, height);
	glutCreateWindow(window_title);

	texture_loader.init();

	glutDisplayFunc(displayEvent);
	glutReshapeFunc(reshapeEvent);
	glutKeyboardFunc(keyboardEvent);
	glutPassiveMotionFunc(mouseMoveEvent);

	glutMainLoop();
}

Editor::~Editor() {
	for (std::vector<Figure*>::size_type i = 0; i != figures.size(); ++i)
		delete[] figures[i];

	std::vector<Figure*>().swap(figures);
	std::vector<Sphere*>().swap(spheres);
	std::vector<LightSource*>().swap(light_sources);
}

void Editor::readObjectsFile(std::string filename) {
	std::ifstream object_file(filename);

	GLfloat center_x, center_y, center_z;
	Color color;
	GLfloat radius;

	while (object_file >> radius >> center_x >> center_y >> center_z >> color.R >> color.G >> color.B) {
		auto new_sphere = new Sphere(radius, Vector3f(center_x, center_y, center_z), color);

		spheres.push_back(new_sphere);
		figures.push_back(new_sphere);
		figures_rotation.push_back(ROTATION_INIT);
	}
	
	object_file.close();
}

void Editor::readLightSourcesFile(std::string filename) {
	std::ifstream light_file(filename);

	GLfloat position_x, position_y, position_z;
	Color color;

	while (light_file >> position_x >> position_y >> position_z >> color.R >> color.G >> color.B) {
		auto new_light_source = new LightSource(Vector3f(position_x, position_y, position_z), color);

		light_sources.push_back(new_light_source);
		figures.push_back(new_light_source);
		figures_rotation.push_back(ROTATION_INIT);
	}

	light_file.close();
}

void Editor::nextFigure() {
	if (selection && 1 < figures.size())
		current_figure = (current_figure + 1) % figures.size();
}

void Editor::prevFigure() {
	if (selection && 1 < figures.size())
		current_figure = current_figure == 0 ? figures.size() - 1 : current_figure - 1;
}

void Editor::figureMoveForward() {
	if (selection && !figures.empty())
		figures[current_figure]->moveForward();
}

void Editor::figureMoveBack() {
	if (selection && !figures.empty())
		figures[current_figure]->moveBack();
}

void Editor::figureMoveLeft() {
	if (selection && !figures.empty())
		figures[current_figure]->moveLeft();
}

void Editor::figureMoveRight() {
	if (selection && !figures.empty())
		figures[current_figure]->moveRight();
}

void Editor::figureMoveUp() {
	if (selection && !figures.empty())
		figures[current_figure]->moveUp();
}

void Editor::figureMoveDown() {
	if (selection && !figures.empty())
		figures[current_figure]->moveDown();
}

void Editor::rotationSwitch() {
	if (selection && !figures.empty())
		figures_rotation[current_figure].active = !figures_rotation[current_figure].active;
}

void Editor::figureRotateX() {
	if (selection && !figures.empty() && figures_rotation[current_figure].active)
		if (figures_rotation[current_figure].coords.getX() == 1.0f)
			figures_rotation[current_figure].coords.setX(0.0f);
		else
			figures_rotation[current_figure].coords.setX(1.0f);
}

void Editor::figureRotateY() {
	if (selection && !figures.empty() && figures_rotation[current_figure].active)
		if (figures_rotation[current_figure].coords.getY() == 1.0f)
			figures_rotation[current_figure].coords.setY(0.0f);
		else
			figures_rotation[current_figure].coords.setY(1.0f);
}

void Editor::figureRotateZ() {
	if (selection && !figures.empty() && figures_rotation[current_figure].active)
		if (figures_rotation[current_figure].coords.getZ() == 1.0f)
			figures_rotation[current_figure].coords.setZ(0.0f);
		else
			figures_rotation[current_figure].coords.setZ(1.0f);
}

void Editor::changeSphereTexture() {
	if (selection && !figures.empty()) {
		auto textures = texture_loader.getTextures();
		GLuint texture = (figures[current_figure]->getTexture() + 1) % textures.size();

		figures[current_figure]->setTexture(texture);
	}
}

void Editor::selectionSwitch() {
	selection = !selection;
}

void Editor::drawText(std::string text, GLint x, GLint y) {
	glRasterPos2i(x, y);

	for (auto symbol : text)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, symbol);
}

void Editor::drawInfo() {
	glColor3f(INFO_COLOR.R, INFO_COLOR.G, INFO_COLOR.B);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();

	auto boolToStr = [](bool val) -> std::string {
		return val ? "true" : "false";
	};

	auto textureInfo = [](std::size_t num) -> std::string {
		return num == 0 ? "none" : std::to_string(figures[current_figure]->getTexture());
	};
	
	drawText("Figures count:  " + std::to_string(figures.size()), INFO_SHIFT.x, height + INFO_SHIFT.y);
	drawText("Lighting:       " + boolToStr(light), INFO_SHIFT.x, height + INFO_SHIFT.y * 2);
	drawText("Selection:      " + boolToStr(selection), INFO_SHIFT.x, height + INFO_SHIFT.y * 3);

	if (selection && figures.size()) {
		drawText("Current figure: " + std::to_string(current_figure), INFO_SHIFT.x, height + INFO_SHIFT.y * 5);
		drawText("Figure rotate:  " + boolToStr(figures_rotation[current_figure].active), INFO_SHIFT.x, height + INFO_SHIFT.y * 6);

		GLint row_shift = 0;
		if (figures_rotation[current_figure].active) {
			drawText("Rotate X:       " + boolToStr(figures_rotation[current_figure].coords.getX()), INFO_SHIFT.x, height + INFO_SHIFT.y * 7);
			drawText("Rotate y:       " + boolToStr(figures_rotation[current_figure].coords.getY()), INFO_SHIFT.x, height + INFO_SHIFT.y * 8);
			drawText("Rotate Z:       " + boolToStr(figures_rotation[current_figure].coords.getZ()), INFO_SHIFT.x, height + INFO_SHIFT.y * 9);

			row_shift = 3;
		}

		drawText("Position X:     " + std::to_string(figures[current_figure]->getPosition().getX()), INFO_SHIFT.x, height + INFO_SHIFT.y * (8 + row_shift));
		drawText("Position Y:     " + std::to_string(figures[current_figure]->getPosition().getY()), INFO_SHIFT.x, height + INFO_SHIFT.y * (9 + row_shift));
		drawText("Position Z:     " + std::to_string(figures[current_figure]->getPosition().getZ()), INFO_SHIFT.x, height + INFO_SHIFT.y * (10 + row_shift));

		drawText("Figure texture: " + textureInfo(figures[current_figure]->getTexture()), INFO_SHIFT.x, height + INFO_SHIFT.y * (12 + row_shift));
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Editor::drawGrid() {
	glColor3f(GRID_COLOR.R, GRID_COLOR.G, GRID_COLOR.B);

	for (GLint i = -GRID_SIZE; i <= GRID_SIZE; i += GRID_CELL_SIZE) {
		glBegin(GL_LINES);

		glVertex3i(-GRID_SIZE, 0, i);
		glVertex3i(GRID_SIZE, 0, i);

		glVertex3i(i, 0, -GRID_SIZE);
		glVertex3i(i, 0, GRID_SIZE);

		glEnd();
	}
}

void Editor::drawScene() {
	drawGrid();

	for (auto sphere : spheres)
		sphere->draw();

	glDisable(GL_LIGHTING);

	drawInfo();

	if (selection)
		if (!figures.empty())
			figures[current_figure]->drawFrame();

	for (std::vector<LightSource*>::size_type i = 0; i != light_sources.size(); ++i) {
		light_sources[i]->draw();
		drawLight(i);
	}

	for (std::vector<Figure*>::size_type i = 0; i != figures.size(); ++i)
		if (figures_rotation[i].active)
			figures[i]->rotate(ROTATE_CENTER, figures_rotation[i].coords);
	
	glutPostRedisplay();
}

void Editor::drawLight(std::vector<LightSource*>::size_type num) {
	glEnable(GL_COLOR_MATERIAL);

	GLfloat color[] = {light_sources[num]->getColor().R,
					   light_sources[num]->getColor().G,
					   light_sources[num]->getColor().B};
	GLfloat position[] = {light_sources[num]->getPosition().getX(),
						  light_sources[num]->getPosition().getY(),
						  light_sources[num]->getPosition().getZ(), 1.0f};

	glEnable(GL_LIGHT0 + num);

	glLightfv(GL_LIGHT0 + num, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT0 + num, GL_POSITION, position);

	glLightf(GL_LIGHT0 + num, GL_CONSTANT_ATTENUATION, LIGHT_COEF[0]);
	glLightf(GL_LIGHT0 + num, GL_LINEAR_ATTENUATION, LIGHT_COEF[1]);
	glLightf(GL_LIGHT0 + num, GL_QUADRATIC_ATTENUATION, LIGHT_COEF[2]);
}

void Editor::displayEvent() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glutSetCursor(GLUT_CURSOR_NONE);
	light ? glEnable(GL_LIGHTING) :
			glDisable(GL_LIGHTING);
	glLoadIdentity();

	auto camera_pos = camera.getPosition();
	auto camera_front = camera.getFront();
	auto camera_up = camera.getUp();

	gluLookAt(camera_pos.getX(), camera_pos.getY(), camera_pos.getZ(),
			  camera_front.getX(), camera_front.getY(), camera_front.getZ(),
			  camera_up.getX(), camera_up.getY(), camera_up.getZ());

	drawScene();

	glutSwapBuffers();
	glFinish();
}

void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, width / height, 0.1, GRID_SIZE * 2.0f);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void Editor::keyboardEvent(std::uint8_t key, GLint x, GLint y) {
	switch (std::tolower(key)) {
		case W_BUTTON:	   camera.moveForward();  break;
		case S_BUTTON:	   camera.moveBack();	  break;
		case A_BUTTON:	   camera.moveLeft();	  break;
		case D_BUTTON:	   camera.moveRight();	  break;

		case Q_BUTTON:	   prevFigure();		  break;
		case E_BUTTON:	   nextFigure();	      break;

		case I_BUTTON:     figureMoveForward();	  break;
		case K_BUTTON:     figureMoveBack();	  break;
		case J_BUTTON:     figureMoveLeft();	  break;
		case L_BUTTON:     figureMoveRight();	  break;
		case M_BUTTON:	   figureMoveUp();		  break;
		case N_BUTTON:     figureMoveDown();	  break;

		case ZERO_BUTTON:  figureRotateX();       break;
		case MINUS_BUTTON: figureRotateY();       break;
		case EQUAL_BUTTON: figureRotateZ();       break;

		case SPACE_BUTTON: selectionSwitch();     break;
		case DEL_BUTTON:   rotationSwitch();      break;

		case ENTER_BUTTON: light = !light;        break;

		case Z_BUTTON:	   changeSphereTexture(); break;

		case ESC_BUTTON:   glutLeaveMainLoop();	  break;
		default:;
	}

	glutPostRedisplay();
}

void Editor::mouseMoveEvent(GLint x, GLint y) {
	camera.mouseView(width, height);

	glutPostRedisplay();
}