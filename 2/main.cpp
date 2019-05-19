#include "Editor.h"

int main(int argc, char* argv[]) {
	Editor editor;

	editor.addTexture("1.bmp");
	editor.addTexture("2.bmp");
	editor.addTexture("3.bmp");

	editor.start(&argc, argv);

	return 0;
}