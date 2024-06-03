#include "pch.h"
#include "Application.h"

int main(int arvc, char** args)
{
	Application* application = new Application("Engine 3D", 1280, 720);
	application->Run();
	delete application;
	return 0;
}