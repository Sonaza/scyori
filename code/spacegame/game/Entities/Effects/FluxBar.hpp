#ifndef GAME_FLUXBAR_HPP
#define GAME_FLUXBAR_HPP

#include <szen/szen.hpp>

class PlayerEntity;

class FluxBar : public sz::Entity
{
public:
	FluxBar(PlayerEntity* player);
	~FluxBar();

	void update();

private:

	PlayerEntity*	m_player;

	float			m_current;

};

#endif // GAME_FLUXBAR_HPP
