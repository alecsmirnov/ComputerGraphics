#ifndef EDITOR_H
#define EDITOR_H

#include <string>

#include "Sphere.h"
#include "KeyButtons.h"
#include "Color.h"
#include "BMPTextureLoaderLight.h"

class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	void addTexture(const char* image);

	void start(int* argc, char* argv[]);

private:
	static void displayEvent();
	static void reshapeEvent(GLint new_width, GLint new_height);
	static void keyboardEvent(std::uint8_t key, GLint x, GLint y);
	static void mouseWheelEvent(GLint button, GLint dir, GLint x, GLint y);

	static void moveCameraX(GLfloat speed);
	static void moveCameraY(GLfloat speed);
	static void cameraScale(GLfloat factor);

	static void changeViewType();
	static void changeRadius(GLfloat radius_coef);
	static void changeSectors(GLint sector_coef);
	static void changeStacks(GLint stacks_coef);
	static void changeTexture();
	static void changeColor();

	static void drawText(std::string text, GLint x, GLint y);
	static void drawInfo();

	static std::string valToStr(GLfloat val, std::uint8_t precision);

	static std::string textureInfo(std::vector<GLuint>::size_type num);
	static std::string colorNameByNum(std::uint8_t num);

private:
	enum class ViewType {
		FILL,
		LINE
	};

private:
	static inline const char* window_title = "Sphere";

	static inline GLint width;
	static inline GLint height;
	
	static inline Sphere sphere;

	static inline GLfloat camera_x;
	static inline GLfloat camera_y;
	static inline GLfloat camera_scale;

	static inline ViewType view_type;

	static inline std::uint8_t color_num;

	static inline TextureLoader texture_loader;
	static inline GLuint current_texture;
};

#endif