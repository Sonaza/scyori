#ifndef SZEN_COMPONENTTYPE_HPP
#define SZEN_COMPONENTTYPE_HPP

#include <typeinfo>
#include <type_traits>
#include <map>
#include <cassert>

namespace sz
{

	struct type_info_comparator
	{
		bool operator()(const std::type_info* a, const std::type_info* b) const
		{
			return a->before(*b) == 1;
		}
	};
	
	class ComponentType;

	typedef std::map<const std::type_info*, ComponentType*,
					 type_info_comparator> ComponentTypeList;

	class ComponentType
	{
	public:

		static ComponentType* getTypeFor(const std::type_info &typeinfo);

		template <class T>
		static ComponentType* getTypeFor()
		{
			assert((std::is_base_of<Component, T>::value) == true);
			return getTypeFor(typeid(T));
		}

		static void releaseComponentTypes();

		int	m_id;

	private:
		ComponentType();

		static int nextId;
		static ComponentTypeList componentTypes;
	};
	
}

#endif // SZEN_COMPONENTTYPE_HPP
