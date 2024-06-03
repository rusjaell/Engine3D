#ifndef IM_GUI_LAYER_H
#define IM_GUI_LAYER_H

class Event;
class ImGUIDebug 
{
public:
	ImGUIDebug();
	~ImGUIDebug();

	void Begin();
	void OnEvent(Event& event);
	void End();

	void BlockEvents(bool blockEvents);

	void SetDarkThemeColors();

private:
	bool blockEvents_;
};

#endif