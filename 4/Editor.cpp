#include "Editor.h"

#include <fstream>
#include <cmath>
#include <cctype>

#include "KeyButtons.h"

static const Vector3f    CAMERA_START_POS   = {-18.0f, 0.5f, -3.0f};
static const Vector3f    CAMERA_START_FRONT = {-12.0f, 0.5f, -3.0f};
static constexpr GLfloat CAMERA_SPEED       = 0.08f;

static constexpr Color GRID_COLOR     = ColorElem::BLUE;
static constexpr GLint GRID_SIZE      = 50;
static constexpr GLint GRID_CELL_SIZE = 1;

static constexpr GLfloat SHINE_MIN = 0.5f;

static constexpr GLfloat PI_F = 3.14159265358979f;

Editor::Editor(GLint width, GLint height) {
	this->width = width;
	this->height = height;

	camera.setPosition(CAMERA_START_POS);
	camera.setFront(CAMERA_START_FRONT);
	camera.setSpeed(CAMERA_SPEED);

	ray_tracing_field = new Color*[height];
	for (GLint i = 0; i != height; ++i)
		ray_tracing_field[i] = new Color[width];

	recurs_lvl = 4;
	pixel_size = 2;

	smoothing = false;
	ray_tracing = false;
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

	glutDisplayFunc(displayEvent);
	glutReshapeFunc(reshapeEvent);
	glutKeyboardFunc(keyboardEvent);
	glutPassiveMotionFunc(mouseMoveEvent);

	glutMainLoop();
}

Editor::~Editor() {
	for (GLint i = 0; i != height; ++i)
		delete[] ray_tracing_field[i];
	delete[] ray_tracing_field;

	for (std::vector<Figure>::size_type i = 0; i != figures.size(); ++i)
		delete[] figures[i];

	std::vector<Figure*>().swap(figures);
	std::vector<LightSource>().swap(light_sources);
}

void Editor::readObjectsFile(std::string filename) {
	std::ifstream object_file(filename);

	auto readMterial = [](std::ifstream& input_stream) -> Material {
		Material material;

		input_stream >> material.ambient[0] >> material.ambient[1] >> material.ambient[2] >> material.ambient[3];
		input_stream >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2] >> material.diffuse[3];
		input_stream >> material.specular[0] >> material.specular[1] >> material.specular[2] >> material.specular[3];
		input_stream >> material.emission[0] >> material.emission[1] >> material.emission[2] >> material.emission[3];
		input_stream >> material.shine;

		return material;
	};

	for (GLint type; object_file >> type;) {
		GLfloat center_x, center_y, center_z;
		Color color;
		Material material;

		switch (static_cast<ObjectType>(type)) {
			case ObjectType::SPHERE: {
				GLfloat radius;

				object_file >> radius;
				object_file >> center_x >> center_y >> center_z;
				object_file >> color.R >> color.G >> color.B;
				material = readMterial(object_file);

				figures.push_back(new Sphere(radius, Vector3f(center_x, center_y, center_z), color, material));

				break;
			}
			case ObjectType::PRISM: {
				std::array<Vector3f, Prism::vertices> vertices;
				GLfloat coord_x, coord_y, coord_z;

				for (std::array<Vector3f, Prism::vertices>::size_type i = 0; i != vertices.size(); ++i) {
					object_file >> coord_x >> coord_y >> coord_z;

					vertices[i].setCoords(coord_x, coord_y, coord_z);
				}
				object_file >> center_x >> center_y >> center_z;
				object_file >> color.R >> color.G >> color.B;
				material = readMterial(object_file);

				figures.push_back(new Prism(vertices, Vector3f(center_x, center_y, center_z), color, material));

				break;
			}
			case ObjectType::PLANE: {
				GLfloat scale_x, scale_z;

				object_file >> center_x >> center_y >> center_z;
				object_file >> scale_x >> scale_z;
				object_file >> color.R >> color.G >> color.B;
				material = readMterial(object_file);

				figures.push_back(new Plane(Vector3f(center_x, center_y, center_z), scale_x, scale_z, color, material));

				break;
			}
			default:;
		}
	}

	object_file.close();
}

void Editor::readLightSourcesFile(std::string filename) {
	std::ifstream light_file(filename);

	GLfloat position_x, position_y, position_z;
	Color color;

	while (light_file >> position_x >> position_y >> position_z >> color.R >> color.G >> color.B)
		light_sources.push_back(LightSource(Vector3f(position_x, position_y, position_z), color));

	light_file.close();
}

void Editor::displayEvent() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	auto camera_pos = camera.getPosition();
	auto camera_front = camera.getFront();
	auto camera_up = camera.getUp();

	glutSetCursor(GLUT_CURSOR_NONE);
	gluLookAt(camera_pos.getX(),   camera_pos.getY(),   camera_pos.getZ(),
			  camera_front.getX(), camera_front.getY(), camera_front.getZ(),
			  camera_up.getX(),    camera_up.getY(),    camera_up.getZ());

	reshapeEvent(width, height);

	if (ray_tracing) {
		trace(camera.getPosition(), static_cast<GLfloat>(width / height));
	}
	else {
		for (auto& figure : figures)
			figure->draw();

		drawGrid();
	}

	for (auto& light : light_sources)
		light.draw();

	glutPostRedisplay();

	glutSwapBuffers();
	glFinish();
}

void Editor::reshapeEvent(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	if (ray_tracing) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
	}
	else {
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, width / height, 0.1, GRID_SIZE * 2.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}

void Editor::keyboardEvent(std::uint8_t key, int x, int y) {
	switch (std::tolower(key)) {
		case Q_BUTTON: smoothing = !smoothing; break;
		case E_BUTTON: break;
		case W_BUTTON: camera.moveUp();    break;
		case S_BUTTON: camera.moveDown();  break;
		case A_BUTTON: camera.moveLeft();  break;
		case D_BUTTON: camera.moveRight(); break;
		case ESC_BUTTON: glutLeaveMainLoop(); break;
		case ENTER_BUTTON: ray_tracing = !ray_tracing; break;
		default:; 
	}

	glutPostRedisplay();
}

void Editor::mouseMoveEvent(int x, int y) {
	camera.mouseView(width, height);

	glutPostRedisplay();
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

bool Editor::isShadow(Ray ray) {
	bool hit = false;

	for (auto i = figures.begin(); i != figures.end() && !hit; ++i)
		if ((*i)->getVisibility() && (*i)->isSimpleHit(ray))
			hit = true;

	return hit;
}

std::vector<Collision> Editor::getCollisions(Ray ray) {
	std::vector<Collision> collisions;
	std::vector<Collision> best_collisions;

	best_collisions.resize(0);

	for (std::vector<Figure*>::size_type i = 0; i != figures.size(); ++i)
		if (figures[i]->getVisibility() && figures[i]->isHit(ray, collisions)) {
			for (int j = 0; j != collisions.size(); ++j)
				collisions[j].object_id = i;

			if (best_collisions.empty() || collisions[0].time < best_collisions[0].time)
				best_collisions = collisions;
		}

	return best_collisions;
}

Color Editor::getShadeColor(Ray ray) {
	auto color = ColorElem::BLACK;

	auto best_collisions = getCollisions(ray);

	if (!best_collisions.empty()) {
		auto first_collision = best_collisions[0];

		auto view = normalize(Vector3f(-ray.front.getX(), -ray.front.getY(), -ray.front.getZ()));

		auto obj_id = best_collisions[0].object_id;
		auto obj_material = figures[obj_id]->getMaterial();

		color.R = obj_material.emission[0];
		color.G = obj_material.emission[1];
		color.B = obj_material.emission[2];

		auto hit_normal = normalize(first_collision.normal);

		GLfloat eps = 0.0001f;
		Ray shadow_ray = {first_collision.position - ray.front * eps, {}, 1};

		for (std::vector<LightSource>::size_type i = 0; i != light_sources.size(); ++i) {
			auto light_color = light_sources[i].getColor();

			Color ambient_color = {obj_material.ambient[0] * light_color.R, 
								   obj_material.ambient[1] * light_color.G, 
								   obj_material.ambient[2] * light_color.B};

			color = {color.R + ambient_color.R, color.G + ambient_color.G, color.B + ambient_color.B};

			shadow_ray.front = light_sources[i].getPosition() - first_collision.position;

			if (!isShadow(shadow_ray)) {
				auto source = normalize(shadow_ray.front);

				auto lambert_angle = scalarProduct(source, hit_normal);

				if (0.0f < lambert_angle) {
					Color diffuse_color = {lambert_angle * obj_material.diffuse[0] * light_color.R,
										   lambert_angle * obj_material.diffuse[1] * light_color.G,
										   lambert_angle * obj_material.diffuse[2] * light_color.B};

					color = {color.R + diffuse_color.R, color.G + diffuse_color.G, color.B + diffuse_color.B};
				}

				auto phong_angle = scalarProduct(normalize(view + source), hit_normal);

				if (0.0f < phong_angle) {
					auto phong_coef = std::powf(phong_angle, obj_material.shine);

					Color specular_color = {phong_coef * obj_material.specular[0] * light_color.R,
											phong_coef * obj_material.specular[1] * light_color.G, 
											phong_coef * obj_material.specular[2] * light_color.B};

					color = {color.R + specular_color.R, color.G + specular_color.G, color.B + specular_color.B};
				}

				if (ray.recurs_lvl != recurs_lvl && SHINE_MIN < obj_material.shine) {
					Ray new_ray = {first_collision.position - ray.front * eps,
									normalize(ray.front - hit_normal * scalarProduct(ray.front, hit_normal) * 2.0f),
									new_ray.recurs_lvl = ray.recurs_lvl + 1};

					Color specular_color = {obj_material.specular[0], 
											obj_material.specular[1],
											obj_material.specular[2]};

					auto shade_color = getShadeColor(new_ray);
		
					color = {color.R + specular_color.R * shade_color.R, 
							 color.G + specular_color.G * shade_color.G, 
							 color.B + specular_color.B * shade_color.B};
				}
			}
		}
	}

	return color;
}

void Editor::trace(const Vector3f& position, GLfloat aspect) {
	GLfloat N = 0.1f;
	GLfloat trace_height = N * std::tanf(60.0f * (PI_F / 180.0f) / 2.0);
	GLfloat trace_width = trace_height * aspect;

	Vector3f n = normalize(camera.getPosition() - camera.getFront());
	Vector3f u = normalize(cross(camera.getUp(), n));
	Vector3f v = normalize(cross(n, u));

	for (GLint i = 0; i < height; i += pixel_size) {
		for (GLint j = 0; j < width; j += pixel_size) {
			Ray ray = {position, {}, 0};

			GLfloat x = -trace_width + (j * 2 * trace_width) / width;
			GLfloat y = -trace_height + (i * 2 * trace_height) / height;

			auto color = ColorElem::BLACK;

			GLfloat coef = 0.0001f;
			if (smoothing) {
				for (GLubyte k = 0; k != 2; ++k) {
					auto front = Vector3f(-N * n.getX() + x * u.getX() + y * v.getX() + coef,
										  -N * n.getY() + x * u.getY() + y * v.getY() - coef,
										  -N * n.getZ() + x * u.getZ() + y * v.getZ() - coef);

					ray.front = normalize(front);
					auto shade_color = getShadeColor(ray);
					color = {color.R + shade_color.R, color.G + shade_color.G, color.B + shade_color.B};

					front = Vector3f(-N * n.getX() + x * u.getX() + y * v.getX() + coef,
									 -N * n.getY() + x * u.getY() + y * v.getY() + coef,
									 -N * n.getZ() + x * u.getZ() + y * v.getZ() - coef);

					ray.front = normalize(front);
					shade_color = getShadeColor(ray);
					color = {color.R + shade_color.R, color.G + shade_color.G, color.B + shade_color.B };

					coef = -coef;
				}

				color = {color.R / 4.0f, color.G / 4.0f, color.B / 4.0f};
			}
			else {
				auto front = Vector3f(-N * n.getX() + x * u.getX() + y * v.getX() + coef,
									  -N * n.getY() + x * u.getY() + y * v.getY() - coef,
									  -N * n.getZ() + x * u.getZ() + y * v.getZ() - coef);
				
				ray.front = normalize(front);
				auto shade_color = getShadeColor(ray);
				color = {color.R + shade_color.R, color.G + shade_color.G, color.B + shade_color.B};
			}

			ray_tracing_field[i][j] = color;
		}
	}

	for (GLint i = 0; i < height; i += pixel_size)
		for (GLint j = 0; j < width; j += pixel_size) {
			glColor3f(ray_tracing_field[i][j].R, ray_tracing_field[i][j].G, ray_tracing_field[i][j].B);
			glRecti(j, i, j + pixel_size, i + pixel_size);
		}
}