#ifndef GAME_ENERGYBAR_HPP
#define GAME_ENERGYBAR_HPP

#include <szen/szen.hpp>

class PlayerEntity;

class EnergyBar : public sz::Entity
{
public:
	EnergyBar(PlayerEntity* player);
	~EnergyBar();

	void update();

private:

	sz::Entity*		m_energyText;

	PlayerEntity*	m_player;

	float			m_current;

};

#endif // GAME_ENERGYBAR_HPP
