#ifndef HPP_EVENT_SDL_QUEST
#	define HPP_EVENT_SDL_QUEST

#	include <functional>
#	include <boost/signal.hpp>

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

class EventLoop
{
	public:
		typedef boost::signal<void (KeyEvent const &)> keyboard_event_type ;

		void operator() () const ;

		void attach_event(char const * event, keyboard_event_type::slot_function_type const & fn) ;

	private:
		keyboard_event_type m_onkeyup ;
		keyboard_event_type m_onkeydown ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
