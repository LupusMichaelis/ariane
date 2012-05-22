#include "screen.hpp"
#include "canvas.hpp"
#include "color.hpp"
#include "style.hpp"
#include "event.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>


#include <boost/format.hpp>
#include <boost/bind.hpp>

template<typename T>
T & get_or_throw(std::shared_ptr<T> const & sptr)
{
	if(!sptr) throw std::bad_alloc() ;
	return *sptr ;
}

class Engine ;

class Interface
{
	public:
		explicit Interface(Engine & engine)
			: m_engine(engine)
		{ }

		virtual ~Interface() { detach_events() ; }

		void background(std::shared_ptr<Canvas> const & new_background)
								{ mp_background = new_background ; }
		Canvas & background()	{ return get_or_throw(mp_background) ; }
		Engine & engine()		{ return m_engine ; }

		void display() ;

	protected:
		template <typename FunPtrT, typename ClassT, typename SlotFunT>
		void attach_event(FunPtrT const fun_ptr) ;
		void detach_events() ;

		virtual
		void init_display() = 0 ;
		virtual
		void listen_events() = 0 ;

	private:
		Engine &					m_engine ;

		std::shared_ptr<Canvas>		mp_background ;

		// Managed connection, to avoid dandling events if this is deleted
		std::vector<boost::signals2::connection>
								m_cons ;

} /* class Interface */ ;

class MenuInterface
	: public Interface
{
	public:
		explicit MenuInterface(Engine & engine)
			: Interface(engine)
			, m_current("first")
			, m_entries()
			, m_positions()
		{ }

		void move(EventLoop &, KeyEvent const & ke) ;

		void entry_next() ;
		void entry_previous() ;
		void select() ;

	private:
		virtual
		void init_display() ;
		virtual
		void listen_events() ;

		std::string					m_current ;
		std::map<std::string, std::shared_ptr<Canvas>>
									m_entries ;
		std::map<std::string, std::shared_ptr<Canvas>>
									m_bg_entries ;

		std::map<std::string, Size> m_positions ;

} /* class MenuInterface */ ;

class QuestInterface
	: public Interface
{
	public:
		explicit QuestInterface(Engine & engine)
			: Interface(engine)
			, m_question("To be, or not to be.")
			, m_current('a')
			, m_good_answer('b')
		{
			m_answers['a'] = "Born" ;
			m_answers['b'] = "to be" ;
			m_answers['c'] = "Alive" ;
		}

		void move(EventLoop &, KeyEvent const & ke) ;

		void answer_next() ;
		void answer_previous() ;
		void select() ;

	private:
		virtual
		void init_display() ;
		virtual
		void listen_events() ;

		std::string					m_question ;
		char						m_current ;
		char						m_good_answer ;
		std::map<char, std::string>	m_answers ;
		std::map<char, std::shared_ptr<Canvas>>
									m_sprites ;
		std::map<char, std::shared_ptr<Canvas>>
									m_bg_sprites ;
		std::map<char, Size>		m_positions ;

} /* class MenuInterface */ ;

class Engine
{
	public:
		Engine()
		{
		}

		void run() ;

		EventLoop & event_loop()			{ return m_ev_loop ; }
		Screen & screen()					{ return *mp_screen ; }
		KeyBoard const & keyboard()	const	{ return m_kb ; }

		void title_screen() ;
		void game_over() ;
		void quest(std::string const & quest_name) ;

	private:
		std::shared_ptr<Interface>	mp_interface ;

		EventLoop					m_ev_loop ;
		KeyBoard					m_kb ;

		std::shared_ptr<Screen>		mp_screen ;

} /* class Engine */ ;

template <typename FunPtrT, typename ClassT, typename SlotFunT>
void Interface::attach_event(FunPtrT const fun_ptr)
{
	auto wrapped_fun_ptr = boost::bind(fun_ptr, static_cast<ClassT * const>(this), _1, _2) ;
	EventLoop & ev_loop = engine().event_loop() ;
	boost::signals2::connection con ;
	con = ev_loop.attach_event(SlotFunT(wrapped_fun_ptr)) ;
	m_cons.push_back(con) ;
}

void Interface::display()
{
	init_display() ;
	listen_events() ;
}

void Interface::detach_events()
{
	std::for_each(m_cons.begin(), m_cons.end(), std::mem_fun_ref(&boost::signals2::connection::disconnect)) ;
}

void MenuInterface::init_display()
{
	Screen & screen = engine().screen() ;
	screen.fill(create_color(0x0)) ;

	Surface & screen_surface = dynamic_cast<Surface &>(screen) ;
	std::shared_ptr<Canvas> new_background ;

	std::shared_ptr<Style> p_style ;
	Style::create(p_style, screen) ;
	p_style->color(create_color(0x0000bb)) ;
	p_style->font("Comic_Sans_MS") ;
	p_style->size(16) ;

	std::shared_ptr<Canvas> p_title ;
	Canvas::create(p_title, create_videomode(8 * 50, 2 * 50, 32)) ;
	p_title->fill(create_color(0x111111)) ;
	p_title->write("Hill quest", Size(), *p_style) ;
	screen_surface.draw(*p_title, Size(2 * 50, 1 * 50)) ;

	clone(new_background, screen_surface) ;
	background(new_background) ;
	background().fill(create_color(0x00)) ;

	m_positions["first"] = Size(6 * 50, 4 * 50) ;
	Canvas::create(m_entries["first"], create_videomode(4 * 50, 1 * 50, 32)) ;
	m_entries["first"]->fill(create_color(0x00aa)) ;
	m_entries["first"]->write("First quest", Size(), *p_style) ;

	Canvas::create(m_bg_entries["first"], create_videomode(4 * 50, 1 * 50, 32)) ;
	m_bg_entries["first"]->fill(create_color(0x0022)) ;
	m_bg_entries["first"]->write("First quest", Size(), *p_style) ;

	Canvas::create(m_entries["second"], create_videomode(4 * 50, 1 * 50, 32)) ;
	m_entries["second"]->fill(create_color(0x00aa)) ;
	m_positions["second"] = Size(6 * 50, 5 * 50) ;
	m_entries["second"]->write("Second quest", Size(), *p_style) ;

	Canvas::create(m_bg_entries["second"], create_videomode(4 * 50, 1 * 50, 32)) ;
	m_bg_entries["second"]->fill(create_color(0x0022)) ;
	m_bg_entries["second"]->write("Second quest", Size(), *p_style) ;

	screen.draw(*m_entries["first"], m_positions["first"]) ;
	screen.draw(*m_bg_entries["second"], m_positions["second"]) ;
	screen.update() ;
}

void MenuInterface::listen_events()
{
	// This is complex, I should be able to make something simpler (automatic FunPtr
	// signature, for example)
	attach_event<void (MenuInterface::*)(EventLoop &, KeyEvent const &)
		, MenuInterface
		, EventLoop::keyboard_event_type::slot_function_type
		>(&MenuInterface::move) ;
}

void MenuInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(ke.pressing())
		return ;

	// preserve background of destination
	Screen & screen = engine().screen() ;
	screen.draw(background(), m_positions[m_current]) ;

	KeyBoard const & kb = engine().keyboard() ;

	if(ke.key() == kb.up())
		entry_previous() ;
	else if(ke.key() == kb.down())
		entry_next() ;
	else if(ke.key() == kb.enter())
		select() ;
}

void MenuInterface::entry_next()
{
	Screen & screen = engine().screen() ;
	screen.draw(*m_bg_entries["first"], m_positions[m_current]) ;

	m_current = "second" ;

	screen.draw(*m_entries[m_current], m_positions[m_current]) ;
	screen.update() ;
}

void MenuInterface::entry_previous()
{
	Screen & screen = engine().screen() ;
	screen.draw(*m_bg_entries["second"], m_positions[m_current]) ;

	m_current = "first" ;

	screen.draw(*m_entries[m_current], m_positions[m_current]) ;
	screen.update() ;
}

void MenuInterface::select()
{
	engine().quest(m_current) ;
}

void QuestInterface::init_display()
{
	Screen & screen = engine().screen() ;
	screen.fill(create_color(0x0)) ;

	Surface & screen_surface = dynamic_cast<Surface &>(screen) ;

	std::shared_ptr<Style> p_style ;
	Style::create(p_style, screen) ;
	p_style->color(create_color(0xffffff)) ;
	p_style->font("Comic_Sans_MS") ;
	p_style->size(16) ;

	std::shared_ptr<Canvas> title ;
	Canvas::create(title, create_videomode(8 * 50, 2 * 50, 32)) ;
	title->fill(create_color(0x111111)) ;
	title->write(m_question, Size(), *p_style) ;
	screen_surface.draw(*title, Size(2 * 50, 1 * 50)) ;

	std::shared_ptr<Canvas> new_background ;
	clone(new_background, screen_surface) ;
	background(new_background) ;
	background().fill(create_color(0x00)) ;

	Size position = Size(6 * 50, 4 * 50) ;
	for(auto it_answer = m_answers.begin() ; it_answer != m_answers.end() ; ++it_answer)
	{
		std::shared_ptr<Canvas> p_sprite ;

		Canvas::create(p_sprite, create_videomode(4 * 50, 1 * 50, 32)) ;
		p_sprite->fill(create_color(0x00aa)) ;
		p_sprite->write(it_answer->second, Size(), *p_style) ;
		m_sprites[it_answer->first] = p_sprite ;

		Canvas::create(p_sprite, create_videomode(4 * 50, 1 * 50, 32)) ;
		p_sprite->fill(create_color(0x0066)) ;
		p_sprite->write(it_answer->second, Size(), *p_style) ;
		m_bg_sprites[it_answer->first] = p_sprite ;

		screen.draw(*(it_answer->first == m_current ? m_sprites[it_answer->first] : m_bg_sprites[it_answer->first]), position) ;
		m_positions[it_answer->first] = position ;
		position.height(position.height() + 1 * 50) ;
	}

	screen.update() ;
}

void QuestInterface::listen_events()
{
	// This is complex, I should be able to make something simpler (automatic FunPtr
	// signature, for example)
	attach_event<void (QuestInterface::*)(EventLoop &, KeyEvent const &)
		, QuestInterface
		, EventLoop::keyboard_event_type::slot_function_type
		>(&QuestInterface::move) ;
}

void QuestInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(ke.pressing())
		return ;

	KeyBoard const & kb = engine().keyboard() ;

	if(ke.key() == kb.up())
		answer_previous() ;
	else if(ke.key() == kb.down())
		answer_next() ;
	else if(ke.key() == kb.enter())
		select() ;
}

void QuestInterface::answer_next()
{
	auto it_current = m_sprites.find(m_current) ; 

	Screen & screen = engine().screen() ;
	screen.draw(*m_bg_sprites[it_current->first], m_positions[m_current]) ;

	++it_current ;
	if(it_current == m_sprites.end())
		it_current = m_sprites.begin() ;

	m_current = it_current->first ;

	screen.draw(*it_current->second, m_positions[m_current]) ;
	screen.update() ;
}

void QuestInterface::answer_previous()
{
	auto it_current = m_sprites.find(m_current) ; 

	Screen & screen = engine().screen() ;
	screen.draw(*m_bg_sprites[it_current->first], m_positions[m_current]) ;

	--it_current ;
	if(it_current == m_sprites.end())
		it_current = --m_sprites.end() ;

	m_current = it_current->first ;

	screen.draw(*it_current->second, m_positions[m_current]) ;
	screen.update() ;
}

void QuestInterface::select()
{
	if(m_current == m_good_answer)
		engine().title_screen() ;
	else
		engine().game_over() ;
}

void Engine::run()
{
	Screen::create(mp_screen, create_videomode(12 * 50, 7 * 50, 16)) ;
	title_screen() ;
	m_ev_loop() ;
}

void Engine::game_over()
{
	std::shared_ptr<Style> p_style ;
	Style::create(p_style, *mp_screen) ;
	p_style->font("Verdana") ;
	p_style->color(create_color(0x00bbbb)) ;
	p_style->size(30) ;
	mp_screen->write("Looser!", Size(), *p_style) ;
	mp_screen->update() ;
	sleep(2) ;
	m_ev_loop.stop() ;
}

void Engine::quest(std::string const & quest_name)
{
	{
		std::shared_ptr<Interface>	p_interface(new QuestInterface(*this)) ;
		mp_interface = p_interface ;
	}
	mp_interface->display() ;
}

void Engine::title_screen()
{
	{
		std::shared_ptr<Interface>	p_interface(new MenuInterface(*this)) ;
		mp_interface = p_interface ;
	}
	mp_interface->display() ;
}

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	cout << "Running quest\n" ;
	try
	{
		Engine engine ;
		engine.run() ;
	}
	catch(std::exception const & e)
	{
		cout << format("Exception '%s'\n") % e.what() ;
	}
	catch(char const * raw)
	{
		cout << format("Exception '%s'\n") % raw ;
	}
	catch(...)
	{
		cout << format("Strange exception raised\n") ;
	}
}
