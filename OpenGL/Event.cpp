#include "pch.h"
#include "Event.h"

MouseMovedEvent::MouseMovedEvent(const float x, const float y)
	: x_(x), y_(y)
{
}

EventType MouseMovedEvent::staticType()
{
	return EventType::MouseMoved;
}

EventType MouseMovedEvent::type() const
{
	return staticType();
}

int MouseMovedEvent::catagory() const
{
	return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput;
}

std::string MouseMovedEvent::to_string() const
{
	std::stringstream ss;
	ss << "[MouseMovedEvent] X: " << x_ << " Y: " << y_;
	return ss.str();
}

float MouseMovedEvent::x() const
{
	return x_;
}

float MouseMovedEvent::y() const
{
	return y_;
}

MouseScrolledEvent::MouseScrolledEvent(const float offsetX, const float offsetY)
	: offsetX_(offsetX), offsetY_(offsetY)
{
}

EventType MouseScrolledEvent::staticType()
{
	return EventType::MouseScrolled;
}

EventType MouseScrolledEvent::type() const
{
	return staticType();
}

int MouseScrolledEvent::catagory() const
{
	return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput;
}

std::string MouseScrolledEvent::to_string() const
{
	std::stringstream ss;
	ss << "[MouseScrolledEvent] OffsetX: " << offsetX_ << " OffsetY: " << offsetY_;
	return ss.str();
}

float MouseScrolledEvent::offsetX() const
{
	return offsetX_;
}

float MouseScrolledEvent::offsetY() const
{
	return offsetY_;
}

MouseButtonEvent::MouseButtonEvent(const MouseCode button, const bool pressed)
	: button_(button), pressed_(pressed)
{
}

MouseCode MouseButtonEvent::button() const
{
	return button_;
}

bool MouseButtonEvent::pressed() const
{
	return pressed_;
}

EventType MouseButtonEvent::staticType()
{
	return EventType::MouseButton;
}

EventType MouseButtonEvent::type() const
{
	return staticType();
}

int MouseButtonEvent::catagory() const
{
	return EventCategory::EventCategoryMouse | EventCategory::EventCategoryMouseButton | EventCategory::EventCategoryInput;
}

std::string MouseButtonEvent::to_string() const
{
	std::stringstream ss;
	ss << "[MouseButtonEvent] Button: " << button_ << " Pressed: " << pressed_;
	return ss.str();
}

WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height)
	: width_(width), height_(height)
{
}

unsigned int WindowResizeEvent::width() const
{
	return width_;
}

unsigned int WindowResizeEvent::height() const
{
	return height_;
}

EventType WindowResizeEvent::staticType()
{
	return EventType::WindowResize;
}

EventType WindowResizeEvent::type() const
{
	return staticType();
}

int WindowResizeEvent::catagory() const
{
	return EventCategory::EventCategoryWindow;
}

std::string WindowResizeEvent::to_string() const
{
	std::stringstream ss;
	ss <<  "[WindowResizeEvent] Width: " << width_ << " Height: " << height_;
	return ss.str();
}

EventType WindowCloseEvent::staticType()
{
	return EventType::WindowClose;
}

EventType WindowCloseEvent::type() const
{
	return staticType();
}

int WindowCloseEvent::catagory() const
{
	return EventCategory::EventCategoryWindow;
}

std::string WindowCloseEvent::to_string() const
{
	std::stringstream ss;
	ss << "[WindowCloseEvent]";
	return ss.str();
}

KeyboardEvent::KeyboardEvent(KeyCode keyCode, const bool pressed, const unsigned short repeatCount, bool typed)
	: keyCode_(keyCode), pressed_(pressed), repeatCount_(repeatCount), typed_(typed)
{
}

KeyCode KeyboardEvent::keyCode() const
{
	return keyCode_;
}

bool KeyboardEvent::pressed() const
{
	return pressed_;
}

unsigned short KeyboardEvent::repeatCount() const
{
	return repeatCount_;
}

bool KeyboardEvent::typed() const
{
	return typed_;
}

EventType KeyboardEvent::staticType()
{
	return EventType::Keyboard;
}

EventType KeyboardEvent::type() const
{
	return staticType();
}

int KeyboardEvent::catagory() const
{
	return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput;
}

std::string KeyboardEvent::to_string() const
{
	std::stringstream ss;
	ss << "[KeyboardEvent] KeyCode: " << keyCode_ << " Pressed: " << pressed_ << " RepeatCount " << repeatCount_ << " Typed: " << typed_;
	return ss.str();
}