#ifndef GAME_PLAYER_CHARGE_HPP
#define GAME_PLAYER_CHARGE_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

class PlayerCharge : public sz::Entity
{
public:
	PlayerCharge(PlayerEntity*);
	~PlayerCharge();

	void onLoad();

	void update();

	void activate();
	bool isActive();

private:

	sz::SoundAsset*		m_sndCharge;

	PlayerEntity*	m_player;

	bool				m_active;
	sz::PausableClock	m_timer;

	sf::Vector2f		m_direction;

	float		m_opacity;

	PlayerEntity::Stance::Type m_stance;

};

#endif // GAME_PLAYER_CHARGE_HPP
