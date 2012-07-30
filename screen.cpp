
#include "screen.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

Screen::Screen(Gui & gui)
	: ComposedWidget {gui}
{
}

Screen::~Screen()
{
}

void Screen::draw()
{
	surface(gui().layout().screen()) ;

	Surface & s = surface() ;
	s.fill(style().color()) ;

	for(auto &p_child: children())
	{
		p_child->display() ;
		s.draw(p_child->surface(), p_child->style().position()) ;
	}

	s.update() ;
}

void Screen::listen_events()
{
}

