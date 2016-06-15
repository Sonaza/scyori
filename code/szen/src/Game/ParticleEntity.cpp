#include <szen/Game/ParticleEntity.hpp>

#include <szen/Game/Components/Transform.hpp>
#include <szen/Game/Components/ParticleComponent.hpp>

using namespace sz;

////////////////////////////////////////////////////
ParticleEntity::ParticleEntity() :
	m_component(NULL)
{
	//attach<Transform>();
	//m_component = attach<ParticleComponent>();
}

////////////////////////////////////////////////////
ParticleEntity::ParticleEntity(const std::string &asset)
{
	attach<Transform>();
	m_component = attach<ParticleComponent>(asset);
}

////////////////////////////////////////////////////
ParticleEntity::~ParticleEntity()
{
	
}

////////////////////////////////////////////////////
void ParticleEntity::update()
{
	if(!m_component) m_component = getComponent<ParticleComponent>();
}

////////////////////////////////////////////////////
void ParticleEntity::emit(const size_t amount)
{
	assert(m_component);

	m_component->emit(amount);
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setPosition(float x, float y)
{
	assert(m_component);

	call(&Transform::setPosition, x, y);
	m_component->updatePosition();
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setPosition(sf::Vector2f pos)
{
	assert(m_component);

	call(&Transform::setPosition, pos);
	m_component->updatePosition();
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setEmissionRate(float rate)
{
	assert(m_component);

	m_component->setEmissionRate(rate);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setLifetime(float time)
{
	assert(m_component);

	m_component->setLifetime(time);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setLifetime(float start, float end)
{
	assert(m_component);

	m_component->setLifetime(start, end);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setVelocity(sf::Vector2f vel)
{
	assert(m_component);

	m_component->setVelocity(vel);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setVelocityCone(float force, float direction, float variance)
{
	assert(m_component);

	m_component->setVelocityCone(force, direction, variance);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setRotation(float rot)
{
	assert(m_component);

	m_component->setRotation(rot);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setRotation(float start, float end)
{
	assert(m_component);

	m_component->setRotation(start, end);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setRotationSpeed(float speed)
{
	assert(m_component);

	m_component->setRotationSpeed(speed);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setRotationSpeed(float start, float end)
{
	assert(m_component);

	m_component->setRotationSpeed(start, end);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setScale(float scale)
{
	assert(m_component);

	m_component->setScale(scale);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setScale(float start, float end)
{
	assert(m_component);

	m_component->setScale(start, end);
	return this;
}

////////////////////////////////////////////////////
ParticleEntity* ParticleEntity::setColor(const sf::Color& color)
{
	assert(m_component);
	
	m_component->setColor(color);
	return this;
}

////////////////////////////////////////////////////
sf::Color ParticleEntity::getColor()
{
	assert(m_component);

	return m_component->getColor();
}