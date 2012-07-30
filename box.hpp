#ifndef HPP_BOX_WIDGET_QUEST
#	define HPP_BOX_WIDGET_QUEST

#	include "widget.hpp"

class Box
	: public ComposedWidget
{
	public:
		explicit Box(Gui & gui, Widget * p_parent) ;
		virtual ~Box() ;

	protected:
		virtual
		void draw() ;

		virtual
		void listen_events() ;

} /* class Box */ ;

#endif // HPP_BOX_WIDGET_QUEST
