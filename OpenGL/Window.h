#ifndef WINDOW_H
#define WINDOW_H

#include "GraphicsContext.h"
#include "Event.h"

class Window
{
	using EventCallbackFunction = std::function<void(Event&)>;

	struct WindowData
	{
		std::string title;
		unsigned int width, height;
		bool vsync;

		EventCallbackFunction EventCallback;
	};

public:
	Window(const char* title, unsigned int width, unsigned int height);
	~Window();

	void OnUpdate();
	void Close();
	void SetEventCallback(const EventCallbackFunction& callback);

	bool SetWindowCenter(GLFWwindow* window);

	unsigned int width() const;
	unsigned int height() const;
	bool isClosable() const;

	GLFWwindow* window() const;

private:
	GLFWwindow* window_;
	GraphicsContext* graphicsContext_;
	WindowData data_;
};

#endif // !WINDOW_H
