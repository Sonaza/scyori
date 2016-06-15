#include <game/Entities/Effects/ExplosionEffect.hpp>

#include <game/GameGlobals.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
ExplosionEffect::ExplosionEffect(sz::Entity* parent) :
	m_parent	(parent)
{
	loadFromPrefab("data/effects/explosion.szt");
	
	auto pos = m_parent->getTransform->getPosition();
	pos.y *= -1.f;
	getTransform->setPosition(pos);

	float scale = thor::random(0.6f, 2.2f);
	getTransform->setScale(scale, scale);

	getTransform->setRotation(thor::random(-180.f, 180.f));

	if(thor::random(0, 2) >= 2)
	{
		getAnimation->play("explode_slower");
	}
	else
	{
		getAnimation->play("explode");
	}

	int snd = thor::random(0, 2);

	std::stringstream sample;
	sample << "explosion_" << thor::random(1, 2);

	sz::SoundAsset* sound = sz::Asset::getSound(sample.str());

	auto channel = sound->play(thor::random(40.f, 50.f));

	/*auto playerPos = GameGlobals::get()->player->getTransform->getPosition();

	sf::Vector3f pan(
		pos.x - playerPos.x,
		-pos.y - playerPos.y,
		0.f
	);

	sound->setPanning(channel, pan);*/

	//setParent(m_parent);
}

////////////////////////////////////////////////////
ExplosionEffect::~ExplosionEffect()
{
	
}

////////////////////////////////////////////////////
void ExplosionEffect::update()
{
	auto anim = getAnimation;

	//if(timer.getElapsedTime() >= sf::seconds(0.5f))//anim->getAnimationTag() == "explode_smoke")
	if(anim->getCurrentFrame() >= 50)
	{
		auto s = getTransform->getScale().x + 0.7f * Time.delta;
		getTransform->setScale(s, s);
	}

	if(anim->isStopped())
	{
		destroyEntity();
	}
}

////////////////////////////////////////////////////
void ExplosionEffect::setScale(float min, float max)
{
	float scale = thor::random(min, max);
	getTransform->setScale(scale, scale);
}
