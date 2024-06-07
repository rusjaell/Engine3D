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
	virtual ~Application();

	void Run();
	void Stop();

	void OnEventCallback(Event& e);

	virtual void OnEvent(Event& event) = 0;
	virtual void OnImGuiRender(double time, double dt) = 0;
	virtual void OnUpdate(double time, double dt) = 0;
	virtual void OnRender(double time, double dt) = 0;

	Window& window();

	int frames() const { return lastFPS_; }
	int updates() const { return lastUPS_; }
	double time() const { return time_; }

	static Application& instance() {
		return *instance_;
	}

	bool isCursorEnabled() const {
		return toggleCursor_;
	}

	void ToggleCursor();

	static int draws_;
	static int vertices;

protected:
	Window* window_;
	
private:
	ImGUIDebug* imGUIDebug_;
	
	bool isRunning_;

	double lastFrameTime_;

	double time_;
	int lastFPS_;
	int lastUPS_;

	bool toggleCursor_;

	static Application* instance_;
};

#endif // !APPLICATION_H
