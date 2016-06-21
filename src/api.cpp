
#include "api.hpp"


Surface::~Surface()
{
}

TextSurface::~TextSurface()
{
}

EventTarget::~EventTarget()
{
}

#include "style.hpp"

Style Drawable::style()
{
	return const_cast<Style &>(const_cast<Drawable const &>(*this).style());
}

Drawable::~Drawable()
{
}
