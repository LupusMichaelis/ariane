#ifndef HPP_MAP_QUEST
#	define HPP_MAP_QUEST

#	include "memory.hpp"
#	include "videomode.hpp"

#	include <fstream>
#	include <vector>

class Map
{
	public:
		typedef std::pair<std::string::const_iterator, std::string::const_iterator> pair_string_const_it;
		struct Element;

		Map();
		void load(std::istream & in);

		std::vector<Element> elements() const;
		VideoMode dimensions() const;

		~Map();

	private:
		typedef void (Map::* loader_callback)(pair_string_const_it specification);
		typedef void (Map::* wall_loader_callback)(std::string word, Size from, Size to);
		// void (Map::*) (Map::pair_string_const_it specification)

		void load_wall(pair_string_const_it specification);

		void load_wall_horizontal(std::string word, Size from, Size to);
		void load_wall_vertical(std::string word, Size from, Size to);

		void load_well(pair_string_const_it specification);
		void load_size(pair_string_const_it specification);
		void load_house(pair_string_const_it specification);
		void load_townhall(pair_string_const_it specification);
		void load_inn(pair_string_const_it specification);
		void load_door(pair_string_const_it specification);

		void load_element_word_doublet(std::string word, pair_string_const_it specification);

	private:
		struct Impl;
		std::unique_ptr<Impl>			mp_impl;

} /* class Map */;

struct Map::Element
{
	Element(std::string const name, Size const position)
		: m_name(name), m_position(position)
	{ }

	std::string m_name;
	Size m_position;
} /* class Map::Element */;

#endif // HPP_MAP_QUEST
