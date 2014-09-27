#include "text_box.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"
#include "style.hpp"

TextBox::TextBox(Gui & gui)
	: Box(gui)
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

TextBox::SharedPtr TextBox::make(Gui & gui)
{
	return TextBox::SharedPtr(new TextBox(gui)) ;
}
