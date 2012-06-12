
#include "videomode.hpp"

bool operator== (Size const & lhs, Size const & rhs)
{
	return lhs.width() == rhs.width()
		&& lhs.height() == rhs.height() ;
}
