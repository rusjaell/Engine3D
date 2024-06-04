#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "ImGuiOverlay.h"

class Editor;
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

	bool vSync() const { return vSync_; }

	void SetVSync(bool enabled);

	static Application& instance() {
		return *instance_;
	}

	bool isCursorEnabled() const {
		return toggleCursor_;
	}

	void ToggleCursor();

	static int draws_;
	static int vertices;

private:
	double lastFrameTime_;
	Window* window_;
	ImGUIDebug* imGUIDebug_;
	Editor* scene_;
	bool isRunning_;

	bool vSync_;

	double time_;
	int lastFPS_;
	int lastUPS_;

	bool toggleCursor_;

	static Application* instance_;
};

#endif // !APPLICATION_H
