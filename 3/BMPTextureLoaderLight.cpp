#include "BMPTextureLoaderLight.h"

#include <iostream>

static constexpr GLint IMAGE_HEADER_SIZE = 54;
static constexpr GLint RGB_CMPONENT      = 3;

void TextureLoader::addTexture(const char* image) {
	texture_names.push_back(image);
}

void TextureLoader::init() {
	for (auto name : texture_names)
		textures.push_back(loadTexture(name));
}

void TextureLoader::clearTextures() {
	std::vector<const char*>().swap(texture_names);
	std::vector<GLuint>().swap(textures);
}

std::vector<GLuint> TextureLoader::getTextures() const {
	return textures;
}

TextureLoader::~TextureLoader() {
	clearTextures();
}

GLuint TextureLoader::loadTexture(const char* image) {
	GLuint loaded_textures = 0;

	if (FILE* fp; fp = fopen(image, "rb")) {
		GLubyte header[IMAGE_HEADER_SIZE];

		if (fread(header, 1, IMAGE_HEADER_SIZE, fp) == IMAGE_HEADER_SIZE && header[0] == 'B' && header[1] == 'M') {
			GLint width  = *(int*)&(header[0x12]);
			GLint height = *(int*)&(header[0x16]);
			GLint size   = *(int*)&(header[0x22]);

			if (size == 0)
				size = width * height * RGB_CMPONENT;

			GLubyte* data = new GLubyte[size];

			fread(data, 1, size, fp);

			glGenTextures(1, &loaded_textures);
			glBindTexture(GL_TEXTURE_2D, loaded_textures);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			fclose(fp);
		}
	}

	return loaded_textures;
}