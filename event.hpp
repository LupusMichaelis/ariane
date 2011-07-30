#ifndef HPP_EVENT_SDL_QUEST
#	define HPP_EVENT_SDL_QUEST

#	include <functional>
#	include <boost/signal.hpp>

class Event
{
	public:
} /* class Event */ ;

class EventLoop
{
	public:
		typedef boost::signal<void ()> event_type ;

		void operator() () const ;

		void attach_event(char const * event, event_type::slot_function_type const & fn) ;

	private:
		boost::signal<void ()> m_onkeyup ;
		boost::signal<void ()> m_onkeydown ;

} /* class EventLoop */ ;

#endif // HPP_EVENT_SDL_QUEST
