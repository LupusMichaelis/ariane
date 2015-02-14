#ifndef HPP_SCREEN_WIDGET_QUEST
#	define HPP_SCREEN_WIDGET_QUEST

#	include "box.hpp"

class Screen
	: public Box
{
	protected:
		Screen() = delete;
		explicit Screen(Gui & gui);

	public:
		typedef std::shared_ptr<Screen>		SharedPtr;
		typedef std::weak_ptr<Screen>		WeakPtr;

		static SharedPtr make(Gui & gui);

	public:
		virtual ~Screen();

	protected:
		virtual
		void init();

} /* class Screen */;

#endif // HPP_SCREEN_WIDGET_QUEST
