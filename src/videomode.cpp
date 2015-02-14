
#include "videomode.hpp"

bool operator== (Size const & lhs, Size const & rhs)
{
	return lhs.width() == rhs.width()
		&& lhs.height() == rhs.height();
}

bool operator< (Size const & lhs, Size const & rhs)
{
	return lhs.width() < rhs.width()
		&& lhs.height() < rhs.height();
}

Size const operator*(unsigned const lhs, Size const & rhs)
{
	return rhs * lhs;
}

Size const Size::operator*(unsigned const rhs) const
{
	Size new_value(*this);
	new_value.m_width *= rhs;
	new_value.m_height *= rhs;
	return new_value;
}

Size const Size::operator+(Size const & rhs) const
{
	Size new_value(*this);
	new_value.m_width += rhs.width();
	new_value.m_height += rhs.height();
	return new_value;
}

Size const Size::operator-(Size const & rhs) const
{
	Size new_value(*this);
	new_value.m_width -= rhs.width();
	new_value.m_height -= rhs.height();
	return new_value;
}

Size & Size::operator+= (Size const & rhs)
{
	m_width += rhs.width();
	m_height += rhs.height();
	return *this;
}

Size & Size::operator-= (Size const & rhs)
{
	m_width -= rhs.width();
	m_height -= rhs.height();
	return *this;
}

