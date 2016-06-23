#ifndef HPP_MOTIF_LIBRARY
#	define HPP_MOTIF_LIBRARY

#	include "memory.hpp"

#	include <string>

class Gui;
class Box;
class ImageBox;
class Surface;

class MotifLibrary
{
	public:
		MotifLibrary();
		~MotifLibrary();
		std::shared_ptr<ImageBox> image_box(Gui & gui, std::string reference) const;
		void add(std::string name, std::unique_ptr<Surface> & p_motif);

	private:
		class Impl;
		std::unique_ptr<Impl> mp_impl;
};

#endif // HPP_MOTIF_LIBRARY
