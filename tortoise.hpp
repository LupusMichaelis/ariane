#ifndef HPP_TORTOISE_QUEST
#	define HPP_TORTOISE_QUEST

#include "quest_interface.hpp"
#include "box.hpp"

class Canvas;
class TortoiseModel;

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

	protected:
		Canvas & tortoise_view() ;
		TortoiseModel & tortoise_model() ;
		TortoiseModel const & tortoise_model() const ;

	private:
		Box::SharedPtr				mp_tortoise ;

} /* class TortoiseInterface */ ;

class TortoiseModel
{
	public:
		TortoiseModel() ;
		~TortoiseModel() ;

		Size const & position() const ;
		
	private:
		struct Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class TortoiseModel */ ;

#endif // HPP_TORTOISE_QUEST

