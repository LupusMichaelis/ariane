
#include "style.hpp"
#include "screen.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

Screen::SharedPtr Screen::make(Gui & gui)
{
	return Screen::SharedPtr(new Screen(gui)) ;
}

Screen::Screen(Gui & gui)
	: Box {gui}
{
	Style s = style() ;
	s.size(gui.layout().videomode().size()) ;
	assert(s.size().width() != 0) ;
	style(s) ;
}

Screen::~Screen()
{
}

void Screen::init()
{
	auto p_s = gui().layout().screen() ;
	Style s = style() ;
	s.size(p_s->videomode().size()) ;
	assert(s.size().width() != 0) ;
	style(s) ;
	surface(std::move(p_s)) ;
}

