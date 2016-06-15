#include <game/Entities/Effects/FluxBar.hpp>

#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
FluxBar::FluxBar(PlayerEntity* player) :
	m_player	(player),
	m_current	(0.f)
{
	loadFromPrefab("data/flux_bar.szt");
}

////////////////////////////////////////////////////
FluxBar::~FluxBar()
{

}

////////////////////////////////////////////////////
void FluxBar::update()
{
	m_current += (m_player->getFluxFraction() - m_current) / 5.f;
	m_current = std::max(0.f, m_current);

	auto shader = getRenderer->getShader();
	if(shader)
	{
		shader->setParameter("u_fluxbar", 100);
		shader->setParameter("u_progress", m_current * 0.475f);
	}

	/*float s = m_current * 1.5f;
	getTransform->setScale(s, 0.75f);*/
}