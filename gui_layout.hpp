#ifndef HPP_GUI_LAYOUT_SDL_QUEST
#	define HPP_GUI_LAYOUT_SDL_QUEST

#	include "videomode.hpp"

#	include <memory>

class Size ;
class Surface ;
class Grid ;

class GuiLayout
{
	public:
		GuiLayout(VideoMode const & set_videomode) ;
		~GuiLayout() ;

		std::unique_ptr<Surface> screen() ;
		std::unique_ptr<Surface> surface(Size const & size) const ;
		std::unique_ptr<Surface> surface(std::string const & file_name) const ;
		std::unique_ptr<Surface> surface(Surface const & source) const ;

		std::unique_ptr<Grid> grid(Surface & reference, Size const & sprite_size) ;

	private:
		VideoMode					m_videomode ;

} /* class GuiLayout */ ;


#endif // define HPP_GUI_LAYOUT_SDL_QUEST
