#include "color.hpp"
#include "style.hpp"
#include "event.hpp"
#include "gui.hpp"
#include "api.hpp"
#include "tools.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>


#include <boost/format.hpp>
#include <boost/bind.hpp>

template<typename T>
T & get_or_throw(std::unique_ptr<T> const & sptr)
{
	if(!sptr) throw std::bad_alloc() ;
	return *sptr ;
}

template<typename T>
T & get_or_throw(std::shared_ptr<T> const & sptr)
{
	if(!sptr) throw std::bad_alloc() ;
	return *sptr ;
}

class Engine ;
class Surface ;

class Interface
{
	public:
		explicit Interface(Engine & engine)
			: m_engine(engine)
		{ }

		virtual ~Interface() { detach_events() ; }

		void background(std::unique_ptr<Surface> & new_background)
								{ mp_background = std::move(new_background) ; }
		Surface & background()	{ return get_or_throw(mp_background) ; }
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
		std::unique_ptr<Surface>	mp_background ;

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
		std::map<std::string, std::unique_ptr<Surface>>
									m_entries ;
		std::map<std::string, std::unique_ptr<Surface>>
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
		std::map<char, std::unique_ptr<Surface>>
									m_sprites ;
		std::map<char, std::unique_ptr<Surface>>
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

		KeyBoard const & keyboard()	const	{ return m_kb ; }

		void title_screen() ;
		void game_over() ;
		void quest(std::string const & quest_name) ;

		Gui const & gui() const { return *mp_gui ; }
		Gui & gui() { return *mp_gui ; }

	private:
		std::unique_ptr<Interface>	mp_interface ;
		std::unique_ptr<Gui>		mp_gui ;

		KeyBoard					m_kb ;


} /* class Engine */ ;

template <typename FunPtrT, typename ClassT, typename SlotFunT>
void Interface::attach_event(FunPtrT const fun_ptr)
{
	auto wrapped_fun_ptr = boost::bind(fun_ptr, static_cast<ClassT * const>(this), _1, _2) ;
	EventLoop & ev_loop = engine().gui().event_loop() ;
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
	Gui & gui = engine().gui() ;
	Surface & screen = gui.screen() ;
	screen.fill(create_color(0x0)) ;

	Style style(screen) ;
	style.color(create_color(0x0000bb)) ;
	style.font("Comic_Sans_MS") ;
	style.size(16) ;

	auto p_title = gui.surface(Size { 8 * 50, 2 * 50}) ;
	p_title->fill(create_color(0x111111)) ;
	p_title->write("Hill quest", Size(), style) ;
	screen.draw(*p_title, Size(2 * 50, 1 * 50)) ;
	{
		auto new_background = gui.surface(screen) ;
		background(new_background) ;
	}
	background().fill(create_color(0x00)) ;

	m_positions["first"] = Size(6 * 50, 4 * 50) ;
	m_entries["first"] = gui.surface(Size {4 * 50, 1 * 50}) ;
	m_entries["first"]->fill(create_color(0x00aa)) ;
	m_entries["first"]->write("First quest", Size(), style) ;

	m_bg_entries["first"] = gui.surface(Size {4 * 50, 1 * 50}) ;
	m_bg_entries["first"]->fill(create_color(0x0022)) ;
	m_bg_entries["first"]->write("First quest", Size(), style) ;

	m_entries["second"] = gui.surface(Size {4 * 50, 1 * 50}) ;
	m_entries["second"]->fill(create_color(0x00aa)) ;
	m_positions["second"] = Size(6 * 50, 5 * 50) ;
	m_entries["second"]->write("Second quest", Size(), style) ;

	m_bg_entries["second"] = gui.surface(Size {4 * 50, 1 * 50}) ;
	m_bg_entries["second"]->fill(create_color(0x0022)) ;
	m_bg_entries["second"]->write("Second quest", Size(), style) ;

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
	Surface & screen = engine().gui().screen() ;
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
	Surface & screen = engine().gui().screen() ;
	screen.draw(*m_bg_entries["first"], m_positions[m_current]) ;

	m_current = "second" ;

	screen.draw(*m_entries[m_current], m_positions[m_current]) ;
	screen.update() ;
}

void MenuInterface::entry_previous()
{
	Surface & screen = engine().gui().screen() ;
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
	Gui & gui = engine().gui() ;
	Surface & screen = gui.screen() ;
	screen.fill(create_color(0x0)) ;

	Surface & screen_surface = dynamic_cast<Surface &>(screen) ;

	Style style(screen) ;
	style.color(create_color(0xffffff)) ;
	style.font("Comic_Sans_MS") ;
	style.size(16) ;

	auto title = gui.surface(Size {8 * 50, 2 * 50}) ;
	title->fill(create_color(0x111111)) ;
	title->write(m_question, Size(), style) ;
	screen_surface.draw(*title, Size(2 * 50, 1 * 50)) ;

	auto new_background = gui.surface(screen) ;
	background(new_background) ;
	background().fill(create_color(0x00)) ;

	Size position = Size(6 * 50, 4 * 50) ;
	for(auto it_answer = m_answers.begin() ; it_answer != m_answers.end() ; ++it_answer)
	{
		auto p_sprite = gui.surface(Size {4 * 50, 1 * 50}) ;
		p_sprite->fill(create_color(0x00aa)) ;
		p_sprite->write(it_answer->second, Size(), style) ;
		m_sprites[it_answer->first] = std::move(p_sprite) ;

		p_sprite = gui.surface(Size {4 * 50, 1 * 50}) ;
		p_sprite->fill(create_color(0x0066)) ;
		p_sprite->write(it_answer->second, Size(), style) ;
		m_bg_sprites[it_answer->first] = std::move(p_sprite) ;

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

	Surface & screen = engine().gui().screen() ;
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

	Surface & screen = engine().gui().screen() ;
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
	mp_gui = std::make_unique<Gui>(create_videomode(12 * 50, 7 * 50, 16)) ;
	title_screen() ;
	gui().event_loop()() ;
}

void Engine::game_over()
{
	Surface & screen = gui().screen() ;
	Style style(screen) ;
	style.font("Verdana") ;
	style.color(create_color(0x00bbbb)) ;
	style.size(30) ;
	screen.write("Looser!", Size(), style) ;
	screen.update() ;
	sleep(2) ;
	gui().event_loop().stop() ;
}

void Engine::quest(std::string const & quest_name)
{
	mp_interface = std::make_unique<QuestInterface>(*this) ;
	mp_interface->display() ;
}

void Engine::title_screen()
{
	mp_interface = std::make_unique<MenuInterface>(*this) ;
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
