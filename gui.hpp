
#ifndef HPP_GUI_SDL_QUEST
#	define HPP_GUI_SDL_QUEST

#	include "videomode.hpp"

#	include <memory>
#	include <vector>

#	include <boost/utility.hpp>

class Size ;

class Surface ;
class EventLoop ;

class Gui
	: boost::noncopyable
{
	public:
		Gui(VideoMode const & set_videomode) ;
		~Gui() ;

		Surface const & screen() const ;
		Surface & screen() ;

		std::unique_ptr<Surface> surface(Size const & size) const ;
		std::unique_ptr<Surface> surface(std::string const & file_name) const ;
		std::unique_ptr<Surface> surface(Surface const & source) const ;

		EventLoop & event_loop() const ;

	private:
		void init_screen() const ;

	private:
		mutable
		std::unique_ptr<Surface>	mp_screen ;
		VideoMode					m_videomode ;

} /* class Gui */ ;


#endif // define HPP_GUI_SDL_QUEST
