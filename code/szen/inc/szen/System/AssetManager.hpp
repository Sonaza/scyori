#ifndef SZEN_ASSETMANAGER_HPP
#define SZEN_ASSETMANAGER_HPP

#include <string>
#include <map>

#define safedelete(__var) if(__var) { delete __var; __var = NULL; }
#define safedeletearray(__var) if(__var) { delete[] __var; __var = NULL; }

namespace sz
{
	
	class FontAsset;
	class TextureAsset;
	class ShaderAsset;
	class SoundAsset;

	typedef std::string AssetID;

	typedef std::map<const AssetID, FontAsset*>		FontAssetList;
	typedef std::map<const AssetID, TextureAsset*>	TextureAssetList;
	typedef std::map<const AssetID, ShaderAsset*>	ShaderAssetList;
	typedef std::map<const AssetID, SoundAsset*>	SoundAssetList;

	namespace Asset
	{

		void uninit();

		// Frees all assets that are marked as scene-bound
		void freeSceneAssets();

		void update();

		FontAsset* loadFont(const AssetID& id, const std::string &path, const bool sceneBound = true);
		FontAsset* getFont(const AssetID& id);

		TextureAsset* loadTexture(const AssetID &id, const std::string &path, const bool sceneBound = true);
		TextureAsset* getTexture(const AssetID &id);

		ShaderAsset* loadShader(const AssetID &id, const std::string &path, const bool sceneBound = true);
		ShaderAsset* getShader(const AssetID &id);

		SoundAsset* loadSound(const AssetID &id, const std::string &path, const bool sceneBound = true);
		SoundAsset* getSound(const AssetID &id);

	}
	
}

#endif // SZEN_ASSETMANAGER_HPP
