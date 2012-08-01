#ifndef HPP_GUI_SDL_QUEST
#	define HPP_GUI_SDL_QUEST


#	include <memory>
#	include <boost/utility.hpp>

class VideoMode ;

class Widget ;
class Screen ;
class Box ;
class TextBox ;
class Style ;

class GuiLayout ;
class EventLoop ;

class Gui
	: boost::noncopyable
{
	public:
		Gui(VideoMode const & set_videomode) ;
		~Gui() ;

		Screen const & screen() const ;
		Screen & screen() ;

		std::shared_ptr<Box> box(Box & parent, Style const & set_style) const ;
		std::shared_ptr<TextBox> text_box(Box & parent, Style const & set_style) const ;

		Style const style() const ;

		EventLoop const & event_loop() const ;
		EventLoop & event_loop() ;

		GuiLayout const & layout() const ;

		void refresh() ;

	private:
		class Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class Gui */ ;


#endif // define HPP_GUI_SDL_QUEST
