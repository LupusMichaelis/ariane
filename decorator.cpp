
#include "decorator.hpp"

Decorator::Decorator(std::unique_ptr<Widget> p_set_decorated)
	: Widget()
	, mp_decorated(std::move(p_set_decorated))
{
	assert(mp_decorated) ;
}

Widget const & Decorator::decorated() const
{
	return *mp_decorated ;
}

Widget & Decorator::decorated()
{
	return const_cast<Widget &>(const_cast<Decorator const &>(*this).decorated()) ;
}

/*
void Decorator::decorated(std::unique_ptr<Widget> p_set_decorated)
{
	mp_decorated = std::move(p_set_decorated) ;
	//p_set_decorated->parent(nullptr) ; //XXX
	decorated().parent(*this) ;
}
*/

void Decorator::adopt(list_type::value_type new_child)
{
	decorated().adopt(std::move(new_child)) ;
}

Widget::list_type::value_type Decorator::abandon(list_type::value_type::element_type & abandoned_child)
{
	return decorated().abandon(abandoned_child) ;
}

bool const Decorator::has_child() const
{
	return decorated().has_child() ;
}

Widget::list_type const & Decorator::children() const
{
	return decorated().children() ;
}

std::vector<Widget::list_type::value_type::pointer> Decorator::children()
{
	return decorated().children() ;
}

Surface const & Decorator::surface() const
{
	return decorated().surface() ;
}

Surface & Decorator::surface()
{
	return decorated().surface() ;
}

Gui const & Decorator::gui() const
{
	return decorated().gui() ;
}

Gui & Decorator::gui()
{
	return decorated().gui() ;
}

Style const & Decorator::style() const
{
	return decorated().style() ;
}

void Decorator::style(Style const & set_style)
{
	return decorated().style(set_style) ;
}

Size const Decorator::size() const
{
	return decorated().size() ;
}

#include "surface.hpp"

void Decorator::surface(std::unique_ptr<Surface> p_set_surface)
{
	return decorated().surface(std::move(p_set_surface)) ;
}

