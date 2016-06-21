#ifndef HPP_GUI_LAYOUT_SDL_QUEST
#	define HPP_GUI_LAYOUT_SDL_QUEST

#	include "videomode.hpp"

#	include <memory>
#	include <boost/filesystem.hpp>

class Size;
class Surface;
class TextSurface;
class GridSurface;
class Font;

class Pen;
class Style;
class FontManager;

class GuiLayout
{
	public:
		GuiLayout(VideoMode const & set_videomode);
		~GuiLayout();

		std::unique_ptr<Surface> screen() const;

		std::unique_ptr<Surface>		surface(Size const size) const;
		std::unique_ptr<Surface>		surface(std::string const file_name) const;
		std::unique_ptr<Surface>		surface(Surface const & source) const;
		std::unique_ptr<TextSurface>	text(std::string const content, Pen const pen, Size const size);

		std::unique_ptr<GridSurface> grid_surface(Surface & reference, Size const sprite_size) const;

		void save(Surface const & to_save, boost::filesystem::path const path) const;

		FontManager const & fonts() const;
		FontManager & fonts();

		VideoMode const videomode() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> mp_impl;

} /* class GuiLayout */;


#endif // define HPP_GUI_LAYOUT_SDL_QUEST
