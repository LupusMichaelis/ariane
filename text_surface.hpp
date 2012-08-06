#ifndef HPP_TEXT_SURFACE_QUEST
#	define HPP_TEXT_SURFACE_QUEST

#	include "api.hpp"
#	include "font.hpp"
#	include "surface.hpp"

class TextSurfaceSDL
	: virtual public SurfaceSDL
	, virtual public TextSurface
{
	public:
		TextSurfaceSDL(GuiLayout & set_gui_layout
				, VideoMode set_videomode
				, std::string const & set_text
				, FontSDL::SharedPtr set_p_font
				, RGBColor const & set_color) ;
		virtual
		~TextSurfaceSDL() ;

		using SurfaceSDL::gui_layout ;

		virtual
		std::string const & text() const ;
		virtual
		RGBColor const & color() const ;
		virtual
		Font const & font() const ;

	private:

		void init() ;

		class Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class TextSurfaceSDL */ ;

#endif // HPP_TEXT_SURFACE_QUEST
