#ifndef HPP_DECORATOR_WIDGET_QUEST
#	define HPP_DECORATOR_WIDGET_QUEST

#	include "widget.hpp"

class Decorator
	: public Widget
{
	public:
		explicit Decorator(std::unique_ptr<Widget> p_set_decorated) ;

		Widget const & decorated() const ;
		Widget & decorated() ;

		void decorated(std::unique_ptr<Widget> p_new_decorated) ;

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

	//protected:
		void surface(std::unique_ptr<Surface> p_set_surface) ;

	protected:
		virtual
		void draw() = 0 ;
		virtual
		void listen_events() = 0 ;

	private:
		std::unique_ptr<Widget> mp_decorated ;

} /* class Decorator */ ;

#endif // HPP_DECORATOR_WIDGET_QUEST
