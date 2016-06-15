#include <szen/Game/PrefabLoader.hpp>

#include <szen/System/AssetManager.hpp>
#include <szen/System/ErrorStream.hpp>

#include <json/json.h>
#include <fstream>
#include <sstream>

#include <map>

#include <szen/Game/Components/Transform.hpp>
#include <szen/Game/Components/Physics.hpp>
#include <szen/Game/Components/Renderer.hpp>
#include <szen/Game/Components/TextRenderer.hpp>
#include <szen/Game/Components/Animation.hpp>
#include <szen/Game/Components/ParticleComponent.hpp>

#include <boost/algorithm/string.hpp>

#include <szen/System/Window.hpp>

using namespace sz;

namespace
{
	typedef std::map<const std::string, const std::string> SavedPrefabList;
	SavedPrefabList	savedPrefabs;

	////////////////////////////////////////////////////
	void clearWindowEvents()
	{
		auto window = sz::Window::getRenderWindow();
		sf::Event event; while(window->pollEvent(event));
	}

	////////////////////////////////////////////////////
	void loadPrefabAssets(json::Value &root, std::string subdir = "")
	{
		if(root.isMember("assets"))
		{
			if(!subdir.empty() && subdir.back() != '/' && subdir.back() != '\\')
			{
				subdir.append("/");
			}

			json::Value assets = root["assets"];

			if(assets.isMember("textures") && !assets["textures"].empty())
			{
				json::Value textures = assets["textures"];

				int i=0;
				for(json::Value::iterator it = textures.begin(); it != textures.end(); ++it)
				{
					if(Asset::getTexture(it.memberName()) != NULL)
					{
						szerr << "Texture asset '" << it.memberName() << "' is already loaded" << ErrorStream::warning;
						continue;
					}

					if((*it).isArray())
					{
						Asset::loadTexture(it.memberName(), subdir + (*it)[0U].asString(), (*it)[1U].asBool());
					}
					else
					{
						Asset::loadTexture(it.memberName(), subdir + (*it).asString());
					}

					if(++i % 5 == 0)
						clearWindowEvents();
				}
			}

			clearWindowEvents();

			if(assets.isMember("sounds") && !assets["sounds"].empty())
			{
				json::Value textures = assets["sounds"];

				for(json::Value::iterator it = textures.begin(); it != textures.end(); ++it)
				{
					if((*it).isArray())
					{
						Asset::loadSound(it.memberName(), subdir + (*it)[0U].asString(), (*it)[1U].asBool());
					}
					else
					{
						Asset::loadSound(it.memberName(), subdir + (*it).asString());
					}
				}
			}

			clearWindowEvents();

			if(assets.isMember("fonts") && !assets["fonts"].empty())
			{
				json::Value fonts = assets["fonts"];

				for(json::Value::iterator it = fonts.begin(); it != fonts.end(); ++it)
				{
					if((*it).isArray())
					{
						Asset::loadFont(it.memberName(), subdir + (*it)[0U].asString(), (*it)[1U].asBool());
					}
					else
					{
						Asset::loadFont(it.memberName(), subdir + (*it).asString());
					}
				}
			}

			clearWindowEvents();

			if(assets.isMember("shaders") && !assets["shaders"].empty())
			{
				json::Value shaders = assets["shaders"];

				for(json::Value::iterator it = shaders.begin(); it != shaders.end(); ++it)
				{
					if((*it).isArray())
					{
						Asset::loadShader(it.memberName(), subdir + (*it)[0U].asString(), (*it)[1U].asBool());
					}
					else
					{
						Asset::loadShader(it.memberName(), subdir + (*it).asString());
					}
				}
			}
		}
	}

}

////////////////////////////////////////////////////
std::string readfile(const std::string &path)
{
	std::ifstream file(path);
	if(!file.is_open())
	{
		szerr << "Unable to open prefab file: " << path << ErrorStream::error;
		return "";
	}

	std::stringstream ss;
	ss << file.rdbuf();

	file.close();

	return ss.str();
}

////////////////////////////////////////////////////
json::Value getPrefabJson(const std::string &path)
{
	json::Value result;
	SavedPrefabList::iterator it = savedPrefabs.find(path);

	std::string code;
	bool loadassets = false;

	std::string dir;

	if(it == savedPrefabs.end())
	{
		code = readfile(path);
		if(code.empty())
		{
			szerr << "Unable to open prefab file: " << path << ErrorStream::error;
		}

		savedPrefabs.insert(std::make_pair(
			path, code
		));

		loadassets = true;
		
		std::vector<std::string> segment;
		boost::split(segment, path, boost::is_any_of("/\\"));

		if(segment.size() > 1)
		{
			segment.pop_back();
			dir = boost::join(segment, "/") + "/";
		}
	}
	else
	{
		code = it->second;
	}

	json::Reader reader;

	if(!reader.parse(code, result, false))
	{
		szerr << "Parsing prefab failed: " << path << "\n\n"
			<< reader.getFormatedErrorMessages() << ErrorStream::error;
	}

	// Load assets required by prefab if any mentioned
	if(loadassets) loadPrefabAssets(result, dir);

	return result;
}

////////////////////////////////////////////////////
bool PrefabLoader::loadPrefab(const std::string &prefab, Entity* entity)
{
	assert(!entity->hasComponents());

	json::Value root = getPrefabJson(prefab);

	if(root.isMember("layer"))
	{
		json::Value layer = root["layer"];

		if(layer.isArray() && layer.size() == 2)
		{
			const std::string level = layer[0U].asString();
			const int id = layer[1U].asInt();

			if(level == "background" || level == "bg")
			{
				entity->setLayer(EntityLayer::Background, id);
			}
			else if(level == "foreground" || level == "fg")
			{
				entity->setLayer(EntityLayer::Foreground, id);
			}
			else if(level == "interface" || level == "ui")
			{
				entity->setLayer(EntityLayer::Interface, id);
			}
		}
		else
		{
			szerr << "Invalid value for entity layer." << ErrorStream::error;
		}
	}

	if(!root.isMember("components"))
	{
		szerr << "Prefab must have list of components: " << prefab << ErrorStream::error;
		return false;
	}

	struct PrefabComponent
	{
		enum ComponentId
		{
			unselected = -1,
			transform = 0,
			physics,
			renderer,
			textrenderer,
			animation,
			particle
		} id;

		json::Value data;

		PrefabComponent() : id(unselected) {}
	};

	std::vector<PrefabComponent> componentlist;

	json::Value components = root["components"];

	for(json::Value::iterator it = components.begin(); it != components.end(); ++it)
	{
		const std::string componentType = it.memberName();

		PrefabComponent pc;
		pc.data = *it;

		if(componentType == "transform")
		{
			pc.id = PrefabComponent::transform;
		}
		else if(componentType == "physics")
		{
			pc.id = PrefabComponent::physics;
		}
		else if(componentType == "renderer")
		{
			pc.id = PrefabComponent::renderer;
		}
		else if(componentType == "textrenderer")
		{
			pc.id = PrefabComponent::textrenderer;
		}
		else if(componentType == "animation")
		{
			pc.id = PrefabComponent::animation;
		}
		else if(componentType == "particle")
		{
			pc.id = PrefabComponent::particle;
		}

		if(pc.id == PrefabComponent::unselected)
		{
			szerr << "Unable to recognise given component in "
				  << prefab << ": " << componentType << "\n\n"
				  << "Entity will not work as intended." << ErrorStream::error;
		}
		else
		{
			componentlist.push_back(pc);
		}
	}

	// Sort by component id (already in the right order)
	std::sort(componentlist.begin(), componentlist.end(), [](PrefabComponent& a, PrefabComponent& b)
	{
		return a.id < b.id;
	});

	for(std::vector<PrefabComponent>::iterator it = componentlist.begin(); it != componentlist.end(); ++it)
	{
		PrefabComponent pc = *it;
		Component* component = NULL;

		switch(pc.id)
		{
			case PrefabComponent::transform:
			{
				component = entity->attach<Transform>();
				break;
			}

			case PrefabComponent::physics:
			{
				component = entity->attach<Physics>();
				break;
			}

			case PrefabComponent::renderer:
			{
				component = entity->attach<Renderer>();
				break;
			}

			case PrefabComponent::textrenderer:
			{
				component = entity->attach<TextRenderer>();
				break;
			}

			case PrefabComponent::animation:
			{
				sf::Vector2u size(
					pc.data["framesize"][0U].asUInt(),
					pc.data["framesize"][1U].asUInt()
				);

				component = entity->attach<Animation>(size);
				break;
			}

			case PrefabComponent::particle:
			{
				component = entity->attach<ParticleComponent>(
					pc.data["texture"].asString()
				);
				break;
			}
		}

		if(component)
		{
			component->parsePrefab(pc.data);
		}
	}

	if(root.isMember("tag") && root["tag"].isString())
	{
		const std::string tag = root["tag"].asString();
		entity->setTag(tag);
	}

	if(root.isMember("parent") && root["parent"].isString())
	{
		const std::string tag = root["parent"].asString();
		Entity* parent = World::findWithTag(tag);

		if(parent && parent != entity)
		{
			entity->setParent(parent);
		}
		else if(parent == entity)
		{
			szerr << "In " << prefab << ":\n\nEntity cannot parent itself." << ErrorStream::warning;
		}
		else
		{
			szerr << "In " << prefab << ":\n\nParent entity with tag '" << tag << "' could not be found, check load order or typos." << ErrorStream::warning;
		}
	}

	return true;
}

////////////////////////////////////////////////////
bool PrefabLoader::loadPrefabAssets(const std::string &prefab)
{
	json::Value root = getPrefabJson(prefab);
	return !root.empty();
}