#ifndef SZEN_RENDERER_HPP
#define SZEN_RENDERER_HPP

#include <szen/Game/Component.hpp>

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace sz
{

	class TextureAsset;
	class ShaderAsset;

	typedef std::unique_ptr<sf::Sprite> SpritePtr;

	class Renderer : public Component
	{
		friend class Animation;

	public:
		Renderer();
		Renderer(const std::string &textureID, const std::string &shaderID = "");
		virtual ~Renderer();

		void attached();
		void update();
		void parsePrefab(json::Value&);

		void setTexture(const std::string &assetID);
		void setShader(const std::string &assetID);
		sf::Shader* getShader();

		void setBlendMode(const sf::BlendMode);

		void setColor(const sf::Color&);
		void setOpacity(float);
		sf::Color getColor();

		float getOpacity() { return m_color.a / 255.f; }

		void draw(sf::RenderTarget&);

		sf::Sprite* getSprite() const;
		sf::Vector2u getTextureSize() const;

		void setTextureRect(sf::IntRect&);

		void setPrerender(sf::Uint32 width, sf::Uint32 height);

	/*protected:

		sf::Shader			m_shader;
		sf::VertexArray		m_vertexArray;
		sf::Transform		m_transform;*/

	private:

		bool				m_isPrerendering;
		sf::Vector2u		m_prerenderSize;
		sf::RenderTexture	m_prerenderTexture;
		const sf::Texture*	m_prerendered;

		PausableClock		m_shaderClock;

		sf::Vector2u		m_textureSize;
		sf::FloatRect		m_subrect;

		sf::Color			m_color;

		sf::Transform		m_transform;

		TextureAsset*		m_textureAsset;
		ShaderAsset*		m_shaderAsset;

		sf::Texture*		m_texture;
		sf::Shader*			m_shader;

		sf::RenderStates	m_renderStates;

	};
	
}

#endif // SZEN_RENDERER_HPP
