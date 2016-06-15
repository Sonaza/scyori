#include <game/Entities/Effects/EnergyBeam.hpp>

////////////////////////////////////////////////////
EnergyBeam::EnergyBeam(sz::Entity* pivot, sz::Entity* target) :
	m_pivot		(pivot),
	m_target	(target)
{
	loadFromPrefab("data/effects/energy_beam.szt");

	m_pivotTransform	= pivot->getComponent<sz::Transform>();
	m_targetTransform	= target->getComponent<sz::Transform>();

	m_transform			= getComponent<sz::Transform>();
}

////////////////////////////////////////////////////
EnergyBeam::~EnergyBeam()
{
	
}

////////////////////////////////////////////////////
void EnergyBeam::onLoad()
{
	
}

////////////////////////////////////////////////////
void EnergyBeam::setColor(sf::Color& color)
{
	getComponent<sz::Renderer>()->setColor(color);
}

////////////////////////////////////////////////////
void EnergyBeam::update()
{
	if(!sz::World::exists(m_pivot) || !sz::World::exists(m_target))
	{
		destroyEntity();
		return;
	}

	sf::Vector2f pivotPosition	= m_pivotTransform->getPosition();
	sf::Vector2f targetPosition = m_targetTransform->getPosition();

	//pivotPosition.y *= 2.f;

	float dist = sz::distancesqrt(pivotPosition, targetPosition);

	float angle = sz::toDegrees(sz::getAngle(
		pivotPosition, targetPosition
	));

	pivotPosition.y *= -1.f;
	m_transform->setPosition(pivotPosition);
	m_transform->setRotation(angle);
	//m_transform->setScale(dist / 500.f, 1.f);
}