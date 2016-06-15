#include <szen/Game/Component.hpp>
#include <szen/Game/Entity.hpp>

using namespace sz;

////////////////////////////////////////////////////
Component::Component() :
	m_entity(NULL),
	m_componentOrder(0)
{
	
}

////////////////////////////////////////////////////
Component::~Component()
{
	
}
