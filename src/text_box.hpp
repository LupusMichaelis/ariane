#ifndef HPP_BOX_TEXT_WIDGET_QUEST
#	define HPP_BOX_TEXT_WIDGET_QUEST

#	include "box.hpp"

class TextBox
	: public Box
{
	protected:
		TextBox() = delete;
		TextBox(Gui & gui);

	public:
		typedef std::shared_ptr<TextBox>		SharedPtr;
		typedef std::weak_ptr<TextBox>			WeakPtr;

		static SharedPtr make(Gui & gui);
		virtual ~TextBox();

		void text(std::string const & new_text);
		std::string const & text() const;

	protected:
		virtual
		void draw();

	private:
		std::string m_text;

} /* class TextBox */;


#endif // HPP_BOX_TEXT_WIDGET_QUEST
