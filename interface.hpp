#ifndef HPP_INTERFACE_QUEST
#	define HPP_INTERFACE_QUEST

#	include "gui.hpp"
#	include "event.hpp"

#	include <vector>

class Engine ;

class Interface
{
	public:
		explicit Interface(Engine & engine) ;

		Engine & engine() ;
		Engine const & engine() const ;
		Style title_style() const ;

		virtual
		void display() = 0 ;

		virtual
		~Interface() ;

		virtual
		void move(EventLoop &, KeyEvent const & ke) = 0 ;
		virtual
		void move(EventLoop &, MouseEvent const & me) = 0 ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & me) = 0 ;

	protected:
		std::vector<EventLoop::con_type> & cons() ;

	private:
		void listen_events() ;
		void unlisten_events() ;

	private:
		class Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class Interface */ ;

#endif // HPP_INTERFACE_QUEST
