#include "event.hpp"

#include <unistd.h>
#include <cassert>

#include <memory>
#include <iostream>
#include <boost/format.hpp>

#include <functional>

using std::cout ;
using std::endl ;
using boost::format ;

void print_key_event(EventLoop &, KeyEvent const & ev)
{
	std::cout << boost::format("key event '%d'\n") % ev.key().value() ;
}

void print_mouse_event(EventLoop &, MouseEvent const & ev)
{
	std::cout << boost::format("mouse event (%d,%d) (%c%c%c%c%c)\n")
		% ev.position().width()
		% ev.position().height()
		% (ev.buttons()[0] ? '+' : '-')
		% (ev.buttons()[1] ? '+' : '-')
		% (ev.buttons()[2] ? '+' : '-')
		% (ev.buttons()[3] ? '+' : '-')
		% (ev.buttons()[4] ? '+' : '-')
		;
}

template <int K>
void on_key_stop(EventLoop & ev_loop, KeyEvent const & ev)
{
	if(!ev.release())
		ev_loop.stop() ;
}

void wait()
{
	EventLoop ev_loop ;
	ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(on_key_stop<'q'>)) ;
	ev_loop() ;
}

#include "color.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"
#include "widget.hpp"

void test_base()
{
	GuiLayout gui_layout {create_videomode(320, 280, 16)} ;
	std::unique_ptr<Surface> p_screen = gui_layout.screen() ;
	Surface & screen = *p_screen ;
	screen.fill(create_color(0xaaff00)) ;

	assert(width(screen) == 320) ;
	assert(height(screen) == 280) ;
	assert(depth(screen) == 16) ;

	auto p_s1 = gui_layout.surface(Size {320, 280}) ;
	p_s1->fill(create_color(0x00ff00)) ;
	screen.draw(*p_s1) ;

	auto p_s2 = gui_layout.surface(Size {40, 40}) ;
	p_s2->fill(create_color(0xaa)) ;
	screen.draw(*p_s2, Size(20, 20)) ;

	auto p_s3 = gui_layout.surface(Size {30, 30}) ;
	p_s3->fill(create_color(0xffff00)) ;
	screen.draw(*p_s3, Size(10, 10)) ;

	auto p_s4 = gui_layout.surface(Size {60, 60}) ;
	p_s4->draw(screen) ;
	screen.fill(*p_s4, Size(60, 60), Size {150, 150}) ;

	screen.update() ;

	screen.dump(std::string(".test_screen.bmp")) ;

	wait() ;
}


void test_load_image()
{
	GuiLayout gui_layout {create_videomode(320, 280, 16)} ;
	std::unique_ptr<Surface> p_screen = gui_layout.screen() ;
	Surface & screen = *p_screen ;

	std::string filename("gfx/kraland_shapes.bmp") ;
	auto p_images = gui_layout.surface(filename) ;

	assert(width(*p_images) == 672) ;
	assert(height(*p_images) == 480) ;
	assert(depth(*p_images) == 24) ;

	screen.draw(*p_images) ;
	screen.update() ;

	wait() ;
}

void test_resize()
{
	GuiLayout gui_layout {create_videomode(320, 280, 16)} ;
	std::unique_ptr<Surface> p_screen = gui_layout.screen() ;
	Surface & screen = *p_screen ;

	screen.fill(create_color(0xaaaaaa)) ;
	screen.update() ;

	wait() ;
	screen.resize(create_size(480, 320)) ;
	screen.update() ;
	wait() ;

	auto p_s1 = gui_layout.surface(Size {20, 20}) ;
	p_s1->fill(create_color(0xaa)) ;

	screen.draw(*p_s1) ;
	screen.update() ;

	p_s1->resize(create_size(50, 50)) ;
	screen.draw(*p_s1, create_size(50, 50)) ;
	screen.update() ;

	wait() ;
}

void test_event()
{
	Gui gui {create_videomode(672, 480, 24)} ;
	gui.layout().screen() ;

	EventLoop & ev_loop = gui.event_loop() ;
	ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(print_key_event)) ;
	ev_loop.attach_event(EventLoop::mouse_button_event_type::slot_function_type(print_mouse_event)) ;
	ev_loop.attach_event(EventLoop::mouse_motion_event_type::slot_function_type(print_mouse_event)) ;
	ev_loop() ;
}

#include "style.hpp"
#include "box.hpp"
#include "screen.hpp"
#include "text_box.hpp"
#include "visitor.hpp"

void test_write()
{
	Gui gui {create_videomode(640, 480, 24)} ;
	Box::SharedPtr p_box ;

	{
		Style style = gui.style() ;
		style.position({10, 10}) ;
		style.size({600, 400}) ;
		style.color(create_color(0xffff)) ;

		p_box = gui.box(gui.screen(), style) ;
	}

	{
		Style style = gui.style() ;
		style.color(create_color(0x0)) ;

		Pen pen = style.pen() ;
		pen.font(Font {"Comic_Sans_MS"}) ;
		pen.color(create_color(0xffffff)) ;
		pen.size(16u) ;
		style.pen(pen) ;
		style.padding(Size {10, 10}) ;

		style.size(Size {300, 100}) ;
		style.position(Size {50, 50}) ;

		TextBox::SharedPtr p_tb ;

		style.color(create_color(0xff)) ;
		p_tb = gui.text_box(gui.screen(), style) ;
		p_tb->text("Rock'n'roll!") ;
		adopt(*p_box, *p_tb) ;

		style.color(create_color(0xff00)) ;
		style.position(Size {50, 200}) ;
		pen.size(10u) ;
		style.pen(pen) ;
		p_tb = gui.text_box(gui.screen(), style) ;
		p_tb->text("Rock'n'roll!") ;
		adopt(*p_box, *p_tb) ;

		style.color(create_color(0xff0000)) ;
		style.position(Size {50, 350}) ;
		pen.size(30u) ;
		style.pen(pen) ;
		p_tb = gui.text_box(gui.screen(), style) ;
		p_tb->text("Rock'n'roll!") ;
		adopt(*p_box, *p_tb) ;
	}

	gui.refresh() ;

	int n = 10 ;
	do
	{
		sleep(1) ;
		Drawable & c = dynamic_cast<Drawable &>(*children(gui.screen())[0]) ;
		Style style = c.style() ;
		style.position(style.position() + Size {10, 10}) ;
		c.style(style) ;
		gui.refresh() ;
	}
	while(--n) ;

	wait() ;
}

void test_widget()
{
	Gui gui {create_videomode(320, 280, 24)} ;
	Style box_style = gui.screen().style() ;
	box_style.position(Size {30, 10}) ;
	box_style.size(Size {300, 30}) ;
	box_style.color(create_color(0x0)) ;

	Pen pen = box_style.pen() ;
	pen.font(Font {"Comic_Sans_MS"}) ;
	pen.color(create_color(0xffffff)) ;
	pen.size(16u) ;
	box_style.pen(pen) ;

	auto p_box = gui.text_box(gui.screen(), box_style) ;
	p_box->text("WTF") ;

	adopt(gui.screen(), *p_box) ;

	Style screen_style {box_style} ;
	screen_style.color(create_color(0xff)) ;
	gui.screen().style(screen_style) ;

	gui.refresh() ;
	wait() ;
}

