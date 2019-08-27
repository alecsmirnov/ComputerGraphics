#include "Editor.h"

int main(int argc, char* argv[]) {
	Editor editor(1024, 768);

	editor.readInputFiles("input/objects.txt", "input/lights.txt");

	editor.start(&argc, argv);

	return 0;
}