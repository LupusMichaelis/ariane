
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
}

Screen::~Screen()
{
}

void Screen::init()
{
	auto p_s = gui().layout().screen() ;
	surface(std::move(p_s)) ;
}

