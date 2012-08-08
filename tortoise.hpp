#ifndef HPP_TORTOISE_QUEST
#	define HPP_TORTOISE_QUEST

#include "interface.hpp"
#include "box.hpp"

class TortoiseInterface
	: public QuestInterface
{
	public:
		explicit TortoiseInterface(QuestEngine & engine)
			: QuestInterface {engine}
		{ }

		virtual ~TortoiseInterface() { }

	private:
		void move_left() ;
		void move_right() ;
		void move_up() ;
		void move_down() ;

		virtual
		void move(EventLoop &, KeyEvent const & ke) ;
		virtual
		void move(EventLoop &, MouseEvent const & me) ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & me) ;

		virtual
		void display() ;

	private:
		Box::SharedPtr				mp_turtle ;

} /* class TortoiseInterface */ ;

#endif // HPP_TORTOISE_QUEST

