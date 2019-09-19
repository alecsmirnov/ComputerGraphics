#include "Editor.h"

int main(int argc, char* argv[]) {
	Editor editor(1024, 768);

	editor.addTexture("textures/1.bmp");
	editor.addTexture("textures/2.bmp");
	editor.addTexture("textures/3.bmp");


	editor.readInputFiles("input/objects.txt", "input/lights.txt");

	editor.start(&argc, argv);

	return 0;
}