
#ifndef HPP_VIDEOMODE_SDL_QUEST
#	define HPP_VIDEOMODE_SDL_QUEST

class Size ;
class VideoMode ;

VideoMode const create_videomode(int width, int height, int depth) ;
Size const create_size(int width, int height) ;

class Size
{
	public:
		Size()
			: m_width(0)
			, m_height(0)
		{ }

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

		int const width() const { return m_width ; }
		int const height() const { return m_height ; }

		Size const operator*(unsigned const rhs) const ;
		Size const operator+(Size const & rhs) const ;
		Size const operator-(Size const & rhs) const ;
		Size & operator+= (Size const & rhs) ;
		Size & operator-= (Size const & rhs) ;

	private:
		int m_width, m_height ;
} ;

bool operator== (Size const & lhs, Size const & rhs) ;
bool operator< (Size const & lhs, Size const & rhs) ;
Size const operator*(unsigned const lhs, Size const & rhs) ;

class VideoMode
{
	public:
		VideoMode(Size size, int depth)
			: m_size(size)
			, m_depth(depth)
		{ }

		int const width() const { return m_size.width() ; }
		int const height() const { return m_size.height() ; }
		int const depth() const { return m_depth ; }
		Size const & size() const { return m_size ; }

		void size(Size const & new_size) { m_size = new_size ; }

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
