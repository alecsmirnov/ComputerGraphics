#include "Editor.h"

#include <iomanip>
#include <sstream>
#include <cctype>

static constexpr Color BACKGROUD_COLOR = ColorElem::BLACK;
static constexpr Color INFO_COLOR      = ColorElem::WHITE;

static constexpr GLfloat CAMERA_MOVE_SPEED   = 5.0f;
static constexpr GLfloat CAMERA_SCALE_FACTOR = 0.4f;

static constexpr GLfloat CAMERA_SCALE_FACTOR_MIN = -CAMERA_SCALE_FACTOR - 1.0f ;
static constexpr GLfloat CAMERA_SCALE_FACTOR_MAX =  CAMERA_SCALE_FACTOR + 9.0f;

static constexpr GLfloat RADIUS_COEF = 0.02f;

static constexpr GLfloat RADIUS_MIN  = 0.02f;
static constexpr GLfloat RADIUS_MAX  = 9.f;

static constexpr GLint SECTORS_STACKS_COEF = 1;

static constexpr GLint SECTORS_STACKS_MIN  = -SECTORS_STACKS_COEF + 3 ;
static constexpr GLint SECTORS_STACKS_MAX  =  SECTORS_STACKS_COEF + 50;

static constexpr vec2<GLint> INFO_SHIFT = {4, -14};

Editor::Editor(GLint width, GLint height) { 
	this->width = width; 
	this->height = height; 

	sphere.init(1.0f, 36, 36);

	camera_x = 0.0f;
	camera_y = 0.0f;
	camera_scale = 1.0f;

	view_type = ViewType::FILL;

	color_num = 0;

	current_texture = 0;
}

void Editor::start(int* argc, char* argv[]) {
	glutInit(argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(width, height);
	glutCreateWindow(window_title);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	texture_loader.init();

	glutDisplayFunc(displayEvent);
	glutReshapeFunc(reshapeEvent);
	glutKeyboardFunc(keyboardEvent);	
	glutMouseWheelFunc(mouseWheelEvent);

	glutMainLoop();	
}

void Editor::addTexture(const char* image) {
	texture_loader.addTexture(image);
}

void Editor::displayEvent() {
	glClearColor(BACKGROUD_COLOR.R, BACKGROUD_COLOR.G, BACKGROUD_COLOR.B, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glTranslatef(0, 0, -(camera_scale + 3));
	glRotatef(camera_x, 1, 0, 0);
	glRotatef(camera_y, 0, 1, 0);

	drawInfo();

	auto color = getColorByNum(color_num);
	glColor3f(color.R, color.G, color.B);
	sphere.draw(current_texture);

	glPopMatrix();

	glutSwapBuffers();
	glFinish();
}

void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(40.0f, 1.0f * width / height, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Editor::keyboardEvent(std::uint8_t key, GLint x, GLint y) {
	switch (std::tolower(key)) {
		case SPACE_BUTTON: changeViewType();                    break;
		
		case W_BUTTON:     moveCameraX(CAMERA_MOVE_SPEED);      break;
		case S_BUTTON:     moveCameraX(-CAMERA_MOVE_SPEED);     break;
		case A_BUTTON:     moveCameraY(-CAMERA_MOVE_SPEED);     break;
		case D_BUTTON:     moveCameraY(CAMERA_MOVE_SPEED);      break;

		case Q_BUTTON:     changeRadius(-RADIUS_COEF);          break;
		case E_BUTTON:     changeRadius(RADIUS_COEF);           break;

		case Z_BUTTON:     changeSectors(-SECTORS_STACKS_COEF); break;
		case X_BUTTON:     changeSectors(SECTORS_STACKS_COEF);  break;

		case C_BUTTON:     changeStacks(-SECTORS_STACKS_COEF);  break;
		case V_BUTTON:     changeStacks(SECTORS_STACKS_COEF);   break;

		case TAB_BUTTON:   changeTexture();                     break;
		case ENTER_BUTTON: changeColor();                       break;

		case ESC_BUTTON:   glutLeaveMainLoop();                 break;
		default:;
	}

	glutPostRedisplay();
}

void Editor::mouseWheelEvent(GLint button, GLint dir, GLint x, GLint y) {
	if (0 < dir)
		cameraScale(CAMERA_SCALE_FACTOR);
	else
		cameraScale(-CAMERA_SCALE_FACTOR);

	glutPostRedisplay();
}

void Editor::moveCameraX(GLfloat speed) {
	camera_x -= speed;
}

void Editor::moveCameraY(GLfloat speed) {
	camera_y += speed;
}

void Editor::cameraScale(GLfloat factor) {
	if (CAMERA_SCALE_FACTOR_MIN < camera_scale + factor && camera_scale + factor < CAMERA_SCALE_FACTOR_MAX)
		camera_scale += factor;
}

void Editor::changeViewType() {
	if (view_type == ViewType::FILL)
		view_type = ViewType::LINE;
	else
		view_type = ViewType::FILL;

	switch (view_type) {
		case ViewType::FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
			break;
		case ViewType::LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
			break;
	}
}

void Editor::changeRadius(GLfloat radius_coef) {
	auto radius = sphere.getRadius();

	if (RADIUS_MIN < radius + radius_coef && radius + radius_coef < RADIUS_MAX)
		sphere.init(radius + radius_coef, sphere.getSectorCount(), sphere.getStackCount());
}

void Editor::changeSectors(GLint sector_coef) {
	auto sectors = sphere.getSectorCount();

	if (SECTORS_STACKS_MIN < sectors + sector_coef && sectors + sector_coef < SECTORS_STACKS_MAX)
		sphere.init(sphere.getRadius(), sectors + sector_coef, sphere.getStackCount());
}

void Editor::changeStacks(GLint stacks_coef) {
	auto stacks = sphere.getStackCount();

	if (SECTORS_STACKS_MIN < stacks + stacks_coef && stacks + stacks_coef < SECTORS_STACKS_MAX)
		sphere.init(sphere.getRadius(), sphere.getSectorCount(), stacks + stacks_coef);
}

void Editor::changeTexture() {
	auto textures = texture_loader.getTextures();

	if (1 < textures.size())
		current_texture = (current_texture + 1) % (textures.size() + 1);
}

void Editor::changeColor() {
	color_num = (color_num + 1) % ColorElem::size;
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

	std::string view_type_str = "fill";
	if (view_type == ViewType::LINE)
		view_type_str = "lines";

	drawText("View:    " + view_type_str, INFO_SHIFT[0], height + INFO_SHIFT[1]);
	drawText("Texture: " + textureInfo(current_texture), INFO_SHIFT[0], height + INFO_SHIFT[1] * 2);
	drawText("Color:   " + colorNameByNum(color_num), INFO_SHIFT[0], height + INFO_SHIFT[1] * 3);

	drawText("Scale:   " + valToStr(-(camera_scale - 2), 1), INFO_SHIFT[0], height + INFO_SHIFT[1] * 5);

	drawText("Radius:  " + valToStr(sphere.getRadius(), 2), INFO_SHIFT[0], height + INFO_SHIFT[1] * 7);
	drawText("Sectors: " + std::to_string(sphere.getSectorCount()), INFO_SHIFT[0], height + INFO_SHIFT[1] * 8);
	drawText("Stacks:  " + std::to_string(sphere.getStackCount()), INFO_SHIFT[0], height + INFO_SHIFT[1] * 9);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

std::string Editor::valToStr(GLfloat val, std::uint8_t precision) {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(precision) << val;

	return stream.str();
}

std::string Editor::textureInfo(std::vector<GLuint>::size_type num) {
	return num == 0 ? "none" : std::to_string(num);
}

std::string Editor::colorNameByNum(std::uint8_t num) {
	std::string name;

	switch (num) {
		case 0: name = "white";     break;
		case 1: name = "red";       break;
		case 2: name = "orange";    break;
		case 3: name = "yellow";    break;
		case 4: name = "green";     break;
		case 5: name = "blue";      break;
		case 6: name = "dark blue"; break;
		case 7: name = "violet";    break;
		default:;
	}

	return name;
}