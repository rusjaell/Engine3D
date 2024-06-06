#include "Engine.h"

int main(int arvc, char** args)
{
	Application* application = new Application("Editor", 1280, 720);
	application->Run();
	delete application;
	return 0;
}