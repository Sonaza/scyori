#ifndef SZEN_PARTICLEENTITY_HPP
#define SZEN_PARTICLEENTITY_HPP

#include <szen/Game/Entity.hpp>

namespace sz
{
	
	class ParticleComponent;

	class ParticleEntity : public Entity
	{
	public:
		ParticleEntity();
		ParticleEntity(const std::string &asset);
		virtual ~ParticleEntity();

		void			update();

		void			emit(const size_t amount);

		ParticleEntity* setPosition(float x, float y);
		ParticleEntity* setPosition(sf::Vector2f pos);
		
		ParticleEntity* setEmissionRate(float rate);

		ParticleEntity* setLifetime(float time);
		ParticleEntity* setLifetime(float start, float end);

		ParticleEntity* setVelocity(sf::Vector2f vel);
		ParticleEntity* setVelocityCone(float force, float direction_radians, float variance);

		ParticleEntity* setRotation(float rot);
		ParticleEntity* setRotation(float start, float end);

		ParticleEntity* setRotationSpeed(float speed);
		ParticleEntity* setRotationSpeed(float start, float end);

		ParticleEntity* setScale(float scale);
		ParticleEntity* setScale(float start, float end);

		ParticleEntity* setColor(const sf::Color&);
		sf::Color		getColor();

	private:

		ParticleComponent*	m_component;

	};
	
}

#endif // SZEN_PARTICLEENTITY_HPP
