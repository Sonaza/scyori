#include <game/Entities/Effects/EnergyBar.hpp>

#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
EnergyBar::EnergyBar(PlayerEntity* player) :
	m_player	(player),
	m_current	(1.f)
{
	loadFromPrefab("data/energy_bar.szt");

	/*m_energyText = sz::World::spawn<sz::Entity>();
	m_energyText->loadFromPrefab("data/energy_text.szt");
	m_energyText->setParent(this);*/
}

////////////////////////////////////////////////////
EnergyBar::~EnergyBar()
{
	
}

////////////////////////////////////////////////////
void EnergyBar::update()
{
	m_current += (m_player->getEnergyFraction() - m_current) / 5.f;
	m_current = std::max(0.f, m_current);

	auto shader = getRenderer->getShader();
	if(shader)
	{
		shader->setParameter("u_fluxbar", -100);
		shader->setParameter("u_progress", m_current * 0.45f);
	}

	/*float s = m_current;
	getTransform->setScale(s, 1.f);*/


}