#include "color.hpp"
#include "style.hpp"
#include "event.hpp"
#include "gui.hpp"
#include "widget.hpp"
#include "api.hpp"
#include "tools.hpp"
#include "box.hpp"
#include "text_box.hpp"
#include "screen.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>


#include <boost/format.hpp>
#include <boost/bind.hpp>

class Surface ;
class Interface ;

class Engine
{
	public:
		Engine()
			: m_gui(create_videomode(640, 480, 16))
		{
		}

		void run() ;

		void title_screen() ;
		void game_over() ;
		void quest(std::string const & quest_name) ;

		Gui & gui()							{ return m_gui ; }
		KeyBoard const & keyboard()	const	{ return m_kb ; }

	private:
		void init_title_screen() ;
		void heart_beat(EventLoop &) { gui().refresh() ; }

		std::unique_ptr<Interface>	mp_interface ;

		KeyBoard					m_kb ;

		Gui 						m_gui ;

} /* class Engine */ ;

class Interface
{
	public:
		explicit Interface(Engine & engine)
			: m_engine(engine)
		{ }

		Engine & engine()		{ return m_engine ; }
		Style title_style() const ;

		virtual
		void display() = 0 ;

		virtual ~Interface() { unlisten_events() ; }

	protected:
		std::vector<EventLoop::con_type> & cons() { return m_cons ; }

	private:
		void unlisten_events()
		{
			std::for_each(m_cons.begin(), m_cons.end(), std::mem_fun_ref(&EventLoop::con_type::disconnect)) ;
		}

	private:
		Engine &					m_engine ;

		// Managed connection, to avoid dandling events if this is deleted
		std::vector<EventLoop::con_type>
									m_cons ;
} /* class Interface */ ;

class MenuInterface
	: public Interface
{
	public:
		explicit MenuInterface(Engine & engine)
			: Interface(engine)
			, m_current("first")
			, m_widgets()
		{
		}

		void move(EventLoop &, KeyEvent const & ke) ;

		void entry_next() ;
		void entry_previous() ;
		void select() ;

	private:
		virtual
		void display() ;
		void listen_events() ;

		std::string					m_current ;
		std::map<std::string, TextBox::SharedPtr>
									m_widgets ;

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
		void display() ;
		void listen_events() ;

		std::string					m_question ;
		char						m_current ;
		char						m_good_answer ;
		std::map<char, std::string>	m_answers ;
		std::map<char, Box::SharedPtr>
									m_widgets ;

} /* class QuestInterface */ ;

Style Interface::title_style() const
{
	Style style ;
	style.color(create_color(0x0)) ;
	style.size(Size { 8 * 50, 2 * 50}) ;
	style.position(Size {2 * 50, 1 * 50}) ;

	Pen pen = style.pen() ;
	pen.color(create_color(0x660000)) ;
	pen.font(Font {"Comic_Sans_MS"}) ;
	pen.size(16) ;
	style.pen(pen) ;

	return style ;
}

void MenuInterface::display()
{
	Gui & gui = engine().gui() ;

	Style containter_style { gui.screen().style() } ;
	containter_style.color(create_color(0x660000)) ;
	auto p_container = gui.box(gui.screen(), containter_style) ;

	{
		auto p_title = gui.text_box(*p_container, title_style()) ;
		p_title->text("Hill quest") ;
	}

	Style entry_style ;
	entry_style.color(create_color(0xaaaaaa)) ;
	{
		Pen pen ;
		pen.color(create_color(0x00aa)) ;
		pen.font(Font {"Verdana"}) ;
		pen.size(11) ;
		entry_style.pen(pen) ;
	}

	entry_style.size(Size {4 * 50, 1 * 50} ) ;

	entry_style.position(Size {6 * 50, 3 * 50} ) ;
	m_widgets["first"] = gui.text_box(*p_container, entry_style) ;
	m_widgets["first"]->text("First quest") ;

	entry_style.color(create_color(0x111111)) ;
	entry_style.position(Size {6 * 50, 4 * 50} ) ; 
	m_widgets["second"] = gui.text_box(*p_container, entry_style) ;
	m_widgets["second"]->text("Second quest") ;

	entry_style.position(Size {6 * 50, 5 * 50} ) ; 
	m_widgets["third"] = gui.text_box(*p_container, entry_style) ;
	m_widgets["third"]->text("Third quest") ;

	gui.refresh() ;
	if(!cons().size()) listen_events() ;
}

void MenuInterface::listen_events()
{
	EventLoop & ev_loop = engine().gui().event_loop() ;
	EventLoop::con_type con ;

	void (MenuInterface::*oks)(EventLoop &, KeyEvent const &) = &MenuInterface::move ;
	auto wrapped_oks = boost::bind(oks, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(wrapped_oks)) ;
	cons().push_back(con) ;
}

void MenuInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(ke.pressing())
		return ;

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
	auto it_current = m_widgets.find(m_current) ; 
	{
		Style ws { it_current->second->style() } ;
		ws.color(create_color(0x111111)) ;
		it_current->second->style(ws) ;
	}

	++it_current;
	if(it_current == m_widgets.end())
		it_current = m_widgets.begin() ;

	{
		Style ws { it_current->second->style() } ;
		ws.color(create_color(0xaaaaaa)) ;
		it_current->second->style(ws) ;
	}

	m_current = it_current->first ;
}

void MenuInterface::entry_previous()
{
	auto it_current = m_widgets.find(m_current) ; 
	{
		Style ws { it_current->second->style() } ;
		ws.color(create_color(0x111111)) ;
		it_current->second->style(ws) ;
	}

	--it_current;
	if(it_current == m_widgets.end())
		it_current = --m_widgets.end() ;

	{
		Style ws { it_current->second->style() } ;
		ws.color(create_color(0xaaaaaa)) ;
		it_current->second->style(ws) ;
	}

	m_current = it_current->first ;
}

void MenuInterface::select()
{
	engine().quest(m_current) ;
}

void QuestInterface::display()
{
	Gui & gui = engine().gui() ;

	auto p_container = gui.box(gui.screen(), gui.screen().style()) ;

	{
		auto p_title = gui.text_box(*p_container, title_style()) ;
		p_title->text(m_question) ;
	}

	Style active_style ;
	active_style.position(Size {6 * 50, 4 * 50} ) ;
	active_style.size(Size {4 * 50, 1 * 50 }) ;
	active_style.color(create_color(0x00aa)) ;

	{
		Pen pen = active_style.pen() ;
		pen.color(create_color(0x111111)) ;
		pen.font(Font {"Arial"}) ;
		pen.size(16) ;
		active_style.pen(pen) ;
	}

	Style inactive_style {active_style} ;
	inactive_style.color(create_color(0x0066));

	//letter_style.size(Size {4 * 50, 1 * 50}) ;

	RGBColor active_color {create_color(0x00aa)} ;

	for(auto it_answer = m_answers.begin() ; it_answer != m_answers.end() ; ++it_answer)
	{
		TextBox::SharedPtr p_answer = gui.text_box(*p_container,
				it_answer->first == m_current ? active_style : inactive_style) ;
		p_answer->text(it_answer->second) ;

		m_widgets[it_answer->first] = p_answer ;

		inactive_style.position(inactive_style.position() + Size {0, 1 * 50}) ;
		inactive_style.position(inactive_style.position() + Size {0, 1 * 50}) ;
	}
	if(!cons().size()) listen_events() ;
}

void QuestInterface::listen_events()
{
	EventLoop & ev_loop = engine().gui().event_loop() ;
	EventLoop::con_type con ;

	void (QuestInterface::*oks)(EventLoop &, KeyEvent const &) = &QuestInterface::move ;
	auto wrapped_oks = boost::bind(oks, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(wrapped_oks)) ;
	cons().push_back(con) ;
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
	auto it_current = m_widgets.find(m_current) ; 

	Style current_style = it_current->second->style() ;
	current_style.color(create_color(0x0066));
	it_current->second->style(current_style) ;


	++it_current ;
	if(it_current == m_widgets.end())
		it_current = m_widgets.begin() ;

	m_current = it_current->first ;

	current_style = it_current->second->style() ;
	current_style.color(create_color(0x00aa));
	it_current->second->style(current_style) ;
}

void QuestInterface::answer_previous()
{
	auto it_current = m_widgets.find(m_current) ; 

	Style current_style = it_current->second->style() ;
	current_style.color(create_color(0x0066));
	it_current->second->style(current_style) ;


	--it_current ;
	if(it_current == m_widgets.end())
		it_current = --m_widgets.end() ;

	m_current = it_current->first ;

	current_style = it_current->second->style() ;
	current_style.color(create_color(0x00aa));
	it_current->second->style(current_style) ;

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
	title_screen() ;
	mp_interface->display() ;

	EventLoop & ev_loop = gui().event_loop() ;
	void (Engine::*oks)(EventLoop &) = &Engine::heart_beat ;
	auto wrapped_oks = boost::bind(oks, this, _1) ;
	auto con = ev_loop.attach_event(EventLoop::time_event_type::slot_function_type(wrapped_oks)) ;
	ev_loop() ;
}

void Engine::game_over()
{
	Pen pen ;
	pen.font(Font {"Verdana"}) ;
	pen.color(create_color(0x00bbbb)) ;
	pen.size(30) ;

	Style style ;
	style.position( {10, 10} ) ;
	style.size( {100, 50} ) ;
	style.pen(pen) ;

	TextBox::SharedPtr p_b = gui().text_box(gui().screen(), style) ;
	p_b->text("Looser!") ;

	gui().refresh() ;
	sleep(2) ;
	gui().event_loop().stop() ;
}

void Engine::quest(std::string const & quest_name)
{
	{
		std::unique_ptr<Interface> p_interface = std::make_unique<QuestInterface>(*this) ;
		std::swap(mp_interface, p_interface) ;
	}
	mp_interface->display() ;
	gui().refresh() ;
}

void Engine::title_screen()
{
	{
		std::unique_ptr<Interface> p_interface = std::make_unique<MenuInterface>(*this) ;
		std::swap(mp_interface, p_interface) ;
	}
	mp_interface->display() ;
	gui().refresh() ;
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
