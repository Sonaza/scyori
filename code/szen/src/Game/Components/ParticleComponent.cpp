#include <szen/Game/Components/ParticleComponent.hpp>
#include <szen/Game/Components/Transform.hpp>

#include <szen/System/AssetManager.hpp>
#include <szen/System/Assets/TextureAsset.hpp>

#include <szen/System/SpriteBatch.hpp>

#include <szen/System/Assets/ShaderAsset.hpp>

#include <szen/Game/Camera.hpp>

using namespace sz;

////////////////////////////////////////////////////
ParticleComponent::ParticleComponent() :
	m_shaderAsset(NULL)
{
	m_componentOrder = 40;
}

////////////////////////////////////////////////////
ParticleComponent::ParticleComponent(const std::string &assetid) :
	m_shaderAsset(NULL)
{
	m_componentOrder = 40;

	TextureAsset* asset = Asset::getTexture(assetid);
	m_particleSystem.reset(new thor::ParticleSystem(asset->getAsset()));

	/*m_emitter = thor::UniversalEmitter::create();

	m_emitter->setEmissionRate(10.f);
	m_emitter->setParticleLifetime(sf::seconds(3));
	m_emitter->setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
	m_emitter->setParticleVelocity(thor::Distributions::circle(sf::Vector2f(), 350.f));
	
	m_particleSystem->addEmitter(m_emitter);*/

	createEmitter();

	//m_particleSystem->addAffector(thor::AnimationAffector::create(thor::FadeAnimation(0.05f, 0.8f)));
	//m_particleSystem->addAffector(thor::ScaleAffector::create(sf::Vector2f(-0.2f, 1.f)));
}

////////////////////////////////////////////////////
ParticleComponent::~ParticleComponent()
{
	
}

////////////////////////////////////////////////////
void ParticleComponent::updatePosition()
{
	sf::Vector2f pos	= m_entity->call(&Transform::getPosition);
	if(!m_entity->hasParent())
		pos.y *= -1.f;

	m_emitter->setParticlePosition(pos);
}

////////////////////////////////////////////////////
void ParticleComponent::update()
{
	if(m_particleSystem)
	{
		updatePosition();

		m_particleSystem->update(m_particleClock.restart());
	}
}

////////////////////////////////////////////////////
void ParticleComponent::createEmitter()
{
	m_emitter = thor::UniversalEmitter::create();
	m_particleSystem->addEmitter(m_emitter);

	/*setEmissionRate(10.f);
	setLifetime(5.f);
	setRotation(-180.f, 180.f);*/
	m_emitter->setEmissionRate(10.f);
	m_emitter->setParticleLifetime(sf::seconds(3));
	m_emitter->setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
	m_emitter->setParticleVelocity(thor::Distributions::circle(sf::Vector2f(), 350.f));

	//addFadeAffector(0.01f, 0.9f);
}

////////////////////////////////////////////////////
void ParticleComponent::emit(const size_t amount)
{
	//setColor(sf::Color(255, 220, 50));

	m_particleSystem->emit(amount);
	//m_emitter->emit(m_particleSystem.get(), amount);
}

////////////////////////////////////////////////////
void ParticleComponent::parsePrefab(json::Value& val)
{
	if(val.isMember("texture"))
	{
		const std::string assetid = val["texture"].asString();
		TextureAsset* asset = Asset::getTexture(assetid);

		if(asset)
		{
			//m_particleTexture.reset(new sf::Texture(*asset->getAsset()));

			m_particleSystem.reset(new thor::ParticleSystem(asset->getAsset()));

			createEmitter();
		}
		else
		{
			szerr << "Prefab particle texture '" << assetid << "' could not be retrieved." << ErrorStream::error;
		}
	}

	if(val.isMember("emissionrate"))
	{
		setEmissionRate(static_cast<float>(val["emissionrate"].asDouble()));
	}
	
	if(val.isMember("lifetime"))
	{
		if(val["lifetime"].isArray() && val["lifetime"].size() == 2)
		{
			setLifetime(
				static_cast<float>(val["lifetime"][0U].asDouble()),
				static_cast<float>(val["lifetime"][1U].asDouble())
			);
		}
		else if(val["lifetime"].isArray() && val["lifetime"].size() == 1)
		{
			setLifetime(
				static_cast<float>(val["lifetime"][0U].asDouble())
			);
		}
		else if(!val["lifetime"].isArray())
		{
			setLifetime(static_cast<float>(val["lifetime"].asDouble()));
		}
		else
		{
			szerr << "Invalid particle lifetime value in prefab." << ErrorStream::error;
		}
	}

	if(val.isMember("velocity"))
	{
		if(val["velocity"].isArray() && val["velocity"].size() == 2)
		{
			setVelocity(sf::Vector2f(
				static_cast<float>(val["velocity"][0U].asDouble()),
				static_cast<float>(val["velocity"][1U].asDouble())
			));
		}
	}

	if(val.isMember("rotation"))
	{
		if(val["rotation"].isArray() && val["rotation"].size() == 2)
		{
			setRotation(
				static_cast<float>(val["rotation"][0U].asDouble()),
				static_cast<float>(val["rotation"][1U].asDouble())
			);
		}
		else if(val["rotation"].isArray() && val["rotation"].size() == 1)
		{
			setRotation(
				static_cast<float>(val["rotation"][0U].asDouble())
			);
		}
		else if(!val["rotation"].isArray())
		{
			setRotation(static_cast<float>(val["rotation"].asDouble()));
		}
		else
		{
			szerr << "Invalid particle rotation value in prefab." << ErrorStream::error;
		}
	}

	if(val.isMember("scale"))
	{
		if(val["scale"].isArray() && val["scale"].size() == 2)
		{
			setScale(
				static_cast<float>(val["scale"][0U].asDouble()),
				static_cast<float>(val["scale"][1U].asDouble())
			);
		}
		else if(val["scale"].isArray() && val["scale"].size() == 1)
		{
			setScale(
				static_cast<float>(val["scale"][0U].asDouble())
			);
		}
		else if(!val["scale"].isArray())
		{
			setScale(static_cast<float>(val["scale"].asDouble()));
		}
		else
		{
			szerr << "Invalid particle scale value in prefab." << ErrorStream::error;
		}
	}

	if(val.isMember("rotationspeed"))
	{
		if(val["rotationspeed"].isArray() && val["rotationspeed"].size() == 2)
		{
			setRotationSpeed(
				static_cast<float>(val["rotationspeed"][0U].asDouble()),
				static_cast<float>(val["rotationspeed"][1U].asDouble())
			);
		}
		else if(val["rotationspeed"].isArray() && val["rotationspeed"].size() == 1)
		{
			setRotationSpeed(
				static_cast<float>(val["rotationspeed"][0U].asDouble())
			);
		}
		else if(!val["rotationspeed"].isArray())
		{
			setRotationSpeed(static_cast<float>(val["rotationspeed"].asDouble()));
		}
		else
		{
			szerr << "Invalid particle rotationspeed value in prefab." << ErrorStream::error;
		}
	}

	if(val.isMember("affectors"))
	{
		json::Value affectors = val["affectors"];

		for(json::Value::iterator it = affectors.begin(); it != affectors.end(); ++it)
		{
			const std::string affector = it.memberName();

			if(affector == "fade")
			{
				addFadeAffector(
					static_cast<float>((*it)[0U].asDouble()),
					static_cast<float>((*it)[1U].asDouble())
				);
			}
			else if(affector == "scale")
			{
				addScaleAffector(
					static_cast<float>((*it)[0U].asDouble()),
					static_cast<float>((*it)[1U].asDouble())
				);
			}
			else if(affector == "force")
			{
				addForceAffector(
					static_cast<float>((*it)[0U].asDouble()),
					static_cast<float>((*it)[1U].asDouble())
				);
			}
		}
	}

	if(val.get("prewarm", 0).asBool())
	{
		prewarm();
	}

	if(val.isMember("shader"))
	{
		json::Value shader = val["shader"];

		if(shader.isConvertibleTo(json::stringValue))
		{
			setShader(shader.asString());
		}
		else if(shader.isObject())
		{
			setShader(shader["asset"].asString());

			if(shader.isMember("param") && !shader["param"].empty())
			{
				json::Value parameters = shader["param"];

				for(json::Value::iterator it = parameters.begin(); it != parameters.end(); ++it)
				{
					const std::string name = it.memberName();

					json::Value v = *it;
					if(v.isArray())
					{
						switch(v.size())
						{
						case 1:
							m_shaderAsset->getAsset()->setParameter(name,
								(float)v[0U].asDouble());
							break;

						case 2:
							m_shaderAsset->getAsset()->setParameter(name,
								(float)v[0U].asDouble(),
								(float)v[1U].asDouble());
							break;

						case 3:
							m_shaderAsset->getAsset()->setParameter(name,
								(float)v[0U].asDouble(),
								(float)v[1U].asDouble(),
								(float)v[2U].asDouble());
							break;

						case 4:
							{
								m_shaderAsset->getAsset()->setParameter(name,
									(float)v[0U].asDouble(),
									(float)v[1U].asDouble(),
									(float)v[2U].asDouble(),
									(float)v[3U].asDouble());
								break;
							}

						default:
							szerr << "Incorrect amount of parameter arguments in prefab." << ErrorStream::error;
							break;
						}
					}
					else if(v.isConvertibleTo(json::realValue))
					{
						m_shaderAsset->getAsset()->setParameter(name,
							(float)v.asDouble());
					}
				}
			}
		}
	}

	if(val.isMember("color"))
	{
		sf::Uint32 size = val["color"].size();
		if(size == 3 || size == 4)
		{
			sf::Color color;

			color.r = static_cast<sf::Uint8>(val["color"][0U].asUInt());
			color.g = static_cast<sf::Uint8>(val["color"][1U].asUInt());
			color.b = static_cast<sf::Uint8>(val["color"][2U].asUInt());

			if(size == 4)
			{
				color.a = static_cast<sf::Uint8>(val["color"][3U].asUInt());
			}

			setColor(color);
		}
	}
	
	const std::string blend = val.get("blendmode", "alpha").asString();

	if(blend == "alpha")
	{
		m_renderStates.blendMode = sf::BlendAlpha;
	}
	else if(blend == "additive")
	{
		m_renderStates.blendMode = sf::BlendAdd;
	}
	else if(blend == "multiply")
	{
		m_renderStates.blendMode = sf::BlendMultiply;
	}
	else if(blend == "none")
	{
		m_renderStates.blendMode = sf::BlendNone;
	}
}

////////////////////////////////////////////////////
void ParticleComponent::setBlendMode(sf::BlendMode mode)
{
	m_renderStates.blendMode = mode;
}

////////////////////////////////////////////////////
void ParticleComponent::setShader(const std::string &assetID)
{
	ShaderAsset* asset = Asset::getShader(assetID);

	if(asset)
	{
		m_shaderAsset = asset;
		//m_shader = asset->getAsset();
	}
}

////////////////////////////////////////////////////
void ParticleComponent::draw(sf::RenderTarget& target)
{
	//target.draw(*m_particleSystem, m_renderStates);
	if(!m_particleSystem) return;

	SpriteBatch::append(m_particleSystem.get(), m_entity->getLayer(), m_shaderAsset, m_renderStates.blendMode);
}

////////////////////////////////////////////////////
void ParticleComponent::attached()
{
	// Add a transform component if needed
	if(!m_entity->hasComponent<Transform>())
		m_entity->attach<Transform>();

	m_entity->addDrawCallback(&ParticleComponent::draw);

	if(m_emitter)
	{
		m_emitter->setParticlePosition(call(&Transform::getPosition));
	}
}

////////////////////////////////////////////////////
void ParticleComponent::setEmissionRate(float rate)
{
	m_emitter->setEmissionRate(rate);
}

////////////////////////////////////////////////////
void ParticleComponent::setLifetime(float time)
{
	m_lifetime = sf::seconds(time);
	m_emitter->setParticleLifetime(m_lifetime);
}

////////////////////////////////////////////////////
void ParticleComponent::prewarm()
{
	sf::Time delta = sf::milliseconds(16);
	int loops = m_lifetime.asMilliseconds() / delta.asMilliseconds();

	// Prewarming particle system by updating particle system as many times
	// as it would be normally during one particle's lifetime and assuming 60 FPS delta
	for(int i=0; i < loops; ++i)
	{
		m_particleSystem->update(delta);
	}
}

////////////////////////////////////////////////////
void ParticleComponent::setColor(const sf::Color& color)
{
	m_color = color;
	m_emitter->setParticleColor(color);
}

////////////////////////////////////////////////////
sf::Color ParticleComponent::getColor()
{
	return m_color;
}

////////////////////////////////////////////////////
void ParticleComponent::setLifetime(float start, float end)
{
	m_emitter->setParticleLifetime(
		thor::Distributions::uniform(sf::seconds(start), sf::seconds(end))
	);
}

////////////////////////////////////////////////////
void ParticleComponent::setVelocity(sf::Vector2f vel)
{
	vel.y *= -1.f;
	m_emitter->setParticleVelocity(vel);
}

////////////////////////////////////////////////////
void ParticleComponent::setVelocityCone(float force, float direction, float variance)
{
	//float angle = thor::toRadian(direction);

	//direction += 1.570796326794897f;

	sf::Vector2f dir(
		cos(direction) * force, sin(direction) * force
	);

	m_emitter->setParticleVelocity(thor::Distributions::deflect(dir, variance));
}

////////////////////////////////////////////////////
void ParticleComponent::setRotation(float rot)
{
	m_emitter->setParticleRotation(rot);
}

////////////////////////////////////////////////////
void ParticleComponent::setRotation(float start, float end)
{
	m_emitter->setParticleRotation(thor::Distributions::uniform(start, end));
}

////////////////////////////////////////////////////
void ParticleComponent::setRotationSpeed(float speed)
{
	m_emitter->setParticleRotationSpeed(speed);
}

////////////////////////////////////////////////////
void ParticleComponent::setRotationSpeed(float start, float end)
{
	m_emitter->setParticleRotationSpeed(thor::Distributions::uniform(start, end));
}

////////////////////////////////////////////////////
void ParticleComponent::setScale(float scale)
{
	m_emitter->setParticleScale(sf::Vector2f(scale, scale));
}

////////////////////////////////////////////////////
void ParticleComponent::setScale(float start, float end)
{
	m_emitter->setParticleScale(
		thor::Distributions::uniformrect(start, end)
	);
}

////////////////////////////////////////////////////
void ParticleComponent::addFadeAffector(float in, float out)
{
	m_affectors.fade = thor::AnimationAffector::create(thor::FadeAnimation(in, out));
	m_particleSystem->addAffector(m_affectors.fade);
}

////////////////////////////////////////////////////
void ParticleComponent::addScaleAffector(float x, float y)
{
	m_affectors.scale = thor::ScaleAffector::create(sf::Vector2f(x, y));
	m_particleSystem->addAffector(m_affectors.scale);
}

////////////////////////////////////////////////////
void ParticleComponent::addForceAffector(float x, float y)
{
	m_affectors.force = thor::ForceAffector::create(sf::Vector2f(x, y));
	m_particleSystem->addAffector(m_affectors.force);
}

