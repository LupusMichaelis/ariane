#ifndef HPP_GUI_SDL_QUEST
#	define HPP_GUI_SDL_QUEST

#	include "videomode.hpp"
#	include "event.hpp"
#	include "style.hpp"

#	include <memory>
#	include <vector>

#	include <boost/utility.hpp>

class Size ;

class Surface ;
class SurfaceSDL ;

class GuiLayout
{
	public:
		GuiLayout(VideoMode const & set_videomode) ;
		~GuiLayout() ;

		std::unique_ptr<Surface> screen() ;
		std::unique_ptr<Surface> surface(Size const & size) const ;
		std::unique_ptr<Surface> surface(std::string const & file_name) const ;
		std::unique_ptr<Surface> surface(Surface const & source) const ;

	private:
		VideoMode					m_videomode ;

} /* class GuiLayout */ ;

class Widget ;
class Screen ;
class Box ;
class TextBox ;

class Gui
	: boost::noncopyable
{
	public:
		Gui(VideoMode const & set_videomode) ;
		~Gui() ;

		Screen const & screen() const ;
		Screen & screen() ;

		Box * const box(Widget & parent, Style const & set_style) ;
		TextBox * const text_box(Widget & parent, Style const & set_style) ;

		Style const style() const ;

		EventLoop const & event_loop() const ;
		EventLoop & event_loop() ;

		GuiLayout & layout() ;

	private:
		GuiLayout					m_layout ;
		EventLoop					m_event_loop ;

		std::unique_ptr<Screen>		mp_screen ;

} /* class Gui */ ;


#endif // define HPP_GUI_SDL_QUEST
