
#include "widget.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

// Widget ////////////////////////////////////////////////////////////////////////////////
Widget::Widget(Widget * p_parent)
	: mp_parent(p_parent)
	, m_cons()
{
}

Widget::~Widget()
{
	detach_events() ;
}

bool const Widget::has_parent() const
{
	return mp_parent ;
}

Widget & Widget::parent()
{
	if(!mp_parent)
		throw std::logic_error("Widget has no parent") ;

	return *mp_parent ;
}

void Widget::parent(Widget & new_parent)
{
	mp_parent = &new_parent ;
}

void Widget::detach_events()
{
	for(auto con: m_cons)
		con.disconnect() ;
}

void Widget::display()
{
	draw() ;
	listen_events() ;
}

// ComposedWidget //////////////////////////////////////////////////////////////////////// 
ComposedWidget::ComposedWidget(Gui & gui, Widget * p_parent)
	: Widget {p_parent}
	, m_gui(gui)
	, mp_surface(nullptr)
	, m_style(gui.style())
	, m_children()
{
}

Gui const & ComposedWidget::gui() const
{
	return m_gui ;
}

Gui & ComposedWidget::gui()
{
	return const_cast<Gui &>(const_cast<ComposedWidget const &>(*this).gui()) ;
}

Style const & ComposedWidget::style() const
{
	return m_style ;
}

void ComposedWidget::style(Style const & new_style)
{
	Style old_style {m_style} ;
	m_style = new_style ;

	//if(m_style != old_style)
		//damage() ;
}

Size const ComposedWidget::size() const
{
	return style().size() ;
}

Surface & ComposedWidget::surface()
{
	return const_cast<Surface &>(const_cast<ComposedWidget const &>(*this).surface()) ;
}

Surface const & ComposedWidget::surface() const
{
	if(!mp_surface)
		throw std::logic_error("WTF") ;

	return *mp_surface ;
}

void ComposedWidget::surface(std::unique_ptr<Surface> p_set_surface)
{
	mp_surface = std::move(p_set_surface) ;
}

bool const ComposedWidget::has_child() const
{
	return m_children.size() > 0 ;
}

ComposedWidget::list_type const & ComposedWidget::children() const
{
	return m_children ;
}

void ComposedWidget::adopt(Widget::list_type::value_type new_child)
{
	new_child->parent(*this) ;
	m_children.push_back(std::move(new_child)) ;
}

Widget::list_type::value_type ComposedWidget::abandon(list_type::value_type::element_type & abandoned_child)
{
	auto it = std::find_if(m_children.begin(), m_children.end()
			, [& abandoned_child] (list_type::value_type & p_w) -> bool
			{ return &abandoned_child == p_w.get() ; }) ;

	if(it == m_children.end())
		throw "Can't abandon non-child Widget" ;

	auto r = std::move(*it) ;
	m_children.erase(it) ;

	//r->parent(nullptr) ; //XXX

	return r ;
}

std::vector<Widget::list_type::value_type::pointer> 
ComposedWidget::children()
{
	std::vector<Widget::list_type::value_type::pointer> list {m_children.size()} ;
	std::transform(m_children.begin(), m_children.end(), list.begin()
			, [] (std::unique_ptr<Widget> & w) -> Widget * { return w.get() ; }) ;

	return list ;
}

//////////////////////////////
// class Image
//

Image::Image(Gui & gui, Widget * p_parent)
	 : ComposedWidget(gui, p_parent)
{
}

Image::~Image()
{
}

void Image::filename(std::string const & file_name)
{
	m_filename = file_name ;
}

std::string const & Image::filename()
{
	return m_filename ;
}

void Image::load()
{
	surface(gui().layout().surface(filename())) ;
	auto widget_style = style() ;
	widget_style.size(surface().videomode().size()) ;
	style(widget_style) ;
}

void Image::draw()
{
}

void Image::listen_events()
{
}

