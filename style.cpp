#include "style.hpp"

Style::Style(Surface & parent_screen)
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

