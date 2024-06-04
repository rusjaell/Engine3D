#ifndef EDITOR_H
#define EDITOR_H

#include "Event.h"
#include "FrameBuffer.h"

class Scene;
class Editor
{
public:
	Editor();
	~Editor();

	void OnEvent(Event& event);
	void OnImGuiRender(double time, double dt);
	void OnUpdate(double time, double dt);
	void OnRender(double time, double dt);

private:
	void RenderViewport();
	void RenderHierarchy();
	void RenderConsole();
	void RenderInspector();
	void RenderDebug();

private:
	Scene* scene_;

	glm::vec2 viewportSize_;
	Shared< FrameBuffer> viewportFrameBuffer_;

	glm::vec3 backgroundColor_ = { 0.2, 0.2, 0.2 };
};

#endif