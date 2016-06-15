#ifndef SZEN_SZEN_HPP
#define SZEN_SZEN_HPP

#if !defined(_DEBUG)
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <szen/System/Core.hpp>
#include <szen/System/Window.hpp>

/////////////////////////

#include <szen/System/Config.hpp>

#include <szen/System/InputManager.hpp>

#include <szen/System/Scene.hpp>
#include <szen/System/SceneManager.hpp>

#include <szen/System/Utility.hpp>
#include <szen/System/PausableClock.hpp>

//////////////////////////
// Asset includes

#include <szen/System/AssetManager.hpp>

#include <szen/System/Assets/FontAsset.hpp>
#include <szen/System/Assets/TextureAsset.hpp>
#include <szen/System/Assets/ShaderAsset.hpp>
#include <szen/System/Assets/SoundAsset.hpp>

//////////////////////////
// Game related

#include <szen/Game/World.hpp>
#include <szen/Game/Camera.hpp>

#include <szen/Game/Entity.hpp>
#include <szen/Game/Component.hpp>

#include <szen/Game/Components/Animation.hpp>
#include <szen/Game/Components/Transform.hpp>
#include <szen/Game/Components/Physics.hpp>
#include <szen/Game/Components/Renderer.hpp>
#include <szen/Game/Components/TextRenderer.hpp>
#include <szen/Game/Components/ParticleComponent.hpp>

#include <szen/Game/ParticleEntity.hpp>
#include <szen/Game/PhysicsContact.hpp>

#define getRenderer		getComponent<sz::Renderer>()
#define getTextRenderer getComponent<sz::TextRenderer>()
#define getTransform	getComponent<sz::Transform>()
#define getPhysics		getComponent<sz::Physics>()
#define getAnimation	getComponent<sz::Animation>()
#define getParticle		getComponent<sz::ParticleComponent>()

#endif // SZEN_SZEN_HPP
