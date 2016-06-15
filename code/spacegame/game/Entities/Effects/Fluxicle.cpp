#include <game/Entities/Effects/Fluxicle.hpp>

#include <game/GameGlobals.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
Fluxicle::Fluxicle(sz::Entity* atEntity) :
	m_speed		(0.f),
	m_angle		(0.f)
{
	loadFromPrefab("data/fluxicle.szt");

	m_player = GameGlobals::get()->player;

	auto pos = atEntity->getTransform->getPosition();
	pos.y *= -1.f;
	getTransform->setPosition(pos);
}

////////////////////////////////////////////////////
Fluxicle::~Fluxicle()
{
	
}

////////////////////////////////////////////////////
void Fluxicle::onLoad()
{
	setAngle(thor::randomDev(0.f, PI));
	setSpeed(thor::random(500.f, 1000.f));

	float scale = thor::random(0.4f, 0.85f);
	getTransform->setScale(scale, scale);

	getTransform->setRotation(thor::random(-180.f, 180.f));

	m_pullDelay = sf::milliseconds(thor::random(180, 300));
}

////////////////////////////////////////////////////
void Fluxicle::setSpeed(float val)
{
	m_speed = val;
}

////////////////////////////////////////////////////
void Fluxicle::setAngle(float val)
{
	m_angle = val;
}

////////////////////////////////////////////////////
void Fluxicle::update()
{
	if(GameGlobals::get()->player->isDead()) destroyEntity();

	getTransform->setRotation(getTransform->getRotation() + Time.total * 15.f);

	auto playerPosition = m_player->getTransform->getPosition();
	auto myPosition = getTransform->getPosition();

	float dist = sz::distancesqrt(myPosition, playerPosition);

	if(dist <= 60)
	{
		m_player->addFlux(m_player->getFluxPercentage(0.24f));

		if(thor::random(0, 5) >= 4)
		{
			std::stringstream sample;
			sample << "flux_collect_" << thor::random(1, 3);

			sz::SoundAsset* snd = sz::Asset::getSound(sample.str());
			auto channel = snd->play(24.f);
			//snd->setPitch(channel, thor::random(0.98f, 1.02f));
		}

		destroyEntity();
	}
	else
	{
		if(Time.totalTime >= m_pullDelay)
		{
			float targetAngle = -sz::getAngle(myPosition, playerPosition);

			float d = 15.f - Time.totalTime.asSeconds() * 6.f;
			//dist / 200.f - std::max(0.f, m_speed-1000.f) / 16.f;
			m_angle += sz::angleDiff(m_angle, targetAngle) / std::min(15.f, std::max(1.f, d));

			m_speed += 1600.f * Time.delta;
		}

		sf::Vector2f dir(
			cos(m_angle), sin(m_angle)
		);

		myPosition.y *= -1.f;
		getTransform->setPosition(myPosition + dir * m_speed * Time.delta);
	}
}