#include "Prism.h"

#include <cmath>

static constexpr GLfloat FRAME_FAULT = 0.07f;

static constexpr GLubyte SIDE_VERTICES = 4;
static constexpr GLubyte SIDES_COUNT   = 6;

static constexpr GLfloat TIME_UNITIALIZED = -1.0f;

static constexpr std::array<std::array<GLbyte, SIDE_VERTICES>, SIDES_COUNT> PLANES_COORD = {0, 1, 2, 3,  // Плоскость снизу
																							7, 6, 2, 3,  // Плоскость слева 
																							7, 3, 0, 4,  // Дальняя плоскость
																							0, 1, 5, 4,  // Плоскость справа
																							2, 1, 5, 6,  // Плоскость спереди
																							4, 5, 6, 7}; // Плоскость сверху

Prism::Prism(const std::array<Vector3f, vertices>& coords, const Vector3f& center, Color color, Material material) {
	this->coords = coords;
	this->center = center;
	this->color = color;
	this->material = material;

	visibility = true;
}

void Prism::setMaterial(Material material) {
	this->material = material;
}

void Prism::setVisibility(bool visibility) {
	this->visibility = visibility;
}

Material Prism::getMaterial() const {
	return material;
}

bool Prism::getVisibility() const {
	return visibility;
}

void Prism::draw() {
	glPushMatrix();
	glTranslated(center.getX(), center.getY(), center.getZ());

	glColor3f(color.R, color.G, color.B);
	for (GLubyte i = 0; i != SIDES_COUNT; ++i) {
		glBegin(GL_QUADS);

		for (GLubyte j = 0; j != SIDE_VERTICES; ++j)
			glVertex3f(coords[PLANES_COORD[i][j]].getX(), coords[PLANES_COORD[i][j]].getY(), coords[PLANES_COORD[i][j]].getZ());

		glEnd();
	}

	glPopMatrix();
}

void Prism::drawFrame() {
	glPushMatrix();
	glTranslated(center.getX(), center.getY(), center.getZ());

	auto frame = [&](GLfloat frame_width, Color frame_color) {
		glLineWidth(frame_width);
		glColor3f(frame_color.R, frame_color.G, frame_color.B);

		for (GLubyte i = 0; i != SIDES_COUNT; ++i) {
			glBegin(GL_LINE_LOOP);

			for (GLubyte j = 0; j != SIDE_VERTICES; ++j)
				glVertex3f(coords[PLANES_COORD[i][j]].getX() * FRAME_COEF - FRAME_FAULT,
						   coords[PLANES_COORD[i][j]].getY() * FRAME_COEF - FRAME_FAULT,
						   coords[PLANES_COORD[i][j]].getZ() * FRAME_COEF - FRAME_FAULT);

			glEnd();
		}

		glLineWidth(1.0f);
	};

	frame(INNER_FRAME_WIDTH, INNER_FRAME_COLOR);
	frame(OUTER_FRAME_WIDTH, OUTER_FRAME_COLOR);

	glPopMatrix();
}

bool Prism::isHit(Ray ray, std::vector<Collision>& collisions) {
	bool hit = false;

	std::array<GLfloat, SIDES_COUNT> collision_time;
	std::array<Vector3f, SIDES_COUNT> normal_planes;

	auto new_ray = transformRay(ray, center, Vector3f(1.0f, 1.0f, 1.0f));

	GLubyte hit_count = 0;
	for (GLubyte i = 0; i != SIDES_COUNT; ++i)
		if (!isPlaneHit(new_ray, PLANES_COORD[i][0], PLANES_COORD[i][1], PLANES_COORD[i][2], PLANES_COORD[i][3], 
		   normal_planes[i], collision_time[i]))
			collision_time[i] = TIME_UNITIALIZED;
		else
			++hit_count;

	if (0 < hit_count) {
		GLubyte max_time_id = 0;

		for (GLubyte i = 1; i != SIDES_COUNT; ++i)
			if (collision_time[max_time_id] < collision_time[i])
				max_time_id = i;

		GLubyte min_time_id = max_time_id;
		for (GLubyte i = 0; i != SIDES_COUNT; ++i)
			if (0.0f < collision_time[i] && collision_time[i] < collision_time[min_time_id])
				min_time_id = i;

		if (0.0f <= collision_time[max_time_id]) {
			std::array<GLubyte, COLLISIONS_MAX> times = {min_time_id, max_time_id};
			std::array<bool, COLLISIONS_MAX> enters = {true, false};

			collisions.resize(COLLISIONS_MAX);

			for (GLubyte i = 0; i != COLLISIONS_MAX; ++i) {
				collisions[i].time = collision_time[times[i]];
				collisions[i].entering = enters[i];
				collisions[i].position = getRayPosition(ray, collision_time[times[i]]);
				collisions[i].normal = normal_planes[times[i]];
			}

			hit = true;
		}
	}

	return hit;
}

bool Prism::isSimpleHit(Ray ray) {
	bool hit = false;

	GLfloat collision_time;
	Vector3f normal_plane;

	auto new_ray = transformRay(ray, center, Vector3f(1, 1, 1));

	for (GLubyte i = 0; i != SIDES_COUNT && !hit; ++i)
		if (isPlaneHit(new_ray, PLANES_COORD[i][0], PLANES_COORD[i][1], PLANES_COORD[i][2], PLANES_COORD[i][3], 
		   normal_plane, collision_time))
			if (0.0f <= collision_time && collision_time <= 1.0f)
				hit = true;

	return hit;
}

bool Prism::isInPlane(const std::array<GLfloat, 4>& x, GLfloat xp, const std::array<GLfloat, 4>& y, GLfloat yp) {
	bool hit = false;

	for (GLubyte i = 0, j = SIDE_VERTICES - 1; i != SIDE_VERTICES; j = i++)
		if (((y[i] <= yp && yp < y[j]) || (y[j] <= yp && yp < y[i])) && 
		   (xp > (x[j] - x[i]) * (yp - y[i]) / (y[j] - y[i]) + x[i]))
			hit = !hit;

	return hit;
}

bool Prism::isPlaneHit(Ray ray, GLbyte ver1, GLbyte ver2, GLbyte ver3, GLbyte ver4, Vector3f& normal, GLfloat& time) {
	bool hit = true;

	auto position = ray.position;
	auto front = ray.front;

	auto a = coords[ver1];
	auto b = coords[ver2];
	auto c = coords[ver3];
	auto d = coords[ver4];

	auto new_normal = polygonNormal(d, c, b);
	auto vn = scalarProduct(front, new_normal);

	Vector3f V = position - a;
	auto cur_time = -scalarProduct(V, new_normal) / vn;
	V = front * cur_time;
	auto O = position + V;

	if (std::fabsf(new_normal.getY()) <= std::fabsf(new_normal.getX()) && std::fabsf(new_normal.getZ()) <= std::fabsf(new_normal.getX())) {
		if (!isInPlane({a.getY(), b.getY(), c.getY(), d.getY()}, O.getY(), {a.getZ(), b.getZ(), c.getZ(), d.getZ()}, O.getZ()))
			hit = false;
	}
	else 
		if (std::fabsf(new_normal.getY()) <= std::fabsf(new_normal.getZ())) {
			if (!isInPlane({a.getX(), b.getX(), c.getX(), d.getX()}, O.getX(), {a.getY(), b.getY(), c.getY(), d.getY()}, O.getY()))
				hit = false;
		}
		else
			if (!isInPlane({a.getX(), b.getX(), c.getX(), d.getX()}, O.getX(), {a.getZ(), b.getZ(), c.getZ(), d.getZ()}, O.getZ()))
				hit = false;

	if (hit) {
		normal = new_normal;
		time = cur_time;
	}

	return hit;
}