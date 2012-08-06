#include "text_surface.hpp"
#include "font_manager.hpp"
#include "gui_layout.hpp"

#include <SDL/SDL_ttf.h>

////////////////////////////////////////////////////////////////////////////////
struct TextSurfaceSDL::Impl
{
	Impl(std::string const & set_text, FontSDL::SharedPtr set_p_font, RGBColor const & set_color)
		: m_text(set_text)
		, mp_font(set_p_font)
		, m_color(set_color)
	{ }

	std::string				m_text ;
	FontSDL::SharedPtr		mp_font ;
	RGBColor				m_color ;

} /* struct TextSurfaceSDL::Impl */ ;

TextSurfaceSDL::TextSurfaceSDL(GuiLayout & set_gui_layout
		, VideoMode set_videomode
		, std::string const & set_text
		, FontSDL::SharedPtr set_p_font
		, RGBColor const & set_color)
	: SurfaceSDL {set_gui_layout, set_videomode}
	, mp_impl { std::make_unique<Impl>(set_text, set_p_font, set_color) }
{
	init() ;
}

TextSurfaceSDL::~TextSurfaceSDL()
{
}

void TextSurfaceSDL::init()
{
	auto p_font = gui_layout().fonts().get(font().name(), font().size()) ;
	TTF_Font * p_handle = std::static_pointer_cast<FontSDL>(p_font)->get_raw() ;

	SDL_Surface * p_text = TTF_RenderText_Solid(p_handle, text().c_str()
			, {color().red(), color().green(), color().blue(), 0, }) ;
	if(!p_text)
		throw TTF_GetError() ;

	set_raw(p_text) ;
}

std::string const & TextSurfaceSDL::text() const
{
	return mp_impl->m_text ;
}

Font const & TextSurfaceSDL::font() const
{
	return *mp_impl->mp_font ;
}

RGBColor const & TextSurfaceSDL::color() const
{
	return mp_impl->m_color ;
}

