#ifndef SZEN_TEXTURE_ASSET_HPP
#define SZEN_TEXTURE_ASSET_HPP

#include <sfml/Graphics.hpp>
#include <szen/System/Assets/TAsset.hpp>

namespace sz
{
	
	class TextureAsset : public TAsset<sf::Texture>
	{
	public:
		TextureAsset(const std::string &path);
		~TextureAsset();

		bool loadAsset();

		// Chance texture filtering between nearest and linear
		void setSmooth(const bool v);

	};
	
}

#endif // SZEN_TEXTUREASSET_HPP
