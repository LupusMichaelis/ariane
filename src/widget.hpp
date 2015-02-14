#ifndef HPP_WIDGET_QUEST
#	define HPP_WIDGET_QUEST

#	include "api.hpp"

class Gui;

class Widget
	: public EventTarget
	, public Drawable
{
	protected:
		Widget(Gui & gui);
		void surface(std::unique_ptr<Surface> p_surface);
		virtual void init() = 0;

	public:
		virtual ~Widget();

		virtual Surface /*const*/ & surface();
		virtual void draw() = 0;

		virtual Gui const & gui();

		virtual void style(Style const & new_style);
		virtual Style const & style() const;
		virtual Style style();

	private:
		struct Impl;
		std::unique_ptr<Impl> mp_impl;

} /* class Widget */;

#endif // HPP_WIDGET_QUEST
