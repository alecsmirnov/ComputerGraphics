#ifndef BMPTEXTURELOADERLIGHT_H
#define BMPTEXTURELOADERLIGHT_H

#include <vector>
#include <GL/freeglut.h>

class TextureLoader {
public:
	TextureLoader() = default;

	void addTexture(const char* image);
	void init();
	void clearTextures();

	std::vector<GLuint> getTextures() const;

	~TextureLoader();

private:
	GLuint loadTexture(const char* image);

private:
	std::vector<const char*> texture_names;
	std::vector<GLuint> textures;
};

#endif