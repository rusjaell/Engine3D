#include "pch.h"
#include "Window.h"

Window::Window(const char* title, unsigned int width, unsigned int height)
{
    data_.title = title;
    data_.width = width;
    data_.height = height;

	if (!glfwInit()) {
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window_ = glfwCreateWindow(data_.width, data_.height, title, NULL, NULL);

	graphicsContext_ = new GraphicsContext(window_);
	graphicsContext_->Init();

	if (!window_)
	{
		glfwTerminate();
		exit(-1);
	}

    bool centered = SetWindowCenter(window_);
    if (!centered)
    {
        glfwTerminate();
        exit(-1);
    }

	glfwMakeContextCurrent(window_);

    glfwSetWindowSizeLimits(window_, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetWindowUserPointer(window_, &data_);

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            glViewport(0, 0, width, height);

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyboardEvent event((KeyCode)key, true, 0, false);
                Input::OnKeyEvent(event);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyboardEvent event((KeyCode)key, false, 0, false);
                Input::OnKeyEvent(event);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyboardEvent event((KeyCode)key, true, 1, false);
                Input::OnKeyEvent(event);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int keycode)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyboardEvent event((KeyCode)keycode, true, 1, true);
        Input::OnKeyEvent(event);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonEvent event((MouseCode)button, true);
                Input::OnMouseButtonEvent(event);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonEvent event((MouseCode)button, false);
                Input::OnMouseButtonEvent(event);
                data.EventCallback(event);
                break;
            }
            }
        });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            Input::OnMouseScrollEvent(event);
            data.EventCallback(event);
        });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPos, (float)yPos);
            Input::OnMouseMoveEvent(event);
            data.EventCallback(event);
        });
}

Window::~Window() {
	Close();
}

void Window::OnUpdate()
{
	graphicsContext_->SwapBuffers();
    glfwPollEvents();
}

void Window::Close()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void Window::SetEventCallback(const EventCallbackFunction& callback)
{
    data_.EventCallback = callback;
}

bool Window::SetWindowCenter(GLFWwindow* window)
{
    if (!window) {
        return false;
    }

    int sx = 0, sy = 0;
    int px = 0, py = 0;
    int mx = 0, my = 0;
    int monitor_count = 0;
    int best_area = 0;
    int final_x = 0, final_y = 0;

    glfwGetWindowSize(window, &sx, &sy);
    glfwGetWindowPos(window, &px, &py);

    // Iterate throug all monitors
    GLFWmonitor** m = glfwGetMonitors(&monitor_count);
    if (!m)
        return false;

    for (int j = 0; j < monitor_count; ++j)
    {
        glfwGetMonitorPos(m[j], &mx, &my);
        const GLFWvidmode* mode = glfwGetVideoMode(m[j]);
        if (!mode) {
            continue;
        }

        // Get intersection of two rectangles - screen and window
        int minX = std::max(mx, px);
        int minY = std::max(my, py);

        int maxX = std::min(mx + mode->width, px + sx);
        int maxY = std::min(my + mode->height, py + sy);

        // Calculate area of the intersection
        int area = std::max(maxX - minX, 0) * std::max(maxY - minY, 0);

        // If its bigger than actual (window covers more space on this monitor)
        if (area > best_area)
        {
            // Calculate proper position in this monitor
            final_x = mx + (mode->width - sx) / 2;
            final_y = my + (mode->height - sy) / 2;

            best_area = area;
        }
    }

    if (best_area) {
        glfwSetWindowPos(window, final_x, final_y);
    }
    else
    {
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        if (primary)
        {
            const GLFWvidmode* desktop = glfwGetVideoMode(primary);

            if (desktop)
                glfwSetWindowPos(window, (desktop->width - sx) / 2, (desktop->height - sy) / 2);
            else
                return false;
        }
        else
            return false;
    }

    return true;
}

unsigned int Window::width() const
{
	return data_.width;
}

unsigned int Window::height() const
{
	return data_.height;
}

bool Window::isClosable() const {
	return glfwWindowShouldClose(window_);
}

GLFWwindow* Window::window() const
{
	return window_;
}