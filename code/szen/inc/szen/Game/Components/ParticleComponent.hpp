#ifndef SZEN_PARTICLECOMPONENT_HPP
#define SZEN_PARTICLECOMPONENT_HPP

#include <string>
#include <vector>

#include <memory>

#include <SFML/Graphics.hpp>

#include <szen/Game/Component.hpp>

#include <Thor/Animation.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include <Thor/Graphics.hpp>

namespace sz
{
	
	typedef std::unique_ptr<thor::ParticleSystem> ParticleSystemPtr;
	typedef thor::UniversalEmitter::Ptr ParticleEmitterPtr;
	
	class ShaderAsset;

	class ParticleComponent : public Component
	{
		friend class ParticleEntity;

	public:
		ParticleComponent();
		ParticleComponent(const std::string &asset);
		virtual ~ParticleComponent();

		void attached();
		void update();
		void parsePrefab(json::Value&);

		void createEmitter();

		void setShader(const std::string &assetID);
		void setBlendMode(sf::BlendMode);

		void draw(sf::RenderTarget& target);

		void emit(const size_t amount);

	//protected:
		
		void prewarm();
		
		void setColor(const sf::Color&);
		sf::Color getColor();

		void setEmissionRate(float rate);

		void setLifetime(float time);
		void setLifetime(float start, float end);

		void setVelocity(sf::Vector2f vel);
		void setVelocityCone(float force, float direction_radians, float variance);

		void setRotation(float rot);
		void setRotation(float start, float end);

		void setRotationSpeed(float speed);
		void setRotationSpeed(float start, float end);

		void setScale(float scale);
		void setScale(float start, float end);

		// Fade in at "in * lifetime" seconds
		// Fade out at "out * lifetime" seconds
		void addFadeAffector(float in, float out);
		void addScaleAffector(float x, float y);
		void addForceAffector(float x, float y);

		void updatePosition();

	private:

		sf::Color				m_color;

		ShaderAsset*			m_shaderAsset;

		sf::Time				m_lifetime;

		sf::RenderStates		m_renderStates;

		ParticleSystemPtr		m_particleSystem;
		ParticleEmitterPtr		m_emitter;

		std::shared_ptr<sf::Texture>	m_particleTexture;

		struct Affectors
		{
			thor::Affector::Ptr		fade;
			thor::Affector::Ptr		force;
			thor::Affector::Ptr		scale;
		} m_affectors;
		
		PausableClock		m_particleClock;

	};
	
}

#endif // SZEN_PARTICLECOMPONENT_HPP
