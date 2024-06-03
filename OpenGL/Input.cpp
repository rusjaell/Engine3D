#include "pch.h"
#include "Event.h"
#include "Application.h"

float Input::mouseX_ = 0.0f;
float Input::mouseY_ = 0.0f;
float Input::scrollDeltaX_ = 0.0f;
float Input::scrollDeltaY_ = 0.0f;
std::unordered_map<KeyCode, bool> Input::keys_;
std::unordered_map<KeyCode, bool> Input::repeatingKeys_;
std::unordered_map<MouseCode, bool> Input::buttons_;

bool Input::OnKeyEvent(KeyboardEvent& event)
{
	keys_[event.keyCode()] = event.pressed();
	repeatingKeys_[event.keyCode()] = event.repeatCount() > 0;
	return false;
}

bool Input::OnMouseMoveEvent(MouseMovedEvent& event)
{
	mouseX_ = event.x();
	mouseY_ = event.y();
	return false;
}

bool Input::OnMouseScrollEvent(MouseScrolledEvent& event)
{
	scrollDeltaX_ = event.offsetX();
	scrollDeltaY_ = event.offsetY();
	return false;
}

bool Input::OnMouseButtonEvent(MouseButtonEvent& event)
{
	buttons_[event.button()] = event.pressed();
	return false;
}

bool Input::IsKeyUp(KeyCode key)
{
	return !keys_[key];
}

bool Input::IsKeyRepeating(KeyCode key)
{
	return !repeatingKeys_[key];
}

bool Input::IsKeyDown(KeyCode key)
{
	return keys_[key];
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	return buttons_[button];
}

bool Input::IsMouseButtonReleased(MouseCode button)
{
	return !buttons_[button];
}

std::pair<float, float> Input::mousePosition()
{
	return { (float)mouseX_, (float)mouseY_ };
}

float Input::mouseX()
{
	return mouseX_;
}

float Input::mouseY()
{
	return mouseY_;
}

std::pair<float, float> Input::scrollDelta()
{
	return { scrollDeltaX_, scrollDeltaY_ };
}

float Input::scrollDeltaX()
{
	float deltaX = scrollDeltaX_;
	scrollDeltaX_ = 0.0f;
	return deltaX;
}

float Input::scrollDeltaY()
{
	float deltaY = scrollDeltaY_;
	scrollDeltaY_ = 0.0f;
	return deltaY;
}