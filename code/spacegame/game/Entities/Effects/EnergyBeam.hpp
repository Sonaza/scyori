#ifndef GAME_ENERGYBEAM_HPP
#define GAME_ENERGYBEAM_HPP

#include <szen/szen.hpp>

class EnergyBeam : public sz::Entity
{
public:
	EnergyBeam(sz::Entity* pivot, sz::Entity* target);
	~EnergyBeam();

	void onLoad();

	void update();

	void setColor(sf::Color&);

private:

	sz::Entity*		m_pivot;
	sz::Entity*		m_target;

	sz::Transform*	m_pivotTransform;
	sz::Transform*	m_targetTransform;

	sz::Transform*	m_transform;

};

#endif // GAME_ENERGYBEAM_HPP
