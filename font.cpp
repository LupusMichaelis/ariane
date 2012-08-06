
#include "font.hpp"
#include "gui_layout.hpp"

#include <SDL/SDL_ttf.h>
#include <map>
#include <string>
#include <boost/format.hpp>

////////////////////////////////////////////////////////////////////////////////
struct FontSDL::Impl
{
	Impl(GuiLayout & gui_layout)
		: m_gui_layout(gui_layout)
		, mp_handle(nullptr)
	{ }

	GuiLayout & m_gui_layout ;
	TTF_Font * mp_handle ;

} /* struct Font::Impl */ ;

FontSDL::FontSDL(GuiLayout & gui_layout, std::string const & name, unsigned const size)
	: Font {name, size}
	, mp_impl { std::make_unique<Impl>(gui_layout) }
{
}

FontSDL::~FontSDL()
{
	TTF_CloseFont(mp_impl->mp_handle) ;
}

FontSDL::SharedPtr FontSDL::make_from_file(GuiLayout & gui_layout, std::string const & name, unsigned const size
		, boost::filesystem::path const & filepath)
{
	if(!boost::filesystem::exists(filepath) or !boost::filesystem::is_regular(filepath))
	{
		auto msg = (boost::format("'%s' is not a font file") % filepath.filename()).str() ;
		throw std::logic_error(msg);
	}

	TTF_Font * font_handle = NULL ;
	font_handle = TTF_OpenFont(filepath.c_str(), size) ;
	if(!font_handle)
		throw SDL_GetError() ;

	std::shared_ptr<FontSDL> p_font = std::make_shared<FontSDL>(gui_layout, name, size) ;
	p_font->mp_impl->mp_handle = font_handle ;

	return p_font ;
}

TTF_Font * FontSDL::get_raw() const
{
	return mp_impl->mp_handle ;
}

