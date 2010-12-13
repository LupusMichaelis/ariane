
#ifndef HPP_SCREEN_SDL_QUEST
#	define HPP_SCREEN_SDL_QUEST

#	include "surface.hpp"

#	include <memory>

class VideoMode ;
class Size ;
typedef Size Position ;

class Screen
	: protected Surface
{
	class Impl ;
	public:
		Screen(int width, int height, int depth) ;
		explicit Screen(VideoMode const & videomode) ;
			
		int width() const ;
		int height() const ;
		int depth() const ;

		void create(Surface & target, VideoMode const & videomode) ;
		void draw(Surface const & motif) ;
		void draw(Surface const & motif, Position const & at) ;

		void update() const ;

	private:
		std::auto_ptr<Impl> mp_impl ;
} ;

#endif // define HPP_SCREEN_SDL_QUEST
