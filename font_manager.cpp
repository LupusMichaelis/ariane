
#include "font_manager.hpp"

#include <map>
#include <SDL/SDL_ttf.h>

struct FontManager::Impl
{
	Impl(GuiLayout & gui_layout, std::list<boost::filesystem::path> paths)
		: m_gui_layout(gui_layout), m_paths(paths), m_fonts()
	{ }

	GuiLayout &								m_gui_layout ;
	std::list<boost::filesystem::path>		m_paths ;
	std::map<std::string, Font::SharedPtr>	m_fonts ;

} /* struct FontManager::Impl */ ;

FontManager::FontManager(GuiLayout & gui_layout, std::list<boost::filesystem::path> const & font_paths)
	: mp_impl { std::make_unique<Impl>(gui_layout, font_paths) }
{
	TTF_Init() ;
}

FontManager::~FontManager()
{
}


Font::SharedPtr FontManager::get(std::string const & name)
{
	Font::SharedPtr p_font ;

	auto it_font_handle = mp_impl->m_fonts.find(name) ;

	if(mp_impl->m_fonts.end() != it_font_handle)
		p_font = it_font_handle->second ;
	else
	{
		p_font = make(name) ;
		mp_impl->m_fonts[name] = p_font ;
	}

	return p_font ;
}

#include <boost/format.hpp>
#include "gui.hpp"

Font::SharedPtr FontManager::make(std::string const & name)
{
	boost::filesystem::path filepath
		{(boost::format("/usr/share/fonts/truetype/msttcorefonts/%s.ttf") % name).str()} ;

	return FontSDL::make_from_file(const_cast<GuiLayout&>(gui_layout()), name, filepath) ;
}

GuiLayout & FontManager::gui_layout()
{
	return mp_impl->m_gui_layout ;
}
