#include "style.hpp"

Font::Font()
	: m_name()
{
}

Font::Font(std::string const & font_name)
	: m_name(font_name)
{
}

std::string const & Font::name() const
{
	return m_name ;
}

void Font::name(std::string const & new_name)
{
	m_name = new_name ;
}

////////////////////////////////////////////////////////////////////////
Pen::Pen()
	: m_font()
	, m_color(0, 0, 0)
	, m_size(10u)
{
}

Pen::Pen(Font const & set_font, RGBColor const & set_color, unsigned const set_size)
	: m_font(set_font)
	, m_color(set_color)
	, m_size(set_size)
{
}

void Pen::color(RGBColor const & color)
{
	m_color = color ;
}

RGBColor const & Pen::color() const
{
	return m_color ;
}

void Pen::font(Font const & new_font)
{
	m_font = new_font ;
}

Font const & Pen::font() const
{
	return m_font ;
}

void Pen::size(unsigned new_size)
{
	m_size = new_size ;
}

unsigned const Pen::size() const
{
	return m_size ;
}

////////////////////////////////////////////////////////////////////////
Style::Style()
	: m_pen(Font {}, create_color(0xffffff), 10u)
	, m_color(0, 0, 0)
	, m_padding()
	, m_position()
	, m_size()
{
}

void Style::position(Size const & new_position)
{
	m_position = new_position ;
}

Size const & Style::position() const
{
	return m_position ;
}

void Style::padding(Size const & new_padding)
{
	m_padding = new_padding ;
}

Size const & Style::padding() const
{
	return m_padding ;
}

void Style::color(RGBColor const & color)
{
	m_color = color ;
}

RGBColor const & Style::color() const
{
	return m_color ;
}

void Style::pen(Pen const & new_pen)
{
	m_pen = new_pen ;
}

Pen const & Style::pen() const
{
	return m_pen ;
}

void Style::size(Size const & new_size)
{
	m_size = new_size ;
}

Size const & Style::size() const
{
	return m_size ;
}

////////////////////////////////////////////////////////////////////////
bool const operator ==(Style const & lhs, Style const & rhs)
{
	return lhs.color() == rhs.color()
		&& lhs.pen() == rhs.pen()
		&& lhs.position() == rhs.position()
		&& lhs.size() == rhs.size() ;
}

bool const operator !=(Style const & lhs, Style const & rhs)
{
	return ! (lhs == rhs) ;
}

bool const operator ==(Pen const & lhs, Pen const & rhs)
{
	return lhs.color() == rhs.color()
		&& lhs.font() == rhs.font()
		&& lhs.size() == rhs.size() ;
}

bool const operator !=(Pen const & lhs, Pen const & rhs)
{
	return ! (lhs == rhs) ;
}

bool const operator ==(Font const & lhs, Font const & rhs)
{
	return lhs.name() == rhs.name() ;
}

bool const operator !=(Font const & lhs, Font const & rhs)
{
	return ! (lhs == rhs) ;
}
