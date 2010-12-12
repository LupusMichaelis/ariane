
#ifndef HPP_VIDEOMODE_SDL_QUEST
#	define HPP_VIDEOMODE_SDL_QUEST

class Size ;
class VideoMode ;

VideoMode const create_videomode(int width, int height, int depth) ;
Size const create_size(int width, int height) ;

class Size
{
	public:
		Size(int width, int height)
			: m_width(width)
			, m_height(height)
		{ }

		int width() const { return m_width ; }
		int height() const { return m_height ; }

	private:
		int m_width, m_height ;
} ;

class VideoMode
{
	public:
		VideoMode(Size size, int depth)
			: m_size(size)
			, m_depth(depth)
		{ }

		int width() const { return m_size.width() ; }
		int height() const { return m_size.height() ; }
		int depth() const { return m_depth ; }

	private:
		Size m_size ;
		int m_depth ;
} ;

inline Size const create_size(int width, int height)
{
	return Size(width, height) ;
}

inline VideoMode const create_videomode(int width, int height, int depth)
{
	return VideoMode(create_size(width, height), depth) ;
}

#endif // define HPP_VIDEOMODE_SDL_QUEST
