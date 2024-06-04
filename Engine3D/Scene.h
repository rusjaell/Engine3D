#ifndef GAME_H
#define GAME_H

#include "Camera.h"

class Event;
class Scene
{
public:
	Scene();
	~Scene();

	void OnEvent(Event& event);
	void OnImGuiRender(double time, double dt);
	void OnUpdate(double time, double dt);
	void OnRender(double time, double dt);

	void SetViewport(unsigned int width, unsigned int height);

private:
	unsigned int width_;
	unsigned int height_;
};

#endif // !GAME_H
