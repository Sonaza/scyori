#ifndef SZEN_TEXTRENDERER_HPP
#define SZEN_TEXTRENDERER_HPP

#include <szen/Game/Component.hpp>

namespace sz
{

	class TextRenderer : public Component
	{
	public:
		TextRenderer();
		virtual ~TextRenderer();

		void attached();
		void update();
		void parsePrefab(json::Value&);

		void setFont(const std::string &assetID);
		void setSize(const size_t size);

		void setBlendMode(const sf::BlendMode);

		void setColor(const sf::Color&);
		void setOpacity(float);

		void setText(const std::string &text);
		std::string getText() const;

		void draw(sf::RenderTarget&);

	private:

		//float			m_relativeScale;

		sf::Text		m_text;
		size_t			m_size;

		sf::Font*		m_font;

		sf::BlendMode	m_blendmode;
		sf::Color		m_color;
		std::string		m_string;

		sf::Vector2f	m_origin;

	};
	
}

#endif // SZEN_TEXTRENDERER_HPP
