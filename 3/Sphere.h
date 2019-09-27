#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <array>
#include <GL/freeglut.h>

template<typename T> using vec2 = std::array<T, 2>;
template<typename T> using vec3 = std::array<T, 3>;

class Sphere {
public:
	Sphere() = default;
    Sphere(GLfloat radius, GLuint sector_count = 36, GLuint stack_count = 18);

	void init(GLfloat radius, GLuint sector_count, GLuint stack_count);

	GLfloat getRadius() const;
	GLuint getSectorCount() const;
	GLuint getStackCount() const;

    void setSectorCount(GLuint sector_count);
    void setStackCount(GLuint stack_count);

    void draw(GLuint texture) const;     

	~Sphere();

private:
	void addVertex(const vec3<GLfloat>& vertex);
	void addIndices(const vec3<GLuint>& index);
	void addTextureCoord(const vec2<GLfloat>& texture);

    void buildVertices();

	void clear();

private:
	GLfloat radius;

	GLuint sector_count;
	GLuint stack_count;

	std::vector<vec3<GLfloat>> vertices;
	std::vector<vec3<GLuint>>  indices;

	std::vector<vec2<GLfloat>> textures_coord;
};

#endif
