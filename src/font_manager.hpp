#ifndef HPP_FONT_MANAGER_QUEST
#	define HPP_FONT_MANAGER_QUEST

#	include "memory.hpp"
#	include "font.hpp"

#	include <list>
#	include <boost/filesystem.hpp>

class Gui;

class FontManager
{
	public:
		FontManager(GuiLayout & gui_layout, std::list<boost::filesystem::path> const & font_paths);
		virtual
		~FontManager();

		Font::SharedPtr get(std::string const & name);

	protected:
		Font::SharedPtr make(std::string const & name);
		GuiLayout & gui_layout();

	private:
		struct Impl;
		std::unique_ptr<Impl>	mp_impl;

} /* class FontManager */;

#endif // HPP_FONT_MANAGER_QUEST

