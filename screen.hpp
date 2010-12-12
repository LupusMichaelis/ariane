
#ifndef HPP_SCREEN_SDL_QUEST
#	define HPP_SCREEN_SDL_QUEST

#	include <memory>


class Surface ;
class VideoMode ;
class Size ;

class Screen
{
	class Impl ;
	public:
		Screen(int width, int height, int depth) ;
		explicit Screen(VideoMode const & videomode) ;
			
		int width() const ;
		int height() const ;
		int depth() const ;

		void create(Surface & target, int height, int width) ;

	private:
		std::auto_ptr<Impl> mp_impl ;
} ;

#endif // define HPP_SCREEN_SDL_QUEST
