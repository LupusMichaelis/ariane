#ifndef HPP_BOX_TEXT_WIDGET_QUEST
#	define HPP_BOX_TEXT_WIDGET_QUEST

#	include "box.hpp"

class TextBox
	: public Box
{
	public:
		explicit TextBox(Gui & gui, Widget * p_parent) ;
		virtual ~TextBox() ;

		void text(std::string const & new_text) ;
		std::string const & text() const ;

	protected:
		virtual
		void draw() ;

		virtual
		void listen_events() ;

	private:
		std::string m_text ;

} /* class TextBox */ ;


#endif // HPP_BOX_TEXT_WIDGET_QUEST
