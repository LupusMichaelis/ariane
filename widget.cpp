
#include "widget.hpp"
#include "style.hpp"
#include "memory.hpp"
#include "gui.hpp"

struct Widget::Impl
{
	Impl(Gui & gui)
		: m_gui(gui)
		, m_style(gui.style())
		, mp_surface()
	{ }

	Gui &						m_gui ;
	Style						m_style ;
	std::unique_ptr<Surface>	mp_surface ;

} /* struct Widget::Impl */ ;

Widget::Widget(Gui & gui)
	: EventTarget()
	, Drawable()
	, mp_impl(std::make_unique<Impl>(gui))
{
}

Gui const & Widget::gui()
{
	return mp_impl->m_gui ;
}

void Widget::style(Style const & new_style)
{
	mp_impl->m_style = new_style ;
}

Style const & Widget::style() const
{
	return mp_impl->m_style ;
}

Style Widget::style()
{
	return const_cast<Style &>(const_cast<Widget const &>(*this).style()) ;
}

Widget::~Widget()
{
}

Surface /* const */ & Widget::surface()
{
	if(!mp_impl->mp_surface)
		init() ;

	return *mp_impl->mp_surface ;
}

void Widget::surface(std::unique_ptr<Surface> p_surface)
{
	std::swap(p_surface, mp_impl->mp_surface) ;
}

