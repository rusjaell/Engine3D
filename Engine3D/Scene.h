#ifndef GAME_H
#define GAME_H

#include "Camera.h"

class Event;
class Scene
{
public:
	Scene();
	~Scene();

	void GeometryPass();
	void ShadowPass();
	void LightingPass();
	void PostProcessingPass();

	void OnEvent(Event& event);
	void OnImGuiRender(double time, double dt);
	void OnUpdate(double time, double dt);
	void OnRender(double time, double dt);

private:

};

#endif // !GAME_H
