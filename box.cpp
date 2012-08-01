#include "style.hpp"
#include "box.hpp"
#include "widget.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "tools.hpp"

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

	for(auto child: children())
	{
		auto * child_widget = dynamic_cast<Drawable *>(child.get()) ;

		child_widget->draw() ;
		surface().draw(child_widget->surface(), child_widget->style().position()) ;
	}
}
