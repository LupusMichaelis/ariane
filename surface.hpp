
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"
#	include "api.hpp"

#	include <memory>

#	include <boost/utility.hpp>

#	include <SDL/SDL.h>


class RGBColor ;
class Style ;
class VideoMode ;
class Size ;
class Gui ;


class RawSurfaceMemory
	: boost::noncopyable
{
	public:
		virtual
		void init(VideoMode const & videomode) = 0 ;

		virtual
		void release() = 0 ;

		virtual
		~RawSurfaceMemory() { }

		SDL_Surface * get_raw()
		{
			return mp_surface ;
		}

	protected:
		explicit RawSurfaceMemory(SDL_Surface * p_surface)
			: mp_surface(p_surface)
		{
			if(!mp_surface)
				throw SDL_GetError() ;
		}

		void set_raw(SDL_Surface * p_raw)
		{
			if(!p_raw)
				throw SDL_GetError() ;
			mp_surface = p_raw ;
		}

		SDL_Surface * init_regular(VideoMode const & videomode)
		{
			return SDL_CreateRGBSurface(SDL_SWSURFACE
				, videomode.width(), videomode.height(), videomode.depth()
				//, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000) ;
				, 0, 0, 0, 0) ;
		}

		void release_regular()
		{
			if(mp_surface)
				SDL_FreeSurface(mp_surface) ;
		}

		void reset()
		{
			// XXX pure virtual will be called, possible memory leak right now
			release() ;
			mp_surface = 0 ;
		}

	private:
		SDL_Surface * mp_surface ;
} /* class RawSurfaceMemory */ ;

class SurfaceMemory
	: public RawSurfaceMemory
{
	public:
		explicit SurfaceMemory(VideoMode const & videomode)
			: RawSurfaceMemory(init_regular(videomode))
		{
		}

		virtual
		void init(VideoMode const & videomode)
		{
			reset() ;
			set_raw(init_regular(videomode)) ;
		}

		virtual
		void release()
		{
			release_regular() ;
		}

	protected:
		explicit SurfaceMemory(SDL_Surface * p_surface)
			: RawSurfaceMemory(p_surface)
		{
		}
} /* class SurfaceMemory */ ;

class ImageMemory
	: public SurfaceMemory
{
	public:
		explicit ImageMemory(std::string const & filename)
			: SurfaceMemory(init_image(filename))
		{
		}

	private:
		SDL_Surface * init_image(std::string const & filename)
		{
			return SDL_LoadBMP(filename.c_str()) ;
		}

} /* class ImageMemory */ ;

class ScreenMemory
	: public RawSurfaceMemory
{
	public:
		explicit ScreenMemory(VideoMode const & videomode)
			: RawSurfaceMemory(init_screen(videomode))
		{
		}

		virtual
		void init(VideoMode const & videomode)
		{
			reset() ;
			set_raw(init_screen(videomode)) ;
		}

		virtual
		void release()
		{
			SDL_Quit() ;
		}

		virtual
		~ScreenMemory() { reset() ; }

	private:
		SDL_Surface * init_screen(VideoMode const & videomode)
		{
			return SDL_SetVideoMode(videomode.width(), videomode.height(), videomode.depth(), SDL_DOUBLEBUF) ;
		}

} /* class ScreenMemory */ ;

struct SurfaceMemoryDeleter
	: std::unary_function<RawSurfaceMemory *, void>
{
	void operator() (RawSurfaceMemory * p_raw)
	{
		p_raw->release() ;
		delete p_raw ;
		p_raw = 0 ;
	}
} /* struct SurfaceMemoryDeleter */ ;

class SurfaceSDL
	: public Surface
{
	public:
		typedef std::unique_ptr<RawSurfaceMemory, SurfaceMemoryDeleter>
			impl_ptr ;

	public:
		SurfaceSDL(Gui & gui, impl_ptr p_surface) ;
		SurfaceSDL(SurfaceSDL const & copied) ;

		~SurfaceSDL() ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		VideoMode const videomode() const ;

		void draw(Surface const & motif) ;
		void draw(Surface const & motif, Size const & at) ;
		void update() const ;
		void fill(RGBColor const & color) ;
		void fill(Surface const & pattern) ;
		void fill(Surface const & pattern, Size const & from, Size const & to) ;
		void resize(Size const & new_size) ;

		void dump(std::string const & filename) ;

		void crop(Surface & target, Size const & origin, Size const & size) const ;

		void write(std::string const & message, Size const & at, Style const & style) ;

		const Gui & gui() const ;
		Gui & gui() ;

	protected:
		void set_raw(SDL_Surface *) ;
		SDL_Surface * get_raw() const ;

	private:
		void draw_static(Surface const & motif, Size const & at) ;

	private:
		Gui &								m_gui ;
		impl_ptr							mp_surface ;
} ;


#endif // define HPP_SURFACE_SDL_QUEST
