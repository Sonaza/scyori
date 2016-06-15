#ifndef GAME_ExplosionEffect_HPP
#define GAME_ExplosionEffect_HPP

#include <szen/szen.hpp>

class ExplosionEffect : public sz::Entity
{
public:
	ExplosionEffect(sz::Entity* parent);
	~ExplosionEffect();

	void update();

	void setScale(float min, float max);

private:

	sz::Entity*	m_parent;

	sz::PausableClock timer;

};

#endif // GAME_ExplosionEffect_HPP
