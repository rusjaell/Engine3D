#include "pch.h"
#include "GraphicsContext.h"

GraphicsContext::GraphicsContext(GLFWwindow* windowHandle)
	: windowHandle_(windowHandle)
{
}

void GraphicsContext::Init()
{
	glfwMakeContextCurrent(windowHandle_);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status) {
		std::cout << "Failed to initialize Glad!\n";
		exit(-2);
	}
}

void GraphicsContext::SwapBuffers()
{
	glfwSwapBuffers(windowHandle_);
}

GLFWwindow* GraphicsContext::handle() const
{
	return windowHandle_;
}
