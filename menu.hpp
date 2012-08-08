#ifndef HPP_MENU_QUEST
#	define HPP_MENU_QUEST

#	include "interface.hpp"
#	include "text_box.hpp"

class MenuInterface
	: public QuestInterface
{
	public:
		explicit MenuInterface(QuestEngine & engine)
			: QuestInterface {engine}
			, m_current(0)
			, m_widgets(3)
		{
		}

		void entry_next() ;
		void entry_previous() ;
		void select() ;

	private:
		virtual
		void move(EventLoop &, KeyEvent const & ke) ;
		virtual
		void move(EventLoop &, MouseEvent const & ) { } ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & ) { } ;

		virtual
		void display() ;

		unsigned					m_current ;
		std::vector<TextBox::SharedPtr>
									m_widgets ;

} /* class MenuInterface */ ;


#endif // HPP_MENU_QUEST


