#ifndef HPP_EVENT_SDL_QUEST
#	define HPP_EVENT_SDL_QUEST

#	include <functional>
#	include <boost/signals2.hpp>

#	include "videomode.hpp"
#	include "memory.hpp"

class Event
{
	public:
		virtual ~Event() { }
} /* class Event */ ;

class Key ;

class KeyBoard
{
	public:
		KeyBoard() ;
		KeyBoard(KeyBoard const &) = delete ;
		KeyBoard const & operator =(KeyBoard const &) = delete ;

		Key const & enter() const ;
		Key const & right_ctrl() const ;
		Key const & left_ctrl() const ;
		Key const & right_alt() const ;
		Key const & left_alt() const ;
		Key const & right_shift() const ;
		Key const & left_shift() const ;
		Key const & right_meta() const ;
		Key const & left_meta() const ;
		Key const & tab() const ;
		Key const & clear() const ;
		Key const & _return() const ;
		Key const & pause() const ;
		Key const & escape() const ;
		Key const & space() const ;
		Key const & exclaim() const ;
		Key const & quotedbl() const ;
		Key const & hash() const ;
		Key const & dollar() const ;
		Key const & ampersand() const ;
		Key const & quote() const ;
		Key const & leftparen() const ;
		Key const & rightparen() const ;
		Key const & asterisk() const ;
		Key const & plus() const ;
		Key const & comma() const ;
		Key const & minus() const ;
		Key const & period() const ;
		Key const & slash() const ;
		Key const & _0() const ;
		Key const & _1() const ;
		Key const & _2() const ;
		Key const & _3() const ;
		Key const & _4() const ;
		Key const & _5() const ;
		Key const & _6() const ;
		Key const & _7() const ;
		Key const & _8() const ;
		Key const & _9() const ;
		Key const & colon() const ;
		Key const & semicolon() const ;
		Key const & less() const ;
		Key const & equals() const ;
		Key const & greater() const ;
		Key const & question() const ;
		Key const & at() const ;
		Key const & leftbracket() const ;
		Key const & backslash() const ;
		Key const & rightbracket() const ;
		Key const & caret() const ;
		Key const & underscore() const ;
		Key const & backquote() const ;
		Key const & a() const ;
		Key const & b() const ;
		Key const & c() const ;
		Key const & d() const ;
		Key const & e() const ;
		Key const & f() const ;
		Key const & g() const ;
		Key const & h() const ;
		Key const & i() const ;
		Key const & j() const ;
		Key const & k() const ;
		Key const & l() const ;
		Key const & m() const ;
		Key const & n() const ;
		Key const & o() const ;
		Key const & p() const ;
		Key const & q() const ;
		Key const & r() const ;
		Key const & s() const ;
		Key const & t() const ;
		Key const & u() const ;
		Key const & v() const ;
		Key const & w() const ;
		Key const & x() const ;
		Key const & y() const ;
		Key const & z() const ;
		Key const & _delete() const ;
		Key const & kp0() const ;
		Key const & kp1() const ;
		Key const & kp2() const ;
		Key const & kp3() const ;
		Key const & kp4() const ;
		Key const & kp5() const ;
		Key const & kp6() const ;
		Key const & kp7() const ;
		Key const & kp8() const ;
		Key const & kp9() const ;
		Key const & kp_period() const ;
		Key const & kp_divide() const ;
		Key const & kp_multiply() const ;
		Key const & kp_minus() const ;
		Key const & kp_plus() const ;
		Key const & kp_enter() const ;
		Key const & kp_equals() const ;
		Key const & up() const ;
		Key const & down() const ;
		Key const & right() const ;
		Key const & left() const ;
		Key const & insert() const ;
		Key const & home() const ;
		Key const & end() const ;
		Key const & pageup() const ;
		Key const & pagedown() const ;
		Key const & f1() const ;
		Key const & f2() const ;
		Key const & f3() const ;
		Key const & f4() const ;
		Key const & f5() const ;
		Key const & f6() const ;
		Key const & f7() const ;
		Key const & f8() const ;
		Key const & f9() const ;
		Key const & f10() const ;
		Key const & f11() const ;
		Key const & f12() const ;
		Key const & f13() const ;
		Key const & f14() const ;
		Key const & f15() const ;
		Key const & numlock() const ;
		Key const & capslock() const ;
		Key const & scrollock() const ;
		Key const & rshift() const ;
		Key const & lshift() const ;
		Key const & rctrl() const ;
		Key const & lctrl() const ;
		Key const & ralt() const ;
		Key const & lalt() const ;
		Key const & rmeta() const ;
		Key const & lmeta() const ;
		Key const & lsuper() const ;
		Key const & rsuper() const ;
		Key const & mode() const ;
		Key const & help() const ;
		Key const & print() const ;
		Key const & sysreq() const ;
		Key const & _break() const ;
		Key const & menu() const ;
		Key const & power() const ;
		Key const & euro() const ;

	private:
		class Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class KeyBoard */ ;

class Key
{
	public:
		explicit
		Key(unsigned const new_value)
			: m_value(new_value) { }

	private:
		char m_value ;

	public:
		Key() = default ;
		Key(Key const &) = default ;
		Key & operator =(Key const &) = default ;

		unsigned const value() const { return m_value ; }

} /* class Key */ ;

inline bool const operator ==(Key const & lhs, Key const & rhs) { return lhs.value() == rhs.value() ; }

class KeyEvent
	: public Event
{
	public:
		explicit KeyEvent(Key const & key, bool press = false)
			: m_key(key)

			, m_press(press)

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

		bool const pressing() const		{ return m_press ; }
		bool const release() const		{ return !m_press ; }

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
		Key		m_key ;

		bool	m_press ;

		bool	m_left_alt ;
		bool	m_left_ctrl ;
		bool	m_left_meta ;
		bool	m_left_shift ;

		bool	m_right_alt ;
		bool	m_right_ctrl ;
		bool	m_right_meta ;
		bool	m_right_shift ;

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
		MouseButtonEvent(Size const & position, MouseEvent::buttons_mask_type const buttons, bool press)
			: MouseEvent(position, buttons)
			, m_press(press)
		{ }

		bool const pressing() const		{ return m_press ; }
		bool const release() const		{ return !m_press ; }

	private:
		bool const m_press ;

} /* class MouseButtonEvent */ ;

class TimeEvent
	: public Event
{
	public:

	private:
		int m_elapsed ;

} /* class TimeEvent */ ;

class EventLoop
{
	public:
		typedef boost::signals2::connection con_type ;
		typedef boost::signals2::signal<void (EventLoop &, KeyEvent const &)>			keyboard_event_type ;
		typedef boost::signals2::signal<void (EventLoop &, MouseMotionEvent const &)>	mouse_motion_event_type ;
		typedef boost::signals2::signal<void (EventLoop &, MouseButtonEvent const &)> 	mouse_button_event_type ;

		typedef boost::signals2::signal<void (EventLoop &)>								time_event_type ;
		static
		int const c_time_tick_event /*= 100 */ ; // in ms

		void init_heart(unsigned time) ;

	public:
		EventLoop() ;

		void operator() () ;

		con_type const attach_event(keyboard_event_type::slot_function_type const & fn) ;
		con_type const attach_event(mouse_motion_event_type::slot_function_type const & fn) ;
		con_type const attach_event(mouse_button_event_type::slot_function_type const & fn) ;

		con_type const attach_event(time_event_type::slot_function_type const & fn) ;

		void stop() ;

		~EventLoop() ;

	private:
		void heart_pace() ;

	private:
		class Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
