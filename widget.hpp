#ifndef HPP_WIDGET_QUEST
#	define HPP_WIDGET_QUEST

#	include "surface.hpp"
#	include "style.hpp"

#	include <boost/signals2.hpp>

class Widget ;
class Gui ;

class Widget
{
	public:
		explicit Widget(Gui & gui, Widget * p_parent = nullptr) ;
		virtual ~Widget() ;

		Surface const & surface() const ;
		Widget const & parent() const ;
		Gui const & gui() const ;
		Gui & gui() ;

		Style const & style() const ;
		void style(Style const &) ;

		Size const size() const ;

		void adopt(std::unique_ptr<Widget> adopted) ;

		void display() ;

		template <typename FunPtrT, typename ClassT, typename SlotFunT>
		void attach_event(FunPtrT const fun_ptr) ;
		void detach_events() ;

		Surface & surface() ;

	protected:
		virtual
		void draw() = 0 ;
		virtual
		void listen_events() = 0 ;

		void surface(std::unique_ptr<Surface> set_surface) ;
		std::vector<std::unique_ptr<Widget>> & children() ;

	private:
		Gui &						m_gui ;
		//std::unique_ptr<Surface>	mp_surface ;
		std::shared_ptr<Surface>	mp_surface ;
		Style						m_style ;

		// Managed connection, to avoid dandling events if this is deleted
		std::vector<boost::signals2::connection>
									m_cons ;

		Widget *					mp_parent ;
		std::vector<std::unique_ptr<Widget>>
									m_children ;

		Size						m_size ;

} /* class Widget */ ;

#	include "event.hpp"
#	include "gui.hpp"

template <typename FunPtrT, typename ClassT, typename SlotFunT>
void Widget::attach_event(FunPtrT const fun_ptr)
{
	auto wrapped_fun_ptr = boost::bind(fun_ptr, static_cast<ClassT * const>(this), _1, _2) ;
	EventLoop & ev_loop = gui().event_loop() ;
	auto con = ev_loop.attach_event(SlotFunT(wrapped_fun_ptr)) ;
	m_cons.push_back(con) ;
}

class Screen
	: public Widget
{
	public:
		explicit Screen(Gui & gui) ;
		virtual ~Screen() ;

	protected:
		virtual
		void draw() ;

		virtual
		void listen_events() ;

} /* class Screen */ ;

class Box
	: public Widget
{
	public:
		explicit Box(Gui & gui, Widget * p_parent) ;
		virtual ~Box() ;

	protected:
		virtual
		void draw() ;

		virtual
		void listen_events() ;

} /* class Box */ ;

class TextBox
	: public Box
{
	public:
		explicit TextBox(Gui & gui, Widget * p_parent) ;
		virtual ~TextBox() ;

		void text(std::string const & new_text) ;
		std::string const & text() const ;

	protected:
		virtual
		void draw() ;

		virtual
		void listen_events() ;

	private:
		std::string m_text ;

} /* class TextBox */ ;


#endif // HPP_WIDGET_QUEST
