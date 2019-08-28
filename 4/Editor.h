#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <GL/freeglut.h>

#include "Camera.h"
#include "Sphere.h"
#include "Prism.h"
#include "Plane.h"
#include "LightSource.h"

class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	void readInputFiles(std::string objects_filename, std::string light_sources_filename);
	void start(int* argc, char* argv[]);

	~Editor();

private:
	static void readObjectsFile(std::string filename);
	static void readLightSourcesFile(std::string filename);

	static bool isShadow(Ray ray);
	static std::vector<Collision> getCollisions(Ray ray);
	static Color getShadeColor(Ray ray);
	static void trace(const Vector3f& position, GLfloat aspect);

	static void nextFigure();
	static void prevFigure();
	static void changeFigureVisibility();
	static void nextLight();
	static void prevLight();
	static void lightMoveForward();
	static void lightMoveBack();
	static void lightMoveLeft();
	static void lightMoveRight();
	static void lightMoveUp();
	static void lightMoveDown();
	static void increasePixelSize();
	static void decreasePixelSize();
	static void selectionSwitch();
	static void smoothingSwitch();

	static void drawGrid();
	static void drawScene();

	static void modelViewSwitch();
	static void rayTracingFieldResize(GLint new_width, GLint new_height);
	static void rayTracingFieldDelete();

	static void displayEvent();
	static void reshapeEvent(GLint new_width, GLint new_height);
	static void keyboardEvent(std::uint8_t key, int x, int y);
	static void mouseMoveEvent(int x, int y);
	
private:
	static inline const char* window_title = "Ray tracing";

	static inline GLint width;
	static inline GLint height;

	static inline Camera camera;

	static inline Color** ray_tracing_field;
	static inline std::vector<Figure*> figures;
	static inline std::vector<LightSource> light_sources;

	static inline std::vector<Figure*>::size_type current_figure;
	static inline std::vector<LightSource>::size_type current_light;
	static inline GLint pixel_size;

	static inline bool selection;
	static inline bool smoothing;
	static inline bool ray_tracing;
};

#endif