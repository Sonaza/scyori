#ifndef GAME_PLAYER_LASH_HPP
#define GAME_PLAYER_LASH_HPP

#include <szen/szen.hpp>
#include <vector>

class PlayerEntity;

class PlayerLash : public sz::Entity
{
public:
	PlayerLash(PlayerEntity*);
	~PlayerLash();

	void update();

	void activate();

	bool isLashing();

private:

	std::vector<sz::Entity*>	m_enemyList;

	PlayerEntity*	p_player;

	std::vector<float>	m_rotations;
	std::vector<float>	m_scales;

	int32	m_frame;
	float	m_rotation;
	float	m_targetRotation;
	float	m_rotationStep;
	float	m_checkInterval;

	float	m_scale;
	float	m_targetScale;
	float	m_scaleStep;

	float	m_progress;

	bool	m_isLashing;

};

#endif // GAME_PLAYER_LASH_HPP
