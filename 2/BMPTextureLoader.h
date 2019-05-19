#ifndef BMPTEXTURELOADER_H
#define BMPTEXTURELOADER_H

#include <vector>
#include <cstdint>
#include <GL/freeglut.h>

#include "Point.h"

// Загрузчик текстур для правильного шестиугольника
class TextureLoader {
public:
	TextureLoader() = default;

	// Установка координат текстур
	void setCoordnates(std::vector<Point<GLfloat>> texture_coords);
	// Установка текстур
	void setTextures(std::vector<GLuint> textures);

	// Получить координаты текстур
	std::vector<Point<GLfloat>> getCoordinates() const;
	// Получить текстуры
	std::vector<GLuint> getTextures() const;

	// Добавить путь до текстуры
	void addTexture(const char* image);

	// Инициализация добавленных текстур
	void initAddedTextures();

	// Очистка текстур
	void clearTextures();

	~TextureLoader();

private:
	std::vector<Point<GLfloat>> texture_coords;

	std::vector<const char*> textures_name;
	std::vector<GLuint> textures;
};

#endif