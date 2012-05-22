#include "style.hpp"
#include "screen.hpp"

void Style::create(std::shared_ptr<Style> & p_style, Screen & parent_screen)
{
	std::shared_ptr<Style> p_new_style(new Style(parent_screen)) ;
	p_new_style->color(RGBColor(0, 0, 0)) ;

	p_style = p_new_style ;
}

Style::Style(Screen & parent_screen)
	: m_screen(parent_screen)
	, m_color(0, 0, 0)
	, m_size()
{
}

void Style::color(RGBColor const & color)
{
	m_color = color ;
}

RGBColor const & Style::color() const
{
	return m_color ;
}

void Style::font(std::string const & new_font)
{
	m_font = new_font ;
}

std::string const & Style::font() const
{
	return m_font ;
}

void Style::size(unsigned const & new_size)
{
	m_size = new_size ;
}

unsigned const Style::size() const
{
	return m_size ;
}

