#ifndef HPP_WIDGET_QUEST
#	define HPP_WIDGET_QUEST

#	include "style.hpp"

#	include <boost/signals2.hpp>

class Surface ;
class Widget ;
class Gui ;

class Widget
{
	public:
		typedef std::vector<std::unique_ptr<Widget>> list_type ;

	public:
		explicit Widget(Widget * p_parent = nullptr) ;
		virtual ~Widget() ;

		// Tree management
		virtual
		void adopt(list_type::value_type new_child) = 0 ;
		virtual
		list_type::value_type abandon(list_type::value_type::element_type & abandoned_child) = 0 ;
		virtual
		bool const has_child() const = 0 ;
		virtual
		list_type const & children() const = 0 ;
		virtual
		std::vector<list_type::value_type::pointer> children() = 0 ;

		bool const has_parent() const ;
		Widget & parent() ;
		void parent(Widget & new_parent) ;
		//

		virtual
		Surface const & surface() const = 0 ;
		virtual
		Surface & surface() = 0 ;
		virtual
		Gui const & gui() const = 0 ;
		virtual
		Gui & gui() = 0 ;
		virtual
		Style const & style() const = 0 ;
		virtual
		void style(Style const &) = 0 ;
		virtual
		Size const size() const = 0 ;

		virtual
		void display() ;

		template <typename FunPtrT, typename ClassT, typename SlotFunT>
		void attach_event(FunPtrT const fun_ptr) ;
		void detach_events() ;

		virtual
		void surface(std::unique_ptr<Surface> set_surface) = 0 ; // XXX should be protected, cls Decorator prevent it

		virtual
		void draw() = 0 ;
		virtual
		void listen_events() = 0 ;

	private:
		Widget *					mp_parent ;

		// Managed connection, to avoid dandling events if this is deleted
		std::vector<boost::signals2::connection>
									m_cons ;
} /* class Widget */ ;

class ComposedWidget
	: public Widget
{
	public:
		ComposedWidget(Gui & gui, Widget * p_parent = nullptr) ;

		void adopt(list_type::value_type new_child) ;
		list_type::value_type abandon(list_type::value_type::element_type & abandoned_child) ;
		bool const has_child() const ;

		list_type const & children() const ;
		std::vector<list_type::value_type::pointer> children() ;

		virtual
		Surface const & surface() const ;
		virtual
		Surface & surface() ;
		virtual
		Gui const & gui() const ;
		virtual
		Gui & gui() ;
		virtual
		Style const & style() const ;
		virtual
		void style(Style const &) ;
		virtual
		Size const size() const ;

	protected:
		void surface(std::unique_ptr<Surface> p_set_surface) ;

	private:
		Gui &						m_gui ;
		std::unique_ptr<Surface>	mp_surface ;
		Style						m_style ;
		list_type					m_children ;

} /* class ComposedWidget */ ;

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

class Image
	: public ComposedWidget
{
	public:
		Image(Gui & gui, Widget * p_parent) ;
		virtual ~Image() ;

		void filename(std::string const & file_name) ;
		std::string const & filename() ;

		void load() ;

		virtual
		void draw() ;
		virtual
		void listen_events() ;

	private:
		std::string		m_filename ;

} /* class Image */ ;

#endif // HPP_WIDGET_QUEST
