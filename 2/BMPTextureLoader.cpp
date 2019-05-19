#define _CRT_SECURE_NO_WARNINGS
#include "BMPTextureLoader.h"

#include <iostream>

static constexpr uint8_t IMAGE_HEADER_SIZE = 54;	// Размер заголовка BMP файла
static constexpr uint8_t RGB_CMPONENT = 3;

// Установка координат текстур
void TextureLoader::setCoordnates(std::vector<Point<GLfloat>> texture_coords) {
	this->texture_coords = texture_coords;
}

// Установка текстур
void TextureLoader::setTextures(std::vector<GLuint> textures) {
	this->textures = textures;
}

// Получить координаты текстур
std::vector<Point<GLfloat>> TextureLoader::getCoordinates() const {
	return texture_coords;
}

// Получить текстуры
std::vector<GLuint> TextureLoader::getTextures() const {
	return textures;
}

// Добавить путь до текстуры
void TextureLoader::addTexture(const char* image) {
	textures_name.push_back(image);
}

// Инициализация добавленных текстур
void TextureLoader::initAddedTextures() {
	GLuint* loaded_textures = new GLuint[textures_name.size()];
	GLubyte** data = new GLubyte*[textures_name.size()];

	glGenTextures(textures_name.size(), loaded_textures);

	for (uint8_t i = 0; i != textures_name.size(); ++i) {
		FILE* fp;
		if (fp = fopen(textures_name[i], "rb")) {
			GLubyte header[IMAGE_HEADER_SIZE];

			if (fread(header, 1, IMAGE_HEADER_SIZE, fp) == IMAGE_HEADER_SIZE && header[0] == 'B' && header[1] == 'M') {
				GLint width  = *(header + 18);
				GLint height = *(header + 22);

				data[i] = new GLubyte[width * height * RGB_CMPONENT];
				fread(data[i], width * height * RGB_CMPONENT, 1, fp);

				fclose(fp);

				glBindTexture(GL_TEXTURE_2D, loaded_textures[i]);
				gluBuild2DMipmaps(GL_TEXTURE_2D, RGB_CMPONENT, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data[i]);

				textures.push_back(loaded_textures[i]);

				delete[] data[i];
			}
			else
				std::cerr << "Error: incorrect BMP-file!" << std::endl;
		}
		else
			std::cerr << "Error: image can't open!" << std::endl;
	}

	delete loaded_textures;
	delete[] data;
}

// Очистка текстур
void TextureLoader::clearTextures() {
	std::vector<const char*>().swap(textures_name);
	std::vector<GLuint>().swap(textures);
}

TextureLoader::~TextureLoader() {
	clearTextures();
	std::vector<Point<GLfloat>>().swap(texture_coords);
}