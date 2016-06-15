#ifndef SZEN_FONTASSET_HPP
#define SZEN_FONTASSET_HPP

#include <sfml/Graphics.hpp>
#include <szen/System/Assets/TAsset.hpp>

namespace sz
{
	
	class FontAsset : public TAsset<sf::Font>
	{
	public:
		FontAsset(const std::string &path);
		~FontAsset();

		bool loadAsset();
	};
	
}

#endif // SZEN_FONTASSET_HPP
