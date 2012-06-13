
#include "surface.hpp"
#include "color.hpp"
#include "style.hpp"
#include "gui_layout.hpp"

#include "tools.hpp"


#include <algorithm>

#include <boost/format.hpp>

SurfaceSDL::SurfaceSDL(GuiLayout & gui_layout, impl_ptr p_surface)
	: Surface()
	, m_gui_layout(gui_layout)
	, mp_surface(move(p_surface))
{
}

SurfaceSDL::SurfaceSDL(SurfaceSDL const & copied)
	: Surface(copied)
	, m_gui_layout(copied.m_gui_layout)
	, mp_surface(std::make_unique<SurfaceMemory, impl_ptr::deleter_type>(copied.videomode()))
{
	draw_static(copied, Size {0,0}) ;
}

SurfaceSDL::~SurfaceSDL()
{
}

GuiLayout const & SurfaceSDL::gui_layout() const
{
	return m_gui_layout ;
}

GuiLayout & SurfaceSDL::gui_layout()
{
	return m_gui_layout ;
}

SDL_Surface * SurfaceSDL::get_raw() const
{
	return mp_surface->get_raw() ;
}

VideoMode const SurfaceSDL::videomode() const
{
	return create_videomode(get_raw()->w, get_raw()->h, get_raw()->format->BitsPerPixel) ;
}

void SurfaceSDL::fill(RGBColor const & color)
{
	Uint32 mapped_color = SDL_MapRGB(get_raw()->format, color.red(), color.green(), color.blue()) ;
	int ret = SDL_FillRect(get_raw(), 0, mapped_color) ;
	if(ret == -1)
		throw SDL_GetError() ;
}

void SurfaceSDL::fill(Surface const & pattern, Size const & from, Size const & to)
{
	Size size(to) ;
	auto p_buffer = gui_layout().surface(to - from) ;

	Size next(0, 0) ;
	do
	{
		p_buffer->draw(pattern, next) ;

		next.width(next.width() + width(pattern)) ;
		if(next.width() > to.width())
		{
			next.height(next.height() + height(pattern)) ;
			next.width(0) ;
		}
	} while(next.height() < to.height()) ;

	draw(*p_buffer, from) ;
}

void SurfaceSDL::draw(Surface const & motif)
{
	draw(motif, Size(0, 0)) ;
}

void SurfaceSDL::draw(Surface const & motif, Size const & at)
{
	draw_static(motif, at) ;
}

void SurfaceSDL::draw_static(Surface const & motif, Size const & at)
{
	SDL_Rect dst ;
	dst.x = at.width() ;
	dst.y = at.height() ;

	SurfaceSDL const & sdl_motif = dynamic_cast<SurfaceSDL const &>(motif) ;
	
	SDL_Surface * p_from = sdl_motif.get_raw() ;
	SDL_Surface * p_to = get_raw() ;
	int ret = SDL_BlitSurface(p_from, 0, p_to, &dst) ;
	
	if(ret == -1)
		throw SDL_GetError() ;
	else if(ret == -2)
		throw "Must reload resources" ;
}

#include <cstring>
// XXX Big trouble here. We have to destroy the SDL_Surface before we can resize the
// screen. This means if we can't recover, the object is in inconsistent state.
// XXX I copy the surface structure before I destroy it. My guess is its undefined
// behaviour. Should look if they are a better (and efficient) way to copy the content of the surface.
void SurfaceSDL::resize(Size const & new_size)
{
	auto new_videomode = create_videomode(new_size, videomode().depth()) ;
	auto p_surface = gui_layout().surface(*this) ;
	mp_surface->init(new_videomode) ;
	draw(*p_surface) ;
}

void SurfaceSDL::update() const
{
	SDL_UpdateRect(get_raw(), 0, 0, 0, 0) ;
}

void SurfaceSDL::dump(std::string const & filename)
{
	int r = SDL_SaveBMP(get_raw(), filename.c_str()) ;
	if(r < 0)
		throw SDL_GetError() ;
}

void SurfaceSDL::crop(Surface & target, Size const & origin, Size const & size) const
{
	SDL_Rect orig ;

	orig.x = origin.width() ;
	orig.y = origin.height() ;
	orig.w = size.width() ;
	orig.h = size.height() ;

	auto & actual = dynamic_cast<SurfaceSDL &>(target) ;

	SDL_Surface * p_to = actual.get_raw() ;
	SDL_Surface * p_from = get_raw() ;
	int ret = SDL_BlitSurface(p_from, &orig, p_to, 0) ;

	if(ret == -1)
		throw SDL_GetError() ;
	else if(ret == -2)
		throw "Must reload resources" ;
}

#include <SDL/SDL_ttf.h>

void SurfaceSDL::write(std::string const & message, Style const & style)
{
	int ret = TTF_Init() ;
	if(ret == -1)
		throw SDL_GetError() ;

	Pen const & pen = style.pen() ;

	std::string font_name = (boost::format("/usr/share/fonts/truetype/msttcorefonts/%s.ttf")
			% pen.font().name()).str() ;

	TTF_Font * font = 0 ;
	font = TTF_OpenFont(font_name.c_str(), pen.size()) ;
	if(!font)
		throw SDL_GetError() ;

	SDL_Surface * p_text = TTF_RenderText_Solid(font, message.c_str()
			, {pen.color().red(), pen.color().green(), pen.color().blue(), 0, }) ;
	if(!p_text)
		throw SDL_GetError() ;

	Size const & at = style.padding() ;

	SDL_Rect dst = { (Sint16) at.width(), (Sint16) at.height(), 0, 0, };
	//SDL_Rect box = { 0, 0, (Sint16) size.width(), (Sint16) size.height(), } ;

	SDL_Surface * p_to = get_raw() ;

	ret = SDL_BlitSurface(p_text, 0/*&box*/, p_to, &dst) ;
	if(ret == -1)
		throw SDL_GetError() ;

	SDL_FreeSurface(p_text) ;
}
