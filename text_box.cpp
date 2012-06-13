#include "text_box.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

TextBox::TextBox(Gui & gui, Widget * p_parent)
	: Box(gui, p_parent)
{
}

TextBox::~TextBox()
{
}

void TextBox::text(std::string const & new_text)
{
	m_text = new_text ;
}

std::string const & TextBox::text() const
{
	return m_text ;
}

void TextBox::draw()
{
	Box::draw() ;

	Surface & s = surface() ;
	s.write(text(), style()) ;
}

void TextBox::listen_events()
{
}


