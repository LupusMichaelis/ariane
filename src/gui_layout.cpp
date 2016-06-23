
#include "gui_layout.hpp"
#include "surface.hpp"
#include "grid_surface.hpp"
#include "font_manager.hpp"
#include "motif_library.hpp"
#include "text_surface.hpp"
#include "gui.hpp"

#include "memory.hpp"

#include <fstream>
#include <map>
#include <boost/algorithm/string.hpp>

struct GuiLayout::Impl
{
	Impl(GuiLayout & gui_layout, VideoMode const & videomode)
		: m_videomode(videomode)
		, m_font_manager(gui_layout, {})
	{ }

	VideoMode					m_videomode;
	FontManager					m_font_manager;
} /* struct GuiLayout::Impl */;

GuiLayout::GuiLayout(VideoMode const & set_videomode)
	: mp_impl(std::make_unique<Impl>(*this, set_videomode))
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError();
	//SDL_ShowCursor(SDL_DISABLE);
}

std::unique_ptr<Surface> GuiLayout::screen() const
{
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), videomode(), true);
}

GuiLayout::~GuiLayout()
{
	SDL_Quit();
}

VideoMode const GuiLayout::videomode() const
{
	return mp_impl->m_videomode;
}

std::unique_ptr<Surface> GuiLayout::surface(Size const size) const
{
	VideoMode v { size, videomode().depth() };
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), v);
}

std::unique_ptr<Surface> GuiLayout::surface(boost::filesystem::path const file_path) const
{
	return surface(file_path.native());
}

std::unique_ptr<Surface> GuiLayout::surface(std::string const file_name) const
{
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), file_name);
}

std::unique_ptr<Surface> GuiLayout::surface(Surface const & source) const
{
	auto & sdl_source = dynamic_cast<SurfaceSDL const &>(source);
	return std::make_unique<SurfaceSDL>(sdl_source);
}

std::unique_ptr<GridSurface> GuiLayout::grid_surface(Surface & reference, Size const sprite_size) const
{
	return std::make_unique<GridSurface>(reference, sprite_size);
}

#include <iostream>

std::unique_ptr<MotifLibrary> GuiLayout::motif_library(boost::filesystem::path const path) const
{
	auto p_motif_library = std::make_unique<MotifLibrary>();

	std::map<std::string, std::string> config;
	{
		boost::filesystem::path description_path = path;
		description_path += std::string{".def"};
		auto description = std::ifstream {description_path.c_str()};

		std::string buffer;
		while(std::getline(description, buffer))
		{
			std::cout << buffer << "\n";
			auto it_colon = std::find(buffer.cbegin(), buffer.cend(), ':');
			if(buffer.cend() != it_colon)
			{
				std::string key {buffer.cbegin(), it_colon};
				std::string value {++it_colon, buffer.cend()};

				boost::trim(key);
				boost::trim(value);

				config.insert(std::make_pair(key, value));
			}
		}
	}

	boost::filesystem::path image_path = path;
	image_path += std::string{"."} + config.at({"image format"});

	bool is_vertical {config.at({"layout"}) == "vertical"};
	Size sprite_size;
	{
		auto size = config.at("size");
		auto it_x = std::find(size.cbegin(), size.cend(), 'x');
		if(size.cend() == it_x)
			throw "Shit";

		std::string w {size.cbegin(), it_x};
		std::string h {++it_x, size.cend()};
		sprite_size = Size {std::atoi(w.c_str()), std::atoi(h.c_str())};
	}

	auto p_assets = surface(image_path);
	auto origin = Size {0, 0};

	std::vector<std::string> names;
	boost::split(names, config.at("names"), boost::is_any_of(","));

	for(auto const name: names)
	{
		auto p_image = surface(sprite_size);
		p_assets->crop(*p_image, origin, sprite_size);
		p_motif_library->add(name, p_image);
		if(is_vertical)
			origin += Size {0, sprite_size.height()};
		else
			origin += Size {sprite_size.width(), 0};
	}

	return p_motif_library;
}

void GuiLayout::save(Surface const & to_save, boost::filesystem::path const path) const
{
	auto & sdl_source = dynamic_cast<SurfaceSDL const &>(to_save);
	sdl_source.dump(path.c_str());
}

FontManager const & GuiLayout::fonts() const
{
	return const_cast<FontManager &>(const_cast<GuiLayout &>(*this).fonts());
}

FontManager & GuiLayout::fonts()
{
	return mp_impl->m_font_manager;
}

std::unique_ptr<TextSurface> GuiLayout::text(std::string const content, Pen const pen, Size const size)
{
	return std::make_unique<TextSurfaceSDL>(*this
			, create_videomode(size, videomode().depth())
			, content
			, pen
		);
}
