
#ifndef HPP_VIDEOMODE_SDL_QUEST
#	define HPP_VIDEOMODE_SDL_QUEST


class VideoMode
{
	public:
		VideoMode(int width, int height, int depth)
			: m_width(width), m_height(height), m_depth(depth)
		{ }

		int width() const { return m_width ; }
		int height() const { return m_height ; }
		int depth() const { return m_depth ; }

	private:
		int m_width, m_height, m_depth ;
} ;

#endif // define HPP_VIDEOMODE_SDL_QUEST
