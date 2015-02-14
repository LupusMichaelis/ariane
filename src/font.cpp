
#include "font.hpp"
#include "gui_layout.hpp"

#include <SDL/SDL_ttf.h>
#include <map>
#include <string>
#include <boost/format.hpp>

#include <SDL/SDL.h>


void font_release (SDL_RWops * s) { SDL_RWclose(s); }

////////////////////////////////////////////////////////////////////////////////
struct FontSDL::Impl
{
	Impl(GuiLayout & gui_layout, SDL_RWops * p_stream)
		: m_gui_layout(gui_layout)
		, mp_stream(p_stream, &font_release)
		, m_handles()
	{ }

	GuiLayout & m_gui_layout;

	std::unique_ptr<SDL_RWops, void (*) (SDL_RWops *)>
		mp_stream;
	std::map<unsigned, std::shared_ptr<TTF_Font>>
		m_handles;

} /* struct Font::Impl */;

FontSDL::FontSDL(GuiLayout & gui_layout, std::string const & name, SDL_RWops * p_stream)
	: Font {name}
	, mp_impl { std::make_unique<Impl>(gui_layout, p_stream) }
{
}

FontSDL::~FontSDL()
{
}

FontSDL::SharedPtr FontSDL::make_from_file(GuiLayout & gui_layout, std::string const & name, boost::filesystem::path const & filepath)
{
	if(!boost::filesystem::exists(filepath) or !boost::filesystem::is_regular(filepath))
	{
		auto msg = (boost::format("'%s' is not a font file") % filepath.filename()).str();
		throw std::logic_error(msg);
	}

	auto p_stream = SDL_RWFromFile(filepath.c_str(), "rb");
	if(!p_stream)
		throw SDL_GetError();

	std::shared_ptr<FontSDL> p_font = std::make_shared<FontSDL>(gui_layout, name, p_stream);

	return p_font;
}

TTF_Font * FontSDL::get_raw(unsigned const size) const
{
	if(mp_impl->m_handles.find(size) == mp_impl->m_handles.end())
	{
		// If we don't rewind the stream, opening font will fail silently
		SDL_RWseek(mp_impl->mp_stream.get(), SEEK_SET, 0);

		mp_impl->m_handles[size] = std::shared_ptr<TTF_Font>
			( TTF_OpenFontRW(mp_impl->mp_stream.get(), 0, size), &TTF_CloseFont);
		if(!mp_impl->m_handles[size])
			throw SDL_GetError();
	}

	return mp_impl->m_handles[size].get();
}

