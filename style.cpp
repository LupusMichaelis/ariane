#include "style.hpp"
#include "tools.hpp"

struct Font::Impl
{
	std::string		m_name ;

	Impl(std::string const & name)
		: m_name(name) { }

} /* struct Font::Impl */ ;

Font::Font(std::string const & name)
	: mp_impl { std::make_unique<Impl>(name) }
{ }

Font::Font(Font const & copied)
	: Font {copied.name()}
{ }

Font & Font::operator =(Font const & copied)
{
	Font copy {copied} ;
	std::swap(mp_impl, copy.mp_impl) ;
	return *this ;
}

Font::~Font()
{
}

std::string const & Font::name() const
{
	return mp_impl->m_name ;
}

bool const operator ==(Font const & lhs, Font const & rhs)
{
	return lhs.name() == rhs.name() ;
}

bool const operator !=(Font const & lhs, Font const & rhs)
{
	return ! (lhs == rhs) ;
}

////////////////////////////////////////////////////////////////////////
struct Pen::Impl
{
	Font		m_font ;
	RGBColor	m_color ;
	unsigned	m_size ;

	Impl(Font const & set_font, RGBColor const & set_color, unsigned const set_size)
		: m_font(set_font)
		, m_color(set_color)
		, m_size(set_size)
	{
	}
} /* struct Pen::Impl */ ;

Pen::Pen(Pen const & copied)
	: Pen {copied.mp_impl->m_font, copied.mp_impl->m_color, copied.mp_impl->m_size}
{
}

Pen::~Pen()
{
}

Pen & Pen::operator =(Pen const & copied)
{
	Pen copy {copied} ;
	std::swap(mp_impl, copy.mp_impl) ;
	return *this ;
}

Pen::Pen(Font const & set_font, RGBColor const & set_color, unsigned const set_size)
	: mp_impl {std::make_unique<Impl>(set_font, set_color, set_size) }
{
}

void Pen::color(RGBColor const & color)
{
	mp_impl->m_color = color ;
}

RGBColor const & Pen::color() const
{
	return mp_impl->m_color ;
}

void Pen::font(Font const & new_font)
{
	mp_impl->m_font = new_font ;
}

Font const & Pen::font() const
{
	return mp_impl->m_font ;
}

void Pen::size(unsigned new_size)
{
	mp_impl->m_size = new_size ;
}

unsigned const Pen::size() const
{
	return mp_impl->m_size ;
}

////////////////////////////////////////////////////////////////////////
struct Border::Impl
{
	Impl(RGBColor const & set_color, unsigned const set_size)
		: m_color(set_color), m_size(set_size) { }

	RGBColor m_color ;
	unsigned m_size ;
} /* struct Border::Impl */ ;

Border::Border(RGBColor const & set_color, unsigned const set_size)
	: mp_impl(std::make_unique<Impl>(set_color, set_size))
{
}

Border::Border(Border const & copied)
	: Border {copied.color(), copied.size()}
{
}

Border & Border::operator =(Border const & copied)
{
	Border copy { copied } ;
	std::swap(this->mp_impl, copy.mp_impl) ;
	return *this ;
}

Border::~Border()
{
}

void Border::color(RGBColor const & new_color)
{
	mp_impl->m_color = new_color ;
}

RGBColor const & Border::color() const
{
	return mp_impl->m_color ;
}

void Border::size(unsigned const new_size)
{
	mp_impl->m_size = new_size ;
}

unsigned const Border::size() const
{
	return mp_impl->m_size ;
}


////////////////////////////////////////////////////////////////////////
struct Style::Impl
{
	Impl(Pen const & set_pen
		, RGBColor const & set_color
		, Size const & set_position
		, Size const & set_padding
		, Size const & set_size
		, Border const & set_border
		)
		: m_pen(set_pen)
		, m_color(set_color)
		, m_padding(set_padding)
		, m_position(set_position)
		, m_size(set_size)
		, m_border(set_border)
	{}

	Pen			m_pen ;
	RGBColor	m_color ;
	Size		m_padding ;
	Size		m_position ;
	Size		m_size ;
	Border		m_border ;

} /* struct Style::Impl */ ;

Style::Style(Pen const & set_pen
		, RGBColor const & set_color
		, Size const & set_position
		, Size const & set_padding
		, Size const & set_size
		, Border const & set_border
		)
	: mp_impl {std::make_unique<Impl>(set_pen, set_color, set_position, set_padding, set_size, set_border) }
{
}

Style::~Style()
{
}

Style::Style(Style const & copied)
	: Style {copied.pen(), copied.color(), copied.position(), copied.padding(), copied.size(), copied.border() }
{
}

Style & Style::operator =(Style const & copied)
{
	Style copy {copied} ;
	std::swap(copy.mp_impl, this->mp_impl) ;
	return *this ;
}

void Style::position(Size const & new_position)
{
	mp_impl->m_position = new_position ;
}

Size const & Style::position() const
{
	return mp_impl->m_position ;
}

void Style::padding(Size const & new_padding)
{
	mp_impl->m_padding = new_padding ;
}

Size const & Style::padding() const
{
	return mp_impl->m_padding ;
}

void Style::color(RGBColor const & color)
{
	mp_impl->m_color = color ;
}

RGBColor const & Style::color() const
{
	return mp_impl->m_color ;
}

void Style::pen(Pen const & new_pen)
{
	mp_impl->m_pen = new_pen ;
}

Pen const & Style::pen() const
{
	return mp_impl->m_pen ;
}

void Style::size(Size const & new_size)
{
	mp_impl->m_size = new_size ;
}

Size const & Style::size() const
{
	return mp_impl->m_size ;
}

void Style::border(Border const & new_border)
{
	mp_impl->m_border = new_border ;
}

Border const & Style::border() const
{
	return mp_impl->m_border ;
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

