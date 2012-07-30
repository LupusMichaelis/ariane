#ifndef HPP_GUI_SDL_QUEST
#	define HPP_GUI_SDL_QUEST

#	include "videomode.hpp"
#	include "event.hpp"
#	include "style.hpp"

#	include <memory>
#	include <vector>

#	include <boost/utility.hpp>

class Widget ;
class Screen ;
class Box ;
class TextBox ;
class Grid ;
class Image ;

class GuiLayout ;

class Gui
	: boost::noncopyable
{
	public:
		Gui(VideoMode const & set_videomode) ;
		~Gui() ;

		Screen const & screen() const ;
		Screen & screen() ;

		Box * const box(Widget & parent, Style const & set_style) ;
		Box * const box(Style const & set_style) ;

		TextBox * const text_box(Widget & parent, Style const & set_style) ;

		Image * const image(Widget & parent, Style const & set_style, std::string const & filename) ;

		Grid * const grid(Widget & decorated, Size const & box_size) ;

		Style const style() const ;

		EventLoop const & event_loop() const ;
		EventLoop & event_loop() ;

		GuiLayout & layout() ;

	private:
		std::unique_ptr<GuiLayout>	mp_layout ;
		EventLoop					m_event_loop ;

		std::unique_ptr<Screen>		mp_screen ;
		std::vector<std::unique_ptr<Widget>>
									m_orphans ;

} /* class Gui */ ;


#endif // define HPP_GUI_SDL_QUEST
