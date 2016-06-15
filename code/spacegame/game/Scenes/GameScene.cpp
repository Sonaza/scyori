#include <game/Scenes/GameScene.hpp>
#include <szen/Game/PrefabLoader.hpp>

#define sprintf sprintf_s

////////////////////////////////////////////////////
GameScene::GameScene()
{
	m_gameStarted = false;
}

////////////////////////////////////////////////////
GameScene::~GameScene()
{
}

#include <game/Entities/Effects/EnergyBeam.hpp>

#include <game/Scenes/MenuScene.hpp>

////////////////////////////////////////////////////
void GameScene::load()
{
	sz::Core::toggleSafeExit(false);

	sz::PrefabLoader::loadPrefabAssets("data/assets.szt");

	fps = 0.f;

	m_player = sz::World::spawn<PlayerEntity>();

	/*{
		sz::Asset::loadTexture("ball", "data/ball.png");
		sz::Asset::loadShader("shader", "data/shader.frag");

		test2 = sz::World::spawn<ActorEntityBase>();
		test2->m_type = ActorEntityBase::Planet;

		test2->setLayer(sz::EntityLayer::Foreground, 6);

		test2->attach<sz::Transform>();

		sz::Renderer* renderer = test2->attach<sz::Renderer>("ball");
		renderer->setShader("shader");
		
		sz::Physics* physics = test2->attach<sz::Physics>();

		test2->call(&sz::Transform::setPosition, 0.f, -500.f);
		//test2->send(&sz::Transform::setScale, sf::Vector2f(0.7f, 0.7f));
		//test2->send(&sz::Transform::setRotation, -7.f);

		//test2->call(&sz::Renderer::setBlendMode, sf::BlendAdd);

		//physics->createCircle(760.f / 2.f, sz::Physics::Static);
		physics->createCircle(460.f / 2.f, sz::Physics::Static);
		physics->finalizeBody(1.f, 0.7f, 1.f);

		sz::World::spawn<EnergyBeam>(test2, m_player);

		//test2->call(&sz::Transform::setScale, 1.2f, 1.2f);
	}*/

	stationLoad();

	{
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/background.szt");
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/bg_stars.szt");
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/bg_stars2.szt");
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/bg_sun.szt");
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/bg_planet.szt");
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/bg_darken.szt");
		{
			auto e = sz::World::spawn<sz::Entity>();
			e->loadFromPrefab("data/bg_darken.szt");
			e->getTransform->setRotation(150.f);
			e->getTransform->setPosition(900, -1200);
			e->getTransform->setScale(3.f, 1.3f);
			e->getRenderer->setOpacity(0.6f);
		}
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/bg_asteroid_dust.szt");

		thor::setRandomSeed(46);

		for(int i=0; i < 260; ++i)
		{
			float angle = sz::toRadians(thor::randomDev(20.f, 10.f));

			sf::Vector2f dir(cos(angle), sin(angle));

			sf::Vector2f pos(140.f, -140.f + thor::random(-120.f, 120.f));
			pos.y += cos(i * 0.5f) * thor::random(0.f, 200.f);
			pos += dir * thor::random(-4000.f, 4000.f);

			auto e = sz::World::spawn<sz::Entity>();

			int an = thor::random(0, 1);

			float rs = thor::random(0.38f, 0.49f);
			sf::Vector2f s(rs, rs);

			if(an == 0)
			{
				e->loadFromPrefab("data/bg_asteroid.szt");
				s.x *= 1.3f;
			}
			else
			{
				e->loadFromPrefab("data/bg_asteroid2.szt");
			}

			auto t = e->getTransform;

			t->setScale(s);

			//float s = an == 0 ? thor::random(0.6f,  1.2f) : thor::random(0.38f, 0.49f);
			
			t->setPosition(pos);
			t->setRotation(thor::random(0.f, 360.f));

			t->setDepth(49.f - (i / 150.f));
		}
		
		thor::setRandomSeed((unsigned long)std::time(0));
	}

	/*{
		sz::Entity* e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/interface.szt");
	}*/

	{
		GameGlobals::get()->gameMessage = sz::World::spawn<GameMessage>();
	}

	{
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/interface_bottom.szt");
		sz::World::spawn<sz::Entity>()->loadFromPrefab("data/interface_top.szt");
	}

	{
		GameGlobals::get()->impact = sz::World::spawn<sz::ParticleEntity>();
		GameGlobals::get()->impact->loadFromPrefab("data/effects/impact.szt");

		GameGlobals::get()->impactbits = sz::World::spawn<sz::ParticleEntity>();
		GameGlobals::get()->impactbits->loadFromPrefab("data/effects/impactbits.szt");
	}

	{
		m_gameoverScreen = sz::World::spawn<sz::Entity>();
		m_gameoverScreen->loadFromPrefab("data/game_over.szt");
	}

	m_zoom = 1.5f;
	sz::Camera::setZoom(m_zoom);

	////////////////////////////////////////////////////

	sz::SceneManager::setOverlayScene(new MenuScene(this));
	sz::SceneManager::toggleOverlay(true);

	resetGame();
}

////////////////////////////////////////////////////
void GameScene::stationLoad()
{
	m_stationAnchor = sz::World::spawn<sz::Entity>();
	m_stationAnchor->loadFromPrefab("data/station/station_anchor.szt");

	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_frame_tl.szt");
	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_frame_tr.szt");
	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_frame_bl.szt");
	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_frame_br.szt");

	auto mask = sz::Asset::getTexture("station_pipe_mask")->getAsset();
	mask->setRepeated(true);

	{
		auto e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/station/station_pipe_n.szt");
		e->getRenderer->getShader()->setParameter("u_mask", *mask);
	}

	{
		auto e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/station/station_pipe_se.szt");
		e->getRenderer->getShader()->setParameter("u_mask", *mask);
	}

	{
		auto e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/station/station_pipe_sw.szt");
		e->getRenderer->getShader()->setParameter("u_mask", *mask);
	}

	m_stationWallAnchor = sz::World::spawn<sz::Entity>();
	m_stationWallAnchor->loadFromPrefab("data/station/station_wall_anchor.szt");

	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_wall_s.szt");
	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_wall_nw.szt");
	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_wall_ne.szt");

	sz::World::spawn<sz::Entity>()->loadFromPrefab("data/station/station_center.szt");

	{
		auto e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/station/forge_green.szt");

		float a = sz::toRadians(120 + 30);
		sf::Vector2f dir(cos(a), sin(a));

		e->getPhysics->setPosition(dir * 5100.f);
		e->getRenderer->setPrerender(375, 375);
	}

	{
		auto e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/station/forge_blue.szt");

		float a = sz::toRadians(30);
		sf::Vector2f dir(cos(a), sin(a));

		e->getPhysics->setPosition(dir * 5100.f);
		e->getRenderer->setPrerender(375, 375);
	}

	{
		auto e = sz::World::spawn<sz::Entity>();
		e->loadFromPrefab("data/station/forge_red.szt");

		float a = sz::toRadians(-90);
		sf::Vector2f dir(cos(a), sin(a));

		e->getPhysics->setPosition(dir * 5100.f);
		e->getRenderer->setPrerender(375, 375);
	}
}

////////////////////////////////////////////////////
void GameScene::unload()
{
	
}

////////////////////////////////////////////////////
bool GameScene::isGameStarted()
{
	return m_gameStarted;
}

////////////////////////////////////////////////////
void GameScene::startGame()
{
	m_gameTimer.restart();

	m_gameStarted = true;

	m_currentWave = 0;

	m_player->setEnabled(true);
	m_player->spawn();

	m_gameoverScreen->getRenderer->setOpacity(0.f);
}

////////////////////////////////////////////////////
void GameScene::resetGame()
{
	m_gameStarted = false;

	m_currentWave = 0;

	m_player->setEnabled(false);
	m_player->reset();
}

////////////////////////////////////////////////////
void GameScene::pauseGame()
{
	sz::Scene::togglePause(true);
	sz::SceneManager::toggleOverlay(true);

	sz::Input::clearInputs();
}

////////////////////////////////////////////////////
void GameScene::stopGame()
{
	m_gameStarted = false;

	sz::Scene::togglePause(false);
	//sz::SceneManager::toggleOverlay(false);

	GameGlobals::get()->setBossSpawned(false);

	m_player->reset();
	m_player->setEnabled(false);

	sz::World::destroyAllOf<EnemyEntityBase>();
	sz::World::destroyAllOf<BulletEntityBase>();

	m_gameoverScreen->getRenderer->setOpacity(0.f);

	sz::Input::clearInputs();
}

////////////////////////////////////////////////////
void GameScene::events(const sf::Event& event)
{

}

////////////////////////////////////////////////////
void GameScene::update()
{
	/*if(sz::Input::isKeyHit(sf::Keyboard::T))
	{
		sz::Scene::togglePause(!sz::Scene::isPaused());
	}*/

	if(!m_gameStarted || sz::Scene::isPaused()) return;

	if(!sz::Window::isActive())
	{
		pauseGame();
	}

	if(sz::Input::isKeyHit(sf::Keyboard::Escape))
	{
		if(m_player->isDead())
		{
			stopGame();
			sz::SceneManager::toggleOverlay(true);
		}
		else
		{
			pauseGame();
		}
	}

	{
		auto renderer = m_gameoverScreen->getRenderer;
		float opacity = renderer->getOpacity();
		float target = m_player->isDead() ? 1.f : 0.f;

		opacity += (target - opacity) / 7.f;
		renderer->setOpacity(opacity);

		float s = 1.f - (1.f-opacity) * 0.25f;
		m_gameoverScreen->getTransform->setScale(s, s);
	}

	if(m_player->isDead() && m_player->getDeathTimer() >= sf::seconds(15.f))
	{
		stopGame();
		sz::SceneManager::toggleOverlay(true);
	}

	{
		auto t = m_stationWallAnchor->getTransform;
		//t->setRotation(cos(Time.total / 5.f));
		float s = 1.f + cos(Time.total / 2.f) * 0.01f - 0.01f;
		t->setScale(s, s);
	}

	spawnUpdate();

	if(fpsupdate.getElapsedTime().asSeconds() > 0.25f)
	{
		fps = 1.f / fpsclock.getElapsedTime().asSeconds();
		fpsupdate.restart();
	}

	fpsclock.restart();
}

#include <game/Entities/Actors/Enemies/EnemyBossGreen.hpp>
#include <game/Entities/Actors/Enemies/EnemyBossRed.hpp>

////////////////////////////////////////////////////
void GameScene::spawnUpdate()
{
	if(m_gameTimer.getElapsedTime() <= sf::seconds(2.f)) return;

	sf::Time spawnInterval;

	bool isBossSpawned = GameGlobals::get()->isBossSpawned();

	if(!isBossSpawned)
	{
		spawnInterval = sf::seconds(1.5f);
	}
	else
	{
		spawnInterval = sf::seconds(2.5f);
	}

	if(m_enemySpawnTimer.getElapsedTime() >= spawnInterval && sz::World::getEntityCount() < 480)
	{
		m_enemySpawnTimer.restart();

		if(!isBossSpawned)
		{
			m_currentWave++;

			if(m_currentWave % 62 == 0)
			{
				//if(m_currentWave == 5)
				{
					spawnEnemy<EnemyBossRed>(sf::Vector2f(0.f, 0.f));
				}
			}
		}

		// MONSTER SPAWN ASD
		int mc = thor::random(1, 2);
		for(int wt = 0; wt < mc; ++wt)
		{
			ActorEntityBase* e = NULL;

			float corner = thor::random(0, 2);

			float angle = sz::toRadians(30.f + corner * 120.f);
			float dist = 5800.f;

			sf::Vector2f position = sf::Vector2f(cos(angle), sin(angle)) * dist;

			int wavetype = thor::random(0, 3);

			sz::EntityList ent;

			if(isBossSpawned)
			{
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
			}

			switch(wavetype)
			{
			case 0:
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				break;

			case 1:
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyHeavy>(position);
				spawnEnemy<EnemyHugger>(position);
				break;

			case 2:
				spawnEnemy<EnemyHugger>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyHeavy>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyRotary>(position);
				break;

			case 3:
				spawnEnemy<EnemyRotary>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyBasic>(position);
				spawnEnemy<EnemyHeavy>(position);
				break;
			}

			for(int i=0; i < ent.size(); ++i)
			{
				ent[i]->getTransform->setPosition(position);
			}
		}
	}
}

////////////////////////////////////////////////////
void GameScene::setSpawnSettings(EnemyEntityBase* enemy, sf::Vector2f pos)
{
	//enemy->getTransform->setPosition(pos);
}

////////////////////////////////////////////////////
void GameScene::render()
{
	/*char s[128];

	{
		float zoom = sz::Camera::getZoom();
		sprintf(s, "FPS %.1f", fps);

		sf::Text zoomtext(s, *sz::Asset::getFont("powerchord")->getAsset(), 45);

		zoomtext.setColor(sf::Color::White);
		zoomtext.setPosition(10.f, 10.f);
		sz::Window::getRenderWindow()->draw(zoomtext);
	}*/

	/*if(isGameStarted())
	{
		sprintf(s, "Health %.1f/%.1f\nFlux %.1f/%.1f",
			m_player->getEnergy(), m_player->getEnergyMax(),
			m_player->getFlux(), m_player->getFluxMax());

		sf::Text zoomtext(s, *sz::Asset::getFont("powerchord")->getAsset(), 35);

		zoomtext.setColor(sf::Color::White);
		zoomtext.setPosition(Screen.width / 2.f - 140.f, 10.f);
		//sz::Window::getRenderWindow()->draw(zoomtext);
	}*/
}