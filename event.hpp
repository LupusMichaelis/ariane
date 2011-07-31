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
} /* class Event */ ;

class MouseEvent
	: public Event
{
	public:
		MouseEvent(Size const & position)
			: m_position(position) { }

		Size const & position() const
		{
			return m_position ;
		}

	private:
		const Size m_position ;
} /* class Event */ ;

class EventLoop
{
	public:
		typedef boost::signal<void (KeyEvent const &)> keyboard_event_type ;
		typedef boost::signal<void (MouseEvent const &)> mouse_event_type ;

		void operator() () const ;

		void attach_event(char const * event, keyboard_event_type::slot_function_type const & fn) ;
		void attach_event(char const * event, mouse_event_type::slot_function_type const & fn) ;

	private:
		mouse_event_type m_onmouseup ;
		mouse_event_type m_onmousedown ;
		mouse_event_type m_onmousemove ;

		keyboard_event_type m_onkeyup ;
		keyboard_event_type m_onkeydown ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
