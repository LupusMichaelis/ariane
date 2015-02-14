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
	return Box::SharedPtr(new Box(gui));
}

Box::~Box()
{
}

void Box::init()
{
	auto p_s = gui().layout().surface(style().size());
	surface(std::move(p_s));
}

void Box::draw()
{
	surface().fill(style().color());
	surface().border(style().border());

	for(auto child: children())
	{
		auto * child_widget = dynamic_cast<Drawable *>(child.get());

		child_widget->draw();
		Size draw_at { style().padding() + std::max(child_widget->style().position(), style().border().size() * Size {1, 1} )};
		surface().draw(child_widget->surface(), draw_at);

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

#include "visitor.hpp"

Size const absolute_position(Box const & w)
{
	class impl
		: public Box::Visitor
	{
			Size m_computed;

		public:
			impl(Box const & w)
				: Box::Visitor(w)
			{ }

			using Box::Visitor::operator();

			virtual
			void operator() (Box const & w)
			{
				m_computed += w.style().position() + w.style().padding();
				if(has_parent(w))
					m_computed += absolute_position(static_cast<Box&>(*w.parent()));
			}

			operator Size() { (*this)(); return m_computed ; }

	} /* class impl */;

	return impl(w);
}

