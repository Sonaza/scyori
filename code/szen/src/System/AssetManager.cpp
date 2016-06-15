#include <szen/System/AssetManager.hpp>

#include <szen/System/Assets/FontAsset.hpp>
#include <szen/System/Assets/TextureAsset.hpp>
#include <szen/System/Assets/ShaderAsset.hpp>
#include <szen/System/Assets/SoundAsset.hpp>

#include <szen/System/ErrorStream.hpp>

using namespace sz;

namespace
{
	FontAssetList		m_fonts;
	TextureAssetList	m_textures;
	ShaderAssetList		m_shaders;
	SoundAssetList		m_sounds;
}

////////////////////////////////////////////////////
void Asset::uninit()
{
	for(FontAssetList::iterator it = m_fonts.begin(); it != m_fonts.end(); delete it->second, ++it);
	m_fonts.clear();

	for(TextureAssetList::iterator it = m_textures.begin(); it != m_textures.end(); delete it->second, ++it);
	m_textures.clear();

	for(ShaderAssetList::iterator it = m_shaders.begin(); it != m_shaders.end(); delete it->second, ++it);
	m_shaders.clear();

	for(SoundAssetList::iterator it = m_sounds.begin(); it != m_sounds.end(); delete it->second, ++it);
	m_sounds.clear();
}

#define deleteSceneAssets(__type, __list) do { for(__type ## AssetList::iterator it = __list.begin(); it != __list.end();) \
	if(it->second->isSceneBound()) { \
		delete it->second; \
		it = __list.erase(it); \
	} else { ++it; } } while(0)

////////////////////////////////////////////////////
void Asset::freeSceneAssets()
{
	deleteSceneAssets(Font, m_fonts);
	deleteSceneAssets(Texture, m_textures);
	deleteSceneAssets(Shader, m_shaders);
	deleteSceneAssets(Sound, m_sounds);
}

////////////////////////////////////////////////////
void Asset::update()
{
	for(auto it = m_sounds.begin(); it != m_sounds.end(); ++it)
	{
		it->second->updateChannels();
	}
}

////////////////////////////////////////////////////
FontAsset* Asset::loadFont(const AssetID& id, const std::string &path, const bool sceneBound)
{
	FontAsset* result = getFont(id);
	
	if(!result)
	{
		result = new FontAsset(path);

		if(result->loadAsset())
		{
			result->setSceneBound(sceneBound);
			m_fonts.insert(std::make_pair(id, result));
		}
		else
		{
			szerr << "Unable to load font: " << path << ErrorStream::error;

			safedelete(result);
		}
	}

	return result;
}

////////////////////////////////////////////////////
FontAsset* Asset::getFont(const AssetID& id)
{
	FontAsset* result = NULL;

	FontAssetList::iterator it = m_fonts.find(id);
	if(it != m_fonts.end())
	{
		result = it->second;
	}
	/*else
	{
		szerr << "Font asset with id '" << id << "' could not be retrieved." << ErrorStream::error;
	}*/

	return result;
}

////////////////////////////////////////////////////
TextureAsset* Asset::loadTexture(const AssetID &id, const std::string &path, const bool sceneBound)
{
	TextureAsset* result = getTexture(id);
	
	if(!result)
	{
		result = new TextureAsset(path);

		if(result->loadAsset())
		{
			result->setSceneBound(sceneBound);
			m_textures.insert(std::make_pair(id, result));
		}
		else
		{
			szerr << "Unable to load texture: " << path << ErrorStream::error;

			safedelete(result);
		}
	}
	
	return result;
}

////////////////////////////////////////////////////
TextureAsset* Asset::getTexture(const AssetID &id)
{
	TextureAsset* result = NULL;

	TextureAssetList::iterator it = m_textures.find(id);
	if(it != m_textures.end())
	{
		result = it->second;
	}
	/*else
	{
		szerr << "Texture asset with id '" << id << "' could not be retrieved." << ErrorStream::error;
	}*/

	return result;
}

////////////////////////////////////////////////////
ShaderAsset* Asset::loadShader(const AssetID &id, const std::string &path, const bool sceneBound)
{
	ShaderAsset* result = getShader(id);
	
	if(!result)
	{
		result = new ShaderAsset(path);

		if(result->loadAsset())
		{
			result->setSceneBound(sceneBound);
			m_shaders.insert(std::make_pair(id, result));
		}
		else
		{
			safedelete(result);
		}
	}

	return result;
}

////////////////////////////////////////////////////
ShaderAsset* Asset::getShader(const AssetID &id)
{
	ShaderAsset* result = NULL;

	ShaderAssetList::iterator it = m_shaders.find(id);
	if(it != m_shaders.end())
	{
		result = it->second;
	}
	/*else
	{
		szerr << "Shader asset with id '" << id << "' could not be retrieved." << ErrorStream::error;
	}*/

	return result;
}

////////////////////////////////////////////////////
SoundAsset* Asset::loadSound(const AssetID &id, const std::string &path, const bool sceneBound)
{
	SoundAsset* result = getSound(id);
	
	if(!result)
	{
		result = new SoundAsset(path);

		if(result->loadAsset())
		{
			result->setSceneBound(sceneBound);
			m_sounds.insert(std::make_pair(id, result));
		}
		else
		{
			safedelete(result);
		}
	}

	return result;
}

////////////////////////////////////////////////////
SoundAsset* Asset::getSound(const AssetID &id)
{
	SoundAsset* result = NULL;

	SoundAssetList::iterator it = m_sounds.find(id);
	if(it != m_sounds.end())
	{
		result = it->second;
	}
	/*else
	{
		szerr << "Sound asset with id '" << id << "' could not be retrieved." << ErrorStream::error;
	}*/

	return result;
}
