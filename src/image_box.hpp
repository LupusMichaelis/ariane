#ifndef HPP_BOX_IMAGE_WIDGET_QUEST
#	define HPP_BOX_IMAGE_WIDGET_QUEST

#	include "box.hpp"
#	include "memory.hpp"

class ImageBox
	: public Box
{
	protected:
		ImageBox() = delete;
		ImageBox(Gui & gui, std::unique_ptr<Surface> p_image);

	public:
		typedef std::shared_ptr<ImageBox>		SharedPtr;
		typedef std::weak_ptr<ImageBox>			WeakPtr;

		static SharedPtr make(Gui & gui, std::string file_name);
		static SharedPtr make(Gui & gui, Surface const & surface);
		//static SharedPtr make(Gui & gui, Surface const & surface, Size from, Size to);

		virtual ~ImageBox();

	protected:
		void draw();

	private:
		class Impl;
		std::unique_ptr<Impl> mp_impl;

} /* class ImageBox */;

#endif // HPP_BOX_IMAGE_WIDGET_QUEST
