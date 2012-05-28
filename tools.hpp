#ifndef HPP_TOOLS_QUEST
#	define HPP_TOOLS_QUEST


/// XXX Remove when STL will provide it
namespace std
{
	template<typename T, typename ...Args>
	std::unique_ptr<T> make_unique( Args&& ...args )
	{
		return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
	}
}

#endif // HPP_TOOLS_QUEST
