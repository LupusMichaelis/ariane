#ifndef HPP_SCREEN_WIDGET_QUEST
#	define HPP_SCREEN_WIDGET_QUEST

#	include "widget.hpp"

class Screen
	: public Widget
{
	public:
		explicit Screen(Gui & gui) ;
		virtual ~Screen() ;

	protected:
		virtual
		void draw() ;

		virtual
		void listen_events() ;

} /* class Screen */ ;

#endif // HPP_SCREEN_WIDGET_QUEST
