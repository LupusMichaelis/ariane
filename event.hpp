#ifndef HPP_EVENT_SDL_QUEST
#	define HPP_EVENT_SDL_QUEST

#	include <functional>
#	include <boost/signal.hpp>

#	include "videomode.hpp"

class Event
{
	public:
		virtual ~Event() { }
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

class WindowEvent
	: public Event
{
} /* class WindowEvent */ ;

class MouseEvent
	: public Event
{
	public:
		typedef std::bitset<5> buttons_mask_type ;

		MouseEvent(Size const & position, buttons_mask_type const & buttons)
			: m_position(position)
			, m_buttons(buttons)
		{ }

		Size const & position() const
		{
			return m_position ;
		}

		buttons_mask_type const & buttons() const
		{
			return m_buttons ;
		}

	private:
		Size const m_position ;
		buttons_mask_type const m_buttons ;

} /* class MouseEvent */ ;

class MouseMotionEvent
	: public MouseEvent
{
	public:
		MouseMotionEvent(Size const & position, MouseEvent::buttons_mask_type const buttons)
			: MouseEvent(position, buttons)
		{ }

} /* class MouseMotionEvent */ ;

class MouseButtonEvent
	: public MouseEvent
{
	public:
		MouseButtonEvent(Size const & position, MouseEvent::buttons_mask_type const buttons, bool down)
			: MouseEvent(position, buttons)
			, m_down(down)
		{ }

		bool const down() const { return m_down ; }

	private:
		bool const m_down ;

} /* class MouseButtonEvent */ ;

class EventLoop
{
	public:
		typedef boost::signal<void (KeyEvent const &)> keyboard_event_type ;
		typedef boost::signal<void (MouseMotionEvent const &)> mouse_motion_event_type ;
		typedef boost::signal<void (MouseButtonEvent const &)> mouse_button_event_type ;

		void operator() () const ;

		void attach_event(keyboard_event_type::slot_function_type const & fn) ;
		void attach_event(mouse_motion_event_type::slot_function_type const & fn) ;
		void attach_event(mouse_button_event_type::slot_function_type const & fn) ;

	private:
		keyboard_event_type m_onkeypress ;

		mouse_button_event_type m_onmousebutton ;
		mouse_motion_event_type m_onmousemotion ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
