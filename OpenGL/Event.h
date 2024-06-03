#ifndef  EVENT_H
#define EVENT_H

#include "Input.h"

enum class EventType
{
	None = 0,
	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus, 
	WindowMoved,

	Keyboard,

	MouseMoved,
	MouseScrolled,
	MouseButton
};

enum EventCategory
{
	None = 0,
	EventCategoryWindow = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4)
};

class Event
{
public:
	virtual ~Event() = default;

	virtual EventType type() const = 0;
	virtual int catagory() const = 0;
	virtual std::string to_string() const = 0;

	bool handled_ = false;
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x, const float y);

	float x() const;
	float y() const;

	static EventType staticType();
	virtual EventType type() const override;
	virtual int catagory() const override;
	virtual std::string to_string() const override;

private:
	float x_;
	float y_;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float offsetX, const float offsetY);

	float offsetX() const;
	float offsetY() const;

	static EventType staticType();
	virtual EventType type() const override;
	virtual int catagory() const override;
	virtual std::string to_string() const override;

private:
	float offsetX_;
	float offsetY_;
};

class MouseButtonEvent : public Event
{
public:
	MouseButtonEvent(const MouseCode button, const bool pressed);

	MouseCode button() const;
	bool pressed() const;

	static EventType staticType();
	virtual EventType type() const override;
	virtual int catagory() const override;
	virtual std::string to_string() const override;

private:
	MouseCode button_;
	bool pressed_;
};

class KeyboardEvent : public Event
{
public:
	KeyboardEvent(KeyCode keyCode, const bool pressed, const unsigned short repeatCount, bool typed);

	KeyCode keyCode() const;
	bool pressed() const;
	unsigned short repeatCount() const;
	bool typed() const;

	static EventType staticType();
	virtual EventType type() const override;
	virtual int catagory() const override;
	virtual std::string to_string() const override;

private:
	KeyCode keyCode_;
	bool pressed_;
	unsigned short repeatCount_;
	bool typed_;
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(const unsigned int width, const unsigned int height);

	unsigned int width() const;
	unsigned int height() const;

	static EventType staticType();
	virtual EventType type() const override;
	virtual int catagory() const override;
	virtual std::string to_string() const override;

private:
	unsigned int width_;
	unsigned int height_;
};

class WindowCloseEvent : public Event
{
public:
	static EventType staticType();
	virtual EventType type() const override;
	virtual int catagory() const override;
	virtual std::string to_string() const override;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: event_(event)
	{
	}

	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (event_.type() == T::staticType())
		{
			event_.handled_ |= func(static_cast<T&>(event_));
			return true;
		}
		return false;
	}

private:
	Event& event_;
};
#endif // ! EVENT_H
