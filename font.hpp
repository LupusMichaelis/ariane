#ifndef HPP_FONT_QUEST
#	define HPP_FONT_QUEST

#	include "style.hpp"
#	include "tools.hpp"

#	include <string>
#	include <boost/filesystem.hpp>

class Pen ;
class GuiLayout ;
class _TTF_Font ;

class FontSDL
	: public Font
{
	public:
		typedef std::shared_ptr<FontSDL> SharedPtr ;

	public:
		FontSDL(GuiLayout & gui_layout, std::string const & name, unsigned const size) ;
		virtual
		~FontSDL() ;

		static
		SharedPtr make_from_file(GuiLayout & gui_layout, std::string const & name, unsigned const size
				, boost::filesystem::path const & filepath) ;

		_TTF_Font * get_raw() const ;

	private:
		class Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class FontSDL */ ;

#endif // HPP_FONT_QUEST
