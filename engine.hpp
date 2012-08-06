#ifndef HPP_ENGINE_QUEST
#	define HPP_ENGINE_QUEST

#	include "tools.hpp"

#	include <string>

class EventLoop ;
class Gui ;
class KeyBoard ;
class Interface ;

class Engine
{
	public:
		Engine() ;
		virtual ~Engine() ;

		virtual
		void first_interface() = 0 ;

		void run() ;
		void game_over() ;

		Gui & gui() ;
		Gui const & gui() const ;
		KeyBoard const & keyboard()	const ;

		template <typename InterfaceType, typename EngineType = Engine>
		void set_interface()
		{
			std::unique_ptr<InterfaceType> p_interface 
				= std::make_unique<InterfaceType>(static_cast<EngineType &>(*this)) ;
			set_interface(std::move(p_interface)) ;
		}

	private:
		void heart_beat(EventLoop &) ;
		void set_interface(std::unique_ptr<Interface> p_interface) ;

	private:
		class Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class Engine */ ;


#endif // HPP_ENGINE_QUEST
