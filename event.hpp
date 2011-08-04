#ifndef HPP_EVENT_SDL_QUEST
#	define HPP_EVENT_SDL_QUEST

#	include <functional>
#	include <boost/signal.hpp>

#	include "videomode.hpp"

class Event
{
	public:
} /* class Event */ ;

class KeyEvent
	: public Event
{
	public:
		KeyEvent(char const key)
			: m_key(key) { }

		char const key() const
		{
			return m_key ;
		}

	private:
		const char m_key ;
} /* class KeyEvent */ ;

#include <bitset>

class MouseEvent
	: public Event
{
	public:
		MouseEvent(Size const & position, std::bitset<3> const & buttons = 0x0)
			: m_position(position)
			, m_buttons(buttons)
		{ }

		Size const & position() const
		{
			return m_position ;
		}

		bool const button(const int idx) const
		{
			return m_buttons[idx] ;
		}

	private:
		Size const m_position ;
		std::bitset<3> const m_buttons ;

} /* class MouseEvent */ ;

class MouseMoveEvent
	: public MouseEvent
{
	public:
		MouseMoveEvent(Size const & position, std::bitset<3> const buttons = 0x0)
			: MouseEvent(position, buttons)
		{ }

} /* class MouseMoveEvent */ ;

class MouseClickEvent
	: public MouseEvent
{
	public:
		MouseClickEvent(Size const & position, std::bitset<3> const buttons = 0x0)
			: MouseEvent(position, buttons)
		{ }

} /* class MouseClickEvent */ ;

class EventLoop
{
	public:
		typedef boost::signal<void (KeyEvent const &)> keyboard_event_type ;
		typedef boost::signal<void (MouseMoveEvent const &)> mousemove_event_type ;
		typedef boost::signal<void (MouseClickEvent const &)> mouseclick_event_type ;

		void operator() () const ;

		void attach_event(keyboard_event_type::slot_function_type const & fn) ;
		void attach_event(mousemove_event_type::slot_function_type const & fn) ;
		void attach_event(mouseclick_event_type::slot_function_type const & fn) ;

	private:
		keyboard_event_type m_onkey ;

		mouseclick_event_type m_onmouseclick ;
		mousemove_event_type m_onmousemove ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
