#ifndef HPP_ADVENTURE_QUEST
#	define HPP_ADVENTURE_QUEST

#include "quest_interface.hpp"
#include "box.hpp"
#include "image_box.hpp"
#include "map.hpp"

class Canvas;
class AdventureModel;
class MotifLibrary;

class AdventureInterface
	: public QuestInterface
{
	public:
		explicit AdventureInterface(QuestEngine & engine);
		virtual ~AdventureInterface();

	private:
		void move_left();
		void move_right();
		void move_up();
		void move_down();

		virtual
		void move(EventLoop &, KeyEvent const & ke);
		virtual
		void move(EventLoop &, MouseEvent const & me);
		virtual
		void move(EventLoop &, MouseButtonEvent const & me);

		virtual
		void display();

	protected:
		Canvas & adventure_view();
		AdventureModel & adventure_model();
		AdventureModel const & adventure_model() const;
		MotifLibrary & motif_library();

	private:
		std::unique_ptr<AdventureModel>			mp_adventure_model;
		std::unique_ptr<MotifLibrary>			mp_motif_library;
		std::shared_ptr<ImageBox>				mp_hodor;

} /* class AdventureInterface */;

////////////////////////////////////////////////////////////////////////////////
class ImageBox;
class AdventureModel
{
	public:
		AdventureModel();
		~AdventureModel();

		Size const & position() const;
		Map const & map() const;

	private:
		struct Impl;
		std::unique_ptr<Impl>	mp_impl;

} /* class AdventureModel */;

#endif // HPP_ADVENTURE_QUEST
