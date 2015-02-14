#ifndef HPP_FONT_QUEST
#	define HPP_FONT_QUEST

#	include "style.hpp"
#	include "memory.hpp"

#	include <string>
#	include <boost/filesystem.hpp>

class Pen ;
class GuiLayout ;
class _TTF_Font ;
struct SDL_RWops ;

class FontSDL
	: public Font
{
	public:
		typedef std::shared_ptr<FontSDL> SharedPtr ;

	public:
		FontSDL(GuiLayout & gui_layout, std::string const & name, SDL_RWops * p_stream) ;
		virtual
		~FontSDL() ;

		static
		SharedPtr make_from_file(GuiLayout & gui_layout, std::string const & name, boost::filesystem::path const & filepath) ;

		_TTF_Font * get_raw(unsigned const size) const ;

	private:
		struct Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class FontSDL */ ;

#endif // HPP_FONT_QUEST
