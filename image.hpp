#ifndef HPP_IMAGE_SDL_QUEST
#	define HPP_IMAGE_SDL_QUEST

#	include "surface.hpp"

class Image
	: public Surface
{
	public:
		virtual
		~Image() ;

		static
		void create(std::shared_ptr<Image> & p_surface, std::string const & filename)
		{
			std::shared_ptr<Image> p_new_surface(new Image(filename)) ;
			p_new_surface->init() ;
			std::swap(p_surface, p_new_surface) ;
		}

	private:
		Image() ;
		explicit Image(std::string const & filename) ;

		void init() ;

		virtual
		void release() throw() ;

		std::string m_filename ;
} ;

#endif // define HPP_IMAGE_SDL_QUEST
