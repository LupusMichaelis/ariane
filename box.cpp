#include "box.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

Box::Box(Gui & gui, Widget * p_parent)
	: Widget(gui, p_parent)
{
}

Box::~Box()
{
}

void Box::draw()
{
	surface(gui().layout().surface(style().size())) ;
}

void Box::listen_events()
{
}

