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

class Key ;

class KeyBoard
{
		class KBImpl ;
		std::auto_ptr<KBImpl> mp_impl ;

	public:
		KeyBoard() ;
		KeyBoard(KeyBoard const &) = delete ;
		KeyBoard const & operator =(KeyBoard const &) = delete ;

		Key const & up() ;
		Key const & down() ;
		Key const & right() ;
		Key const & left() ;

		Key const & enter() ;
		Key const & right_ctrl() ;
		Key const & left_ctrl() ;
		Key const & right_alt() ;
		Key const & left_alt() ;
		Key const & right_shift() ;
		Key const & left_shift() ;

		Key const & q() ;

} /* class KeyBoard */ ;

class Key
{
	friend class KeyBoard ;
	private:
		explicit
		Key(unsigned const new_value)
			: m_value(new_value) { }

		char m_value ;

	public:
		Key() = default ;
		Key(Key const &) = default ;
		Key & operator =(Key const &) = default ;

		unsigned const value() const { return m_value ; }

} /* class Key */ ;

class KeyEvent
	: public Event
{
	public:
		explicit KeyEvent(Key const & key)
			: m_key(key)

			, m_left_alt(false)
			, m_left_ctrl(false)
			, m_left_meta(false)
			, m_left_shift(false)

			, m_right_alt(false)
			, m_right_ctrl(false)
			, m_right_meta(false)
			, m_right_shift(false)
		{ }

		Key const & key() const			{ return m_key ; }

		void key(Key const & key)		{ m_key = key ; }

		bool const left_alt() const		{ return m_left_alt ; }
		bool const left_ctrl() const	{ return m_left_ctrl ; }
		bool const left_meta() const	{ return m_left_meta ; }
		bool const left_shift() const	{ return m_left_shift ; }
		bool const right_alt() const	{ return m_left_alt ; }
		bool const right_ctrl() const	{ return m_left_ctrl ; }
		bool const right_meta() const	{ return m_left_meta ; }
		bool const right_shift() const	{ return m_left_shift ; }

		void left_alt(bool const new_left_alt)			{ m_left_alt = new_left_alt ; }
		void left_ctrl(bool const new_left_ctrl)		{ m_left_ctrl = new_left_ctrl ; }
		void left_meta(bool const new_left_meta)		{ m_left_meta = new_left_meta ; }
		void left_shift(bool const new_left_shift)		{ m_left_shift = new_left_shift ; }
		void right_alt(bool const new_right_alt)		{ m_right_alt = new_right_alt ; }
		void right_ctrl(bool const new_right_ctrl)		{ m_right_ctrl = new_right_ctrl ; }
		void right_meta(bool const new_right_meta)		{ m_right_meta = new_right_meta ; }
		void right_shift(bool const new_right_shift)	{ m_right_shift = new_right_shift ; }

	private:
		Key			m_key ;

		unsigned	m_left_alt ;
		unsigned	m_left_ctrl ;
		unsigned	m_left_meta ;
		unsigned	m_left_shift ;

		unsigned	m_right_alt ;
		unsigned	m_right_ctrl ;
		unsigned	m_right_meta ;
		unsigned	m_right_shift ;

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
		typedef boost::signal<void (EventLoop &, KeyEvent const &)> keyboard_event_type ;
		typedef boost::signal<void (EventLoop &, MouseMotionEvent const &)> mouse_motion_event_type ;
		typedef boost::signal<void (EventLoop &, MouseButtonEvent const &)> mouse_button_event_type ;

		void operator() () ;

		void attach_event(keyboard_event_type::slot_function_type const & fn) ;
		void attach_event(mouse_motion_event_type::slot_function_type const & fn) ;
		void attach_event(mouse_button_event_type::slot_function_type const & fn) ;

		void stop() ;

	private:
		bool				m_running ;
		keyboard_event_type m_onkeypress ;

		mouse_button_event_type m_onmousebutton ;
		mouse_motion_event_type m_onmousemotion ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
