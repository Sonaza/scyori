#ifndef SZEN_SHADER_ASSET_HPP
#define SZEN_SHADER_ASSET_HPP

#include <sfml/Graphics.hpp>
#include <szen/System/Assets/TAsset.hpp>

namespace sz
{
	
	class ShaderAsset : public TAsset<sf::Shader>
	{
	public:
		ShaderAsset(const std::string &path);
		~ShaderAsset();

		bool loadAsset();

	};
	
}

#endif // SZEN_SHADERASSET_HPP
