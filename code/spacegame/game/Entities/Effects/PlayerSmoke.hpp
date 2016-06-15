#ifndef GAME_PLAYER_SMOKE_HPP
#define GAME_PLAYER_SMOKE_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

class PlayerSmoke : public sz::Entity
{
public:
	PlayerSmoke(PlayerEntity*);
	~PlayerSmoke();

	void update();

	void setIntensity(float intensity);

private:

	float m_intensity;

	PlayerEntity*	m_player;

	PlayerEntity::Stance::Type m_stance;

};

#endif // GAME_PLAYER_LASH_HPP
