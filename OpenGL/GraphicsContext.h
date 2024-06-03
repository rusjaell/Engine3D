#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

struct GLFWwindow;
class GraphicsContext
{
public:
	GraphicsContext(GLFWwindow* windowHandle);

	void Init();
	void SwapBuffers();

	GLFWwindow* handle() const;

private:
	GLFWwindow* windowHandle_;

};

#endif // !GRAPHICS_CONTEXT_H
