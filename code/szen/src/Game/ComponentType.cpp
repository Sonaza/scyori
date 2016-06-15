#include <szen/Game/ComponentType.hpp>

using namespace sz;

////////////////////////////////////////////////////

int ComponentType::nextId = 1;
ComponentTypeList ComponentType::componentTypes;

////////////////////////////////////////////////////
ComponentType::ComponentType()
{
	m_id = nextId;
	nextId += 1;
}

////////////////////////////////////////////////////
ComponentType* ComponentType::getTypeFor(const std::type_info &t)
{
	ComponentType* type = componentTypes[&t];

	if(!type)
	{
		type = new ComponentType;
		componentTypes[&t] = type;
	}

	return type;
}

////////////////////////////////////////////////////
void ComponentType::releaseComponentTypes()
{
	for(ComponentTypeList::iterator it = componentTypes.begin();
		it != componentTypes.end(); delete it->second, ++it);

	componentTypes.clear();
}
