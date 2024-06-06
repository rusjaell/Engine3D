#ifndef EDITOR_H
#define EDITOR_H

#include "Scene.h"
#include "FrameBuffer.h"
#include "Application.h"

class Editor : public Application
{
public:
	Editor(const char* title, unsigned int width, unsigned int height);
	~Editor();

	virtual void OnEvent(Event& event) override;
	virtual void OnImGuiRender(double time, double dt) override;
	virtual void OnUpdate(double time, double dt) override;
	virtual void OnRender(double time, double dt) override;

	void RenderScene(double time, double dt);

private:
	void RenderViewport();
	void RenderHierarchy();
	void RenderConsole();
	void RenderInspector();
	void RenderDebug();
	void RenderContent();

private:
	Scene* scene_;

	glm::vec2 viewportSize_;
	Shared<FrameBuffer> viewportFrameBuffer_;

	glm::vec3 backgroundColor_ = { 0.2, 0.2, 0.2 };
};

#endif