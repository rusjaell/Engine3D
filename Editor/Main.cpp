#include "Engine.h"
#include "Editor.h"

int main(int arvc, char** args)
{
	Editor* application = new Editor("Editor", 1280, 720);
	application->Run();
	delete application;
	return 0;
}