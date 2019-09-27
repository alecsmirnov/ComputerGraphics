#include "Sphere.h"

#include <windows.h>
#include <cmath>

static constexpr GLuint SECTOR_COUNT_MIN = 3;
static constexpr GLuint STACK_COUNT_MIN  = 2;

static constexpr GLenum INDICES_COUNT    = 3;

static constexpr GLfloat PI = 3.1415926f;

Sphere::Sphere(GLfloat radius, GLuint sector_count, GLuint stack_count) {
    init(radius, sector_count, stack_count);
}

void Sphere::init(GLfloat radius, GLuint sector_count, GLuint stack_count) {
    this->radius = radius;
    this->sector_count = SECTOR_COUNT_MIN < sector_count ? sector_count : SECTOR_COUNT_MIN;
	this->stack_count = STACK_COUNT_MIN < stack_count ? stack_count : STACK_COUNT_MIN;

	buildVertices();
}

void Sphere::setSectorCount(GLuint sector_count) {
	init(radius, sector_count, stack_count);
}

void Sphere::setStackCount(GLuint stack_count) {
	init(radius, sector_count, stack_count);
}

GLfloat Sphere::getRadius() const { 
	return radius; 
}

GLuint Sphere::getSectorCount() const {
	return sector_count; 
}

GLuint Sphere::getStackCount() const {
	return stack_count; 
}

void Sphere::draw(GLuint texture) const {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vec3<GLfloat>), vertices.data());
    glTexCoordPointer(2, GL_FLOAT, sizeof(vec2<GLfloat>), textures_coord.data());

	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, indices.size() * INDICES_COUNT, GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Sphere::clear() {
	std::vector<vec3<GLfloat>>().swap(vertices);
	std::vector<vec3<GLuint>>().swap(indices);

	std::vector<vec2<GLfloat>>().swap(textures_coord);
}

// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
void Sphere::buildVertices() {
    clear();

	GLfloat sector_step = 2 * PI / sector_count;
	GLfloat stack_step  = PI / stack_count;

    for (GLuint i = 0; i != stack_count + 1; ++i) {
		GLfloat stack_angle = PI / 2 - i * stack_step;			// [pi/2, -pi/2]

		GLfloat xy = radius * std::cosf(stack_angle);			// r * cos(u)
		GLfloat z = radius * std::sinf(stack_angle);			// r * sin(u)

        for (GLuint j = 0; j != sector_count + 1; ++j) {
			GLfloat sector_angle = j * sector_step;				// [0, 2pi]

			GLfloat x = xy * std::cosf(sector_angle);			// r * cos(u) * cos(v)
			GLfloat y = xy * std::sinf(sector_angle);			// r * cos(u) * sin(v)
            addVertex({x, y, z});

			GLfloat u = 1.0f * j / sector_count;
			GLfloat v = 1.0f * i / stack_count;
            addTextureCoord({u, v});
        }
    }

	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
    for (GLuint i = 0; i != stack_count; ++i) {
		GLuint k1 = i * (sector_count + 1);						// Начало текущего сектора
		GLuint k2 = k1 + sector_count + 1;						// Начало следующего сектора

        for (GLuint j = 0; j != sector_count; ++j, ++k1, ++k2) {
            if (i != 0)
                addIndices({k1, k2, k1 + 1});					// k1---k2---k1+1

            if (i != stack_count - 1)
                addIndices({k1 + 1, k2, k2 + 1});				// k1+1---k2---k2+1
        }
    }
}

void Sphere::addVertex(const vec3<GLfloat>& vertex) {
 	vertices.push_back(vertex);
}

void Sphere::addIndices(const vec3<GLuint>& index) {
	indices.push_back(index);
}

void Sphere::addTextureCoord(const vec2<GLfloat>& texture) {
	textures_coord.push_back(texture);
}

Sphere::~Sphere() {
	clear();
}