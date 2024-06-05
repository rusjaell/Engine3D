#include "pch.h"
#include "Application.h"
#include "Shader.h"
#include "Scene.h"
#include "Input.h"
#include "Editor.h"

Application* Application::instance_ = nullptr;
int Application::draws_ = 0;
int Application::vertices = 0;

Application::Application(const char* title, unsigned int width, unsigned int height)
	: isRunning_(true), lastFrameTime_(0.0), lastFPS_(0), lastUPS_(0), time_(0.0)
{
	instance_ = this;

	window_ = new Window(title, width, height);
	window_->SetEventCallback(BIND_FN(Application::OnEvent));

	imGUIDebug_ = new ImGUIDebug();
	scene_ = new Editor();
}

Application::~Application()
{
	delete imGUIDebug_;
	delete scene_;
	delete window_;
}

void Application::Run()
{
	glfwSwapInterval(0);

	const double UPS = 60.0; // Updates per second

	const double updateInterval = 1.0 / UPS; // Fixed update interval

	double accumulator = 0.0; // Accumulator for the elapsed time

	int frameCount = 0; // Frame counter
	int updateCount = 0; // Update counter
	double fpsTimer = 0.0; // Timer to track FPS/UPS printing interval

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor( 1, 0, 0, 1 );

	while (isRunning_) {

		double time = glfwGetTime();
		double deltaTime = time - lastFrameTime_;
		lastFrameTime_ = time;

		time_ += deltaTime;

		accumulator += deltaTime;
		fpsTimer += deltaTime;

		while (accumulator >= updateInterval) {
			scene_->OnUpdate(time_, updateInterval);
			accumulator -= updateInterval; // Decrease the accumulator
			updateCount++; // Increment update counter
		}

		scene_->OnRender(time_, deltaTime);

		imGUIDebug_->Begin();
		scene_->OnImGuiRender(time_, deltaTime);
		imGUIDebug_->End();
		
		frameCount++;

		window_->OnUpdate();

		isRunning_ = !window_->isClosable();

		if (fpsTimer >= 1.0) {

			lastFPS_ = frameCount;
			lastUPS_ = updateCount;
			
			std::cout << "FPS: " << lastFPS_ << " UPS: " << lastUPS_ << " DRAWS: " << draws_ << " VERTICES: " << vertices << '\n';

			frameCount = 0;
			updateCount = 0;
			fpsTimer = 0.0;
		}

		vertices = 0;
		draws_ = 0;
	}
}

void Application::Stop()
{
	isRunning_ = false;
}

void Application::OnEvent(Event& e)
{
	imGUIDebug_->OnEvent(e);
	if (e.handled_)
		return;
	scene_->OnEvent(e);
}

Window& Application::window()
{
	return *window_;
}

void Application::ToggleCursor()
{
	toggleCursor_ = !toggleCursor_;
	if (toggleCursor_)
		glfwSetInputMode(window_->window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window_->window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}