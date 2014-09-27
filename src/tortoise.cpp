
#include "tortoise.hpp"
#include "engine.hpp"
#include "style.hpp"
#include "screen.hpp"
#include "gui.hpp"

#include "quest_engine.hpp"

#include "SDL/SDL.h"

void TortoiseInterface::display()
{
	Screen & screen = engine().gui().screen() ;

	Style bg_style = screen.style() ;
	bg_style.position({10, 10}) ;
	bg_style.size(bg_style.size() - Size {20, 20}) ;
	bg_style.color(create_color(0xaaaa00)) ;
	auto p_container = screen.gui().box(screen, bg_style) ;

	Style sprite_style = screen.style() ;
	sprite_style.size(Size {20, 20}) ;
	sprite_style.color(create_color(0x00aa)) ;

	sprite_style.color(create_color(0x00)) ;
	mp_tortoise = screen.gui().box(*p_container, sprite_style) ;

	set_container(p_container) ;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL) ;
}

void TortoiseInterface::move(EventLoop &, MouseButtonEvent const & me)
{
	Style tortoise_style = mp_tortoise->style() ;
	Style bg_style = engine().gui().screen().style() ;

	RGBColor tortoise_color = tortoise_style.color() ;
	RGBColor bg_color = bg_style.color() ;

	tortoise_style.color(bg_color) ;
	mp_tortoise->style(tortoise_style) ;

	bg_style.color(tortoise_color) ;
	engine().gui().screen().style(bg_style) ;
}

void TortoiseInterface::move(EventLoop &, MouseEvent const & me)
{
	Style tortoise_style = mp_tortoise->style() ;
	tortoise_style.position(me.position()) ;
	mp_tortoise->style(tortoise_style) ;
}

void TortoiseInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(!ke.pressing())
		return ;

	KeyBoard const & kb = engine().keyboard() ;

	if(ke.key() == kb.up())
		move_up() ;
	if(ke.key() == kb.down())
		move_down() ;
	if(ke.key() == kb.right())
		move_right() ;
	if(ke.key() == kb.left())
		move_left() ;
}

void TortoiseInterface::move_left()
{
	Style tortoise_style = mp_tortoise->style() ;
	Size new_position = tortoise_style.position() - Size(10, 0) ;
	tortoise_style.position(new_position) ;
	mp_tortoise->style(tortoise_style) ;
}

void TortoiseInterface::move_right()
{
	Style tortoise_style = mp_tortoise->style() ;
	Size new_position = tortoise_style.position() + Size(10, 0) ;
	tortoise_style.position(new_position) ;
	mp_tortoise->style(tortoise_style) ;
}

void TortoiseInterface::move_up()
{
	Style tortoise_style = mp_tortoise->style() ;
	Size new_position = tortoise_style.position() - Size(0, 10) ;
	tortoise_style.position(new_position) ;
	mp_tortoise->style(tortoise_style) ;
}

void TortoiseInterface::move_down()
{
	Style tortoise_style = mp_tortoise->style() ;
	Size new_position = tortoise_style.position() + Size(0, 10) ;
	tortoise_style.position(new_position) ;
	mp_tortoise->style(tortoise_style) ;
}

////////////////////////////////////////////////////////////////////////////////

struct TortoiseModel::Impl
{
	Size m_position ;
} /* struct TortoiseModel */ ;

TortoiseModel::TortoiseModel()
	: mp_impl(std::make_unique<Impl>())
{
}

TortoiseModel::~TortoiseModel()
{
}

Size const & TortoiseModel::position() const
{
	return mp_impl->m_position ;
}
