#include "text_surface.hpp"
#include "font_manager.hpp"
#include "gui_layout.hpp"

#include <SDL/SDL_ttf.h>

////////////////////////////////////////////////////////////////////////////////
struct TextSurfaceSDL::Impl
{
	Impl(std::string const & set_text, Pen const & set_pen)
		: m_text(set_text)
		, m_pen(set_pen)
	{ }

	std::string				m_text;
	Pen						m_pen;

} /* struct TextSurfaceSDL::Impl */;

TextSurfaceSDL::TextSurfaceSDL(GuiLayout & set_gui_layout
		, VideoMode set_videomode
		, std::string const & set_text
		, Pen const & set_pen)
	: SurfaceSDL {set_gui_layout, set_videomode}
	, mp_impl { std::make_unique<Impl>(set_text, set_pen) }
{
	// XXX call virtual functions /!\ //
	init();
}

TextSurfaceSDL::~TextSurfaceSDL()
{
}

void TextSurfaceSDL::init()
{
	auto p_font = gui_layout().fonts().get(pen().font().name());
	TTF_Font * p_handle = std::static_pointer_cast<FontSDL>(p_font)->get_raw(pen().size());
	if(!p_handle)
		throw TTF_GetError();

	SDL_Surface * p_text = TTF_RenderText_Solid(p_handle, text().c_str()
			, {pen().color().red(), pen().color().green(), pen().color().blue(), 0, });
	if(!p_text)
		throw TTF_GetError();

	set_raw(p_text);
}

std::string const & TextSurfaceSDL::text() const
{
	return mp_impl->m_text;
}

Pen const & TextSurfaceSDL::pen() const
{
	return mp_impl->m_pen;
}
