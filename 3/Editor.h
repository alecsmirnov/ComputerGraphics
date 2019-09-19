#ifndef EDITOR_H
#define EDITOR_H

#include <string>

#include "Camera.h"
#include "Sphere.h"
#include "LightSource.h"
#include "BMPTextureLoaderLight.h"

struct Rotation {
	bool active;
	Vector3f coords;
};

class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	void addTexture(const char* image);

	void readInputFiles(std::string objects_filename, std::string light_sources_filename);

	void start(int* argc, char* argv[]);

	~Editor();

private:
	static void readObjectsFile(std::string filename);
	static void readLightSourcesFile(std::string filename);

	static void nextFigure();
	static void prevFigure();

	static void figureMoveForward();
	static void figureMoveBack();
	static void figureMoveLeft();
	static void figureMoveRight();
	static void figureMoveUp();
	static void figureMoveDown();

	static void figureRotateX();
	static void figureRotateY();
	static void figureRotateZ();

	static void changeSphereTexture();

	static void selectionSwitch();
	static void rotationSwitch();

	static void drawText(std::string text, GLint x, GLint y);
	static void drawInfo();
	static void drawGrid();
	static void drawScene();

	static void drawLight(std::vector<LightSource*>::size_type num);

	static void displayEvent();
	static void reshapeEvent(GLint new_width, GLint new_height);
	static void keyboardEvent(std::uint8_t key, GLint x, GLint y);
	static void mouseMoveEvent(GLint x, GLint y);

private:
	static inline const char* window_title = "Scene";

	static inline GLint width;
	static inline GLint height;

	static inline Camera camera;

	static inline std::vector<Sphere*> spheres;
	static inline std::vector<LightSource*> light_sources;

	static inline std::vector<Figure*> figures;
	static inline std::vector<Rotation> figures_rotation;

	static inline std::vector<Figure*>::size_type current_figure;

	static inline bool selection;
	static inline bool light;

	static inline TextureLoader texture_loader;
};

#endif