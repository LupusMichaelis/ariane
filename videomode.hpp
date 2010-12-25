
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

		Size(Size const & copied)
			: m_width(copied.m_width)
			, m_height(copied.m_height)
		{ }

		void translate(int distance, int angle = 0)
		{
			switch(angle)
			{
				case 0:
					m_width += distance ;
					break ;
				default:
					throw "todo" ;
			}
		}

		void width(int new_width) { m_width = new_width ; }
		void height(int new_height) { m_height = new_height ; }

		int width() const { return m_width ; }
		int height() const { return m_height ; }

		Size const operator-(Size const & rhs) const
		{
			Size new_value(*this) ;
			new_value.m_width -= rhs.width() ;
			new_value.m_height -= rhs.height() ;
			return new_value ;
		}

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
{ return Size(width, height) ; }

inline VideoMode const create_videomode(Size const & size, int depth)
{ return VideoMode(size, depth) ; }

inline VideoMode const create_videomode(int width, int height, int depth)
{ return create_videomode(create_size(width, height), depth) ; }

template<typename T>
int width(T const & thing)
{ return thing.videomode().width() ; }

template<typename T>
int height(T const & thing)
{ return thing.videomode().height() ; }

template<typename T>
int depth(T const & thing)
{ return thing.videomode().depth() ; }

#endif // define HPP_VIDEOMODE_SDL_QUEST
