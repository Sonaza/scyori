#ifndef SZEN_PREFABLOADER_HPP
#define SZEN_PREFABLOADER_HPP

#include <string>

#include <szen/Game/World.hpp>

namespace sz
{
	
	namespace PrefabLoader
	{

		bool loadPrefab(const std::string &prefab, Entity*);
		bool loadPrefabAssets(const std::string &prefab);

	}
	
}

#endif // SZEN_PREFABLOADER_HPP
