#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "ImGuiOverlay.h"

class Scene;
class Application
{
public:
	Application(const char* title, unsigned int width, unsigned int height);
	~Application();

	void Run();
	void Stop();
	void OnEvent(Event& e);

	Window& window();

	int frames() const { return lastFPS_; }
	int updates() const { return lastUPS_; }
	double time() const { return time_; }

	static Application& instance() {
		return *instance_;
	}

	static int draws_;
	static int vertices;

private:
	double lastFrameTime_;
	Window* window_;
	ImGUIDebug* imGUIDebug_;
	Scene* scene_;
	bool isRunning_;

	double time_;
	int lastFPS_;
	int lastUPS_;

	static Application* instance_;
};

#endif // !APPLICATION_H
