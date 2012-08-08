#include "style.hpp"
#include "box.hpp"
#include "widget.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "memory.hpp"

#ifndef NDEBUG
#	include <iostream>
#	include <boost/format.hpp>
#endif // NDEBUG

Box::Box(Gui & gui)
	: Widget { gui }
{
}

Box::SharedPtr Box::make(Gui & gui)
{
	return Box::SharedPtr(new Box(gui)) ;
}

Box::~Box()
{
}

void Box::init()
{
	auto p_s = gui().layout().surface(style().size()) ;
	surface(std::move(p_s)) ;
}

void Box::draw()
{
	surface().fill(style().color()) ;
	surface().border(style().border()) ;

	for(auto child: children())
	{
		auto * child_widget = dynamic_cast<Drawable *>(child.get()) ;

		child_widget->draw() ;
		Size draw_at { style().padding() + std::max(child_widget->style().position(), style().border().size() * Size {1, 1} )} ;
		surface().draw(child_widget->surface(), draw_at) ;

#ifndef NDEBUG
		std::cout << boost::format("'%s' was drawn on '%s' at {w:%d,h:%d}\n")
			% typeid(*child_widget).name()
			% typeid(*this).name()
			% draw_at.width()
			% draw_at.height()
			;
#endif // NDEBUG

	}
}
