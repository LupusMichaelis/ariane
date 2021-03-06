
#include "surface.hpp"
#include "color.hpp"
#include "style.hpp"
#include "gui_layout.hpp"

#include "memory.hpp"


#include <algorithm>

#include <boost/format.hpp>

void ignore_screen_release (SDL_Surface *) { }

struct SurfaceSDL::Impl
{
	typedef std::unique_ptr<SDL_Surface, void (*) (SDL_Surface *)> unique_ptr;

	void ensure()
	{
		if(!mp_surface)
			throw SDL_GetError();
	}

	Impl(SDL_Surface * p_raw
		, unique_ptr::deleter_type deleter
		, GuiLayout & gui_layout
		, bool is_screen = false
		)
		: mp_surface {p_raw, deleter}
		, m_gui_layout(gui_layout)
		, m_is_screen(is_screen)
	{
	}

	Impl(GuiLayout & gui_layout, VideoMode const & videomode, bool is_screen = false)
		: Impl
		{
			is_screen
				  ? SDL_SetVideoMode(videomode.width(), videomode.height(), videomode.depth(), SDL_DOUBLEBUF)
				  : SDL_CreateRGBSurface(SDL_SWSURFACE, videomode.width(), videomode.height(), videomode.depth()
						, 0, 0, 0, 0)
			, is_screen
				? &ignore_screen_release // Subsequent calls to SDL_SetVideoMode manage memory allocation
				: &SDL_FreeSurface
			, gui_layout
			, is_screen
		}
	{
		ensure();
	}

	explicit Impl(GuiLayout & gui_layout, std::string const & filename)
		: mp_surface { SDL_LoadBMP(filename.c_str()), &SDL_FreeSurface }
		, m_gui_layout(gui_layout)
		, m_is_screen(false)
	{
		ensure();
	}

	unique_ptr			mp_surface;
	GuiLayout &			m_gui_layout;
	bool				m_is_screen;

} /* SurfaceSDL::Impl */;

SurfaceSDL::SurfaceSDL(GuiLayout & gui_layout, VideoMode videomode, bool is_screen /*= false*/)
	: Surface()
	, mp_impl(std::make_unique<Impl>(gui_layout, videomode, is_screen))
{
}

SurfaceSDL::SurfaceSDL(GuiLayout & gui_layout, std::string filename)
	: Surface()
	, mp_impl(std::make_unique<Impl>(gui_layout, filename))
{
}

SurfaceSDL::SurfaceSDL(SurfaceSDL const & copied)
	: Surface(copied)
	, mp_impl(std::make_unique<Impl>(const_cast<GuiLayout &>(copied.gui_layout()), copied.videomode()))
{
	draw_static(copied, nullptr, nullptr);
}

SurfaceSDL::~SurfaceSDL()
{
}

GuiLayout const & SurfaceSDL::gui_layout() const
{
	return mp_impl->m_gui_layout;
}

GuiLayout & SurfaceSDL::gui_layout()
{
	return mp_impl->m_gui_layout;
}

SDL_Surface * SurfaceSDL::get_raw() const
{
	return mp_impl->mp_surface.get();
}

void SurfaceSDL::set_raw(SDL_Surface * p_raw)
{
	mp_impl->mp_surface.reset(p_raw); //std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)>(p_raw, &SDL_FreeSurface)) ;
}

VideoMode const SurfaceSDL::videomode() const
{
	return create_videomode(get_raw()->w, get_raw()->h, get_raw()->format->BitsPerPixel);
}

void SurfaceSDL::border(Border const border)
{
	Uint32 mapped_color = SDL_MapRGB(get_raw()->format
			, border.color().red()
			, border.color().green()
			, border.color().blue()
			);

	Uint16 const s = border.size();
	Uint16 const h = get_raw()->h;
	Uint16 const w = get_raw()->w;

	SDL_Rect border_part {0, 0, s, h};
	int ret = SDL_FillRect(get_raw(), &border_part, mapped_color);
	if(ret == -1)
		throw SDL_GetError();

	border_part.x = w - s;
	border_part.y = 0;
	border_part.w = w;
	border_part.h = h;
	ret = SDL_FillRect(get_raw(), &border_part, mapped_color);
	if(ret == -1)
		throw SDL_GetError();

	border_part.x = s;
	border_part.y = 0;
	border_part.w = w - s;
	border_part.h = s;
	ret = SDL_FillRect(get_raw(), &border_part, mapped_color);
	if(ret == -1)
		throw SDL_GetError();

	border_part.x = s;
	border_part.y = h - s;
	border_part.w = w - s;
	border_part.h = h;
	ret = SDL_FillRect(get_raw(), &border_part, mapped_color);
	if(ret == -1)
		throw SDL_GetError();
}

void SurfaceSDL::fill(RGBColor const color)
{
	Uint32 mapped_color = SDL_MapRGB(get_raw()->format, color.red(), color.green(), color.blue());
	int ret = SDL_FillRect(get_raw(), 0, mapped_color);
	if(ret == -1)
		throw SDL_GetError();
}

void SurfaceSDL::fill(Surface const & pattern, Size const from, Size const to)
{
	Size size(to);
	auto p_buffer = gui_layout().surface(to - from);

	Size next(0, 0);
	do
	{
		p_buffer->draw(pattern, next);

		next.width(next.width() + width(pattern));
		if(next.width() > to.width())
		{
			next.height(next.height() + height(pattern));
			next.width(0);
		}
	} while(next.height() < to.height());

	draw(*p_buffer, from);
}

void SurfaceSDL::draw(Surface const & motif)
{
	draw_static(motif, nullptr, nullptr);
}

void SurfaceSDL::draw(Surface const & motif, Size const at)
{
	draw_static(motif, &at, nullptr);
}

void SurfaceSDL::draw(Surface const & motif, Size const at, Size const by)
{
	draw_static(motif, &at, &by);
}

void SurfaceSDL::draw_static(Surface const & motif, Size const * at, Size const * by)
{
	std::unique_ptr<SDL_Rect> position, boundary;

	if(at)
	{
		position = std::make_unique<SDL_Rect>();
		position->x = at->width();
		position->y = at->height();
	}

	if(by)
	{
		boundary = std::make_unique<SDL_Rect>();
		boundary->w = by->width();
		boundary->h = by->height();
	}

	SurfaceSDL const & sdl_motif = dynamic_cast<SurfaceSDL const &>(motif);

	SDL_Surface * p_from = sdl_motif.get_raw();
	SDL_Surface * p_to = get_raw();
	int ret = SDL_BlitSurface(p_from, boundary.get(), p_to, position.get());

	if(ret == -1)
		throw SDL_GetError();
	else if(ret == -2)
		throw "Must reload resources";
}

#include <cstring>
// XXX Big trouble here. We have to destroy the SDL_Surface before we can resize the
// screen. This means if we can't recover, the object is in inconsistent state.
// XXX I copy the surface structure before I destroy it. My guess is its undefined
// behaviour. Should look if they are a better (and efficient) way to copy the content of the surface.
void SurfaceSDL::resize(Size const new_size)
{
	auto new_videomode = create_videomode(new_size, videomode().depth());
	auto p_copy = gui_layout().surface(*this);
	mp_impl = std::make_unique<Impl>(gui_layout(), new_videomode, mp_impl->m_is_screen);
	draw(*p_copy);
}

#include <iostream>
#include <boost/format.hpp>

void SurfaceSDL::update() const
{
	SDL_UpdateRect(get_raw(), 0, 0, 0, 0);
}

void SurfaceSDL::dump(std::string const filename) const
{
	int r = SDL_SaveBMP(get_raw(), filename.c_str());
	if(r < 0)
		throw SDL_GetError();
}

void SurfaceSDL::crop(Surface & target, Size const origin, Size const size) const
{
	SDL_Rect orig;

	orig.x = origin.width();
	orig.y = origin.height();
	orig.w = size.width();
	orig.h = size.height();

	auto & actual = dynamic_cast<SurfaceSDL &>(target);

	SDL_Surface * p_to = actual.get_raw();
	SDL_Surface * p_from = get_raw();
	int ret = SDL_BlitSurface(p_from, &orig, p_to, 0);

	if(ret == -1)
		throw SDL_GetError();
	else if(ret == -2)
		throw "Must reload resources";
}

void SurfaceSDL::write(std::string const message, Style const style)
{
	Size const & at = style.padding();
	Size const & by = style.size();

	std::unique_ptr<Surface> p_text { gui_layout().text(message, style.pen(), by - at - at) };
	draw(*p_text, at, by);
}
