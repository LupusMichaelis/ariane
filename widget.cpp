
#include "widget.hpp"
#include "gui.hpp"
#include "surface.hpp"

Widget::Widget(Gui & gui, Widget * p_parent)
	: m_gui(gui)
	, mp_surface(nullptr)
	, m_style(gui.style())

	, m_cons()

	, mp_parent(p_parent)
	, m_children()
{
}

Widget::~Widget()
{
	detach_events() ;
}

Gui const & Widget::gui() const
{
	return m_gui ;
}

Gui & Widget::gui()
{
	return const_cast<Gui &>(const_cast<Widget const &>(*this).gui()) ;
}

Style const & Widget::style() const
{
	return m_style ;
}

void Widget::style(Style const & new_style)
{
	Style old_style {m_style} ;
	m_style = new_style ;

	//if(m_style != old_style)
		//damage() ;
}

Size const Widget::size() const
{
	return m_size ;
}

void Widget::adopt(std::unique_ptr<Widget> p_adopted)
{
	//p_adopted->parent(this) ;
	m_children.push_back(std::move(p_adopted)) ;
}

Surface & Widget::surface()
{
	return const_cast<Surface &>(const_cast<Widget const &>(*this).surface()) ;
}

Surface const & Widget::surface() const
{
	return *mp_surface ;
}

void Widget::surface(std::unique_ptr<Surface> set_surface)
{
	mp_surface = std::move(set_surface) ;
}

void Widget::display()
{
	draw() ;
	listen_events() ;
}

void Widget::detach_events()
{
	for(auto con: m_cons)
		con.disconnect() ;
}

std::vector<std::unique_ptr<Widget>> & Widget::children()
{
	return m_children ;
}

