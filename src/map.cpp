#include "map.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>

struct Map::Impl
{
	std::vector<Map::Element> m_elements;
	VideoMode m_dimensions;
} /* struct Map::Impl */;

Map::Map()
	: mp_impl {std::make_unique<Map::Impl>()}
{
}

Map::~Map()
{
}

std::vector<Map::Element> Map::elements() const
{
	return mp_impl->m_elements;
}

VideoMode Map::dimensions() const
{
	return mp_impl->m_dimensions;
}


inline std::string::const_iterator search_string(Map::pair_string_const_it haystack, std::string needle)
{
	return std::search(haystack.first, haystack.second
					, std::begin(needle), std::end(needle));
}

inline bool search_doublet(Map::pair_string_const_it haystack
		, Map::pair_string_const_it & first
		, Map::pair_string_const_it & second
		)
{
	auto open = std::find(haystack.first, haystack.second, '(');
	if(open == haystack.second)
		return false;

	auto close = std::find(open, haystack.second, ')');
	if(close == haystack.second)
		return false;

	auto first_commas = std::find(open, close, ',');
	if(first_commas == haystack.second)
		return false;

	first.first = open + 1;
	first.second = first_commas;

	second.first = first_commas + 1;
	second.second = close;

	return true;
}

inline bool search_triplet(Map::pair_string_const_it haystack
		, Map::pair_string_const_it & first
		, Map::pair_string_const_it & second
		, Map::pair_string_const_it & third
		)
{
	auto open = std::find(haystack.first, haystack.second, '(');
	if(open == haystack.second)
		return false;

	auto close = std::find(open, haystack.second, ')');
	if(close == haystack.second)
		return false;

	auto first_commas = std::find(open, close, ',');
	if(first_commas == haystack.second)
		return false;

	auto second_commas = std::find(first_commas + 1, close, ',');
	if(second_commas == haystack.second)
		return false;

	first.first = open + 1;
	first.second = first_commas;

	second.first = first_commas + 1;
	second.second = second_commas;

	third.first = second_commas + 1;
	third.second = close;

	return true;
}

#include <map>

#include <boost/format.hpp>

void Map::load(std::istream & in)
{
	static std::map<std::string const, loader_callback> const
		map_callbacks =
			{ { std::string("size"),		&Map::load_size }
			, { std::string("wall"),		&Map::load_wall }
			, { std::string("house"),		&Map::load_house }
			, { std::string("well"),		&Map::load_well }
			, { std::string("townhall"),	&Map::load_townhall }
			, { std::string("inn"),			&Map::load_inn }
			};

	std::string buffer;
	bool found = false;

	while(std::getline(in, buffer))
	{
		auto buffer_pair_it = std::make_pair(std::begin(buffer), std::end(buffer));
		for(auto loader: map_callbacks)
		{
			auto it = search_string(buffer_pair_it, loader.first);
			if(std::begin(buffer) == it)
			{
				(this->*loader.second)(buffer_pair_it);
				found = true;
				break;
			}
		}

		if(!found)
			throw (boost::format("String '%s' doesn't contain a supported keyword") % buffer).str();
	}
}

#include <cstdlib>

void Map::load_size(pair_string_const_it specification)
{
	std::string word("size");

	std::advance(specification.first, word.size());

	// XXX check we're not out of string

	pair_string_const_it first;
	pair_string_const_it second;
	pair_string_const_it third;

	if(!search_triplet(specification, first, second, third))
		throw "Triplet not found";

	std::string width { first.first, first.second };
	std::string height { second.first, second.second };
	std::string depth { third.first, third.second };

	VideoMode dim {Size {std::atoi(width.c_str())
		, std::atoi(height.c_str()) }
		, std::atoi(depth.c_str())
	};

	mp_impl->m_dimensions = dim;
}

void Map::load_wall(pair_string_const_it specification)
{
	std::string word("wall");

	std::advance(specification.first, word.size());

	// XXX check we're not out of string

	pair_string_const_it first;
	pair_string_const_it second;

	Size from, to;

	if(!search_doublet(specification, first, second))
		throw "Doublet not found";

	std::string width { first.first, first.second };
	std::string height { second.first, second.second };
	specification.first = second.second;

	to = Size {std::atoi(width.c_str()), std::atoi(height.c_str()) };

	bool discard_first = false;
	do
	{
		if(!search_doublet(specification, first, second))
			break;

		std::string width { first.first, first.second };
		std::string height { second.first, second.second };
		specification.first = second.second;

		from = to;
		to = Size {std::atoi(width.c_str()), std::atoi(height.c_str()) };

		wall_loader_callback loader = nullptr;

		if(from.width() == to.width())
		{
			if(discard_first)
				from.height(from.height() + (from.height() < to.height() ? 1 : -1));

			loader = &Map::load_wall_vertical;
		}
		else if(from.height() == to.height())
		{
			if(discard_first)
				from.width(from.width() + (from.width() < to.width() ? 1 : -1));

			loader = &Map::load_wall_horizontal;
		}
		else
			throw "Non-vertical and non-horizontal walls unsupported";

		(this->*loader)("wall", from, to);

		discard_first = true;

	} while(true);
}

void Map::load_wall_horizontal(std::string word, Size from, Size to)
{
		Element element {word, from};

		do
		{
			mp_impl->m_elements.push_back(element);
			from.width(from.width() + (from.width() < to.width() ? 1 : -1));
			element.m_position = from;
		}
		while(from.width() != to.width());

		mp_impl->m_elements.push_back(element);
}

void Map::load_wall_vertical(std::string word, Size from, Size to)
{
		Element element {word, from};

		do
		{
			mp_impl->m_elements.push_back(element);
			from.height(from.height() + (from.height() < to.height() ? 1 : -1));
			element.m_position = from;
		}
		while(from.height() != to.height());

		mp_impl->m_elements.push_back(element);
}

inline void search_word_doublet(Map::pair_string_const_it haystack, Size & doublet)
{
	std::pair<std::string::const_iterator, std::string::const_iterator> first;
	std::pair<std::string::const_iterator, std::string::const_iterator> second;

	if(!search_doublet(haystack, first, second))
		throw "Doublet not found";

	std::string width { first.first, first.second };
	std::string height { second.first, second.second };
	haystack.first = second.second;

	doublet = Size {std::atoi(width.c_str()), std::atoi(height.c_str()) };
}

void Map::load_element_word_doublet(std::string word, pair_string_const_it specification)
{
	Element element {word, Size{}};
	std::advance(specification.first, word.size());
	// XXX check we're not out of string
	search_word_doublet(specification, element.m_position);
	mp_impl->m_elements.push_back(element);
}

void Map::load_well(pair_string_const_it specification)
{
	load_element_word_doublet("well", specification);
}

void Map::load_house(pair_string_const_it specification)
{
	load_element_word_doublet("house", specification);
}

void Map::load_townhall(pair_string_const_it specification)
{
	load_element_word_doublet("townhall", specification);
}

void Map::load_inn(pair_string_const_it specification)
{
	load_element_word_doublet("inn", specification);
}
