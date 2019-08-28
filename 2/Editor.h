#ifndef EDITOR_H
#define EDITOR_H

#include <GL/freeglut.h>

#include "BMPTextureLoader.h"
#include "HexagonRasterisation.h"

// Редактор поля шестиугольников
class Editor {
public:
	Editor(GLint width = 640, GLint height = 480);

	// Добавить текстуру к набору текстур шестиугольников
	void addTexture(const char* image);

	// Начать работу редактора
	void start(int* argc, char* argv[]);

	~Editor();

private:
	// Инициализация загрузчика текстур
	static inline void initTextureLoader();

	// Вывод на экран
	static void displayEvent();
	// Изменение размеров окна
	static void reshapeEvent(GLint new_width, GLint new_height);
	// Обработка сообщений от клавиатуры
	static void keyboardEvent(std::uint8_t key, int x, int y);
	// Обработка сообщений от мышки
	static void mouseEvent(int button, int state, int x, int y);

	// Обработка левого клика мыши
	static void clickEvent(int x, int y);

	// Меню изменения текущего шестиугольника
	static void hexagonChangeMenu(int task);
	// Меню изменения цвета шестиугольника
	static void hexagonColorMenu(int task);
	// Меню изменения масштаба шестиугольника
	static void hexagonScaleMenu(int task);
	// Меню изменения операций с цветом 
	static void colorOperationMenu(int task);
	// Меню работы с растеризацией
	static void fieldRasterisationMenu(int task);
	// Главное меню
	static void mainMenu(int task);
	// Обработка меню
	static void menuEvent();

	// Нарисовать вершины с указанным режимом
	static void drawVertices(GLenum mode, const Hexagon& hexagon);
	// Нарисовать полигон
	static void drawPolygon(std::vector<Hexagon>::size_type hex_number);
	// Нарисовать шестиугольник под указанным номером
	static void drawHexagon(std::vector<Hexagon>::size_type hex_number);
	// Нарисовать текущий шестиугольник
	static void drawCurrentHexagon();
	// Нарисовать указатель
	static void drawPointer();

	// Удалить текущий шестиугольник
	static void deleteCurrentHexagon();

	// Выбрать предыдущий шестиугольник
	static void prevHexagon();
	// Выбрать сдедующий шестиугольник
	static void nextHexagon();
	
	// Повернуть текущий шестиугольник
	static void rotateCurrentHexagon(GLfloat degrees);
	// Сдвинуть по оси Х текущий шестиугольник
	static void moveCurrentHexagonXAxis(GLint step);
	// Сдвинуть по оси Y текущий шестиугольник
	static void moveCurrentHexagonYAxis(GLint step);
	// Изменить масштаб текущего шестиугольника
	static void scaleCurrentHexagon(GLfloat factor);
	// Изменить текстуры текущего шестиугольника
	static void changeCurrentHexagonTexture();

	// Изменить цвет объекта (шестиугольник или указатель)
	static void changeObjectColor(Color color);

	// Изменить режим отображения (обычный или растр)
	static void changePresentationView();

private:
	// Тип представления операций над цветами
	enum class ColorOperation {
		NONE,
		AND,
		NAND
	};

	// Тип представления режима отображения
	enum class PresentationView {
		GLUT,
		RASTER
	};

private:
	static inline const char* window_title = "Hexagon rasterization";

	static inline GLint width;
	static inline GLint height;

	static inline std::vector<Hexagon> hexes;
	static inline std::vector<Hexagon>::size_type current_hexagon;

	static inline bool smoothing;

	static inline ColorOperation color_operation;
	static inline PresentationView presentation_view;

	static inline Point<GLint> pointer;
	static inline bool  pointer_activity;
	static inline Color pointer_color;

	static inline TextureLoader texture_loader;

	static inline HexagonRasterisation* field;
};

#endif