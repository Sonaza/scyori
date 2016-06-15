#ifndef SZEN_SPRITEBATCH_HPP
#define SZEN_SPRITEBATCH_HPP

#include <SFML/Graphics.hpp>

#include <functional>
#include <map>
#include <vector>

#include <thor/Particles.hpp>

namespace sz
{

	class TextureAsset;
	class ShaderAsset;

	struct BatchData
	{	
		BatchData() : texture(NULL), shader(NULL), particles(NULL) {}
		BatchData(size_t layer, const sf::Texture*, const sf::Shader*, sf::BlendMode);
		BatchData(size_t layer, thor::ParticleSystem*, const sf::Shader*, sf::BlendMode);
		BatchData(size_t layer, sf::Text*, sf::BlendMode);

		size_t layer;

		sf::VertexArray vertices;
		const sf::Texture*	texture;
		const sf::Shader*	shader;
		sf::BlendMode	blendmode;

		thor::ParticleSystem* particles;

		sf::Text		text;

		enum Type
		{
			Vertices,
			Particles,
			Text
		} type;
	};

	typedef std::vector<BatchData> BatchDataList;

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		static void init();

		static void clear();
		//static void append(const sf::Vertex* quad, sf::Uint32 layer, TextureAsset* texture, ShaderAsset* shader, sf::BlendMode blendmode);
		static void append(const sf::Vertex* quad, sf::Uint32 layer, const sf::Texture* texture, ShaderAsset* shader, sf::BlendMode blendmode);
		static void append(thor::ParticleSystem* system, sf::Uint32 layer, ShaderAsset* shader, sf::BlendMode blendmode);

		static void append(sf::Text* text, sf::Uint32 layer, sf::BlendMode blendmode);

		static void draw(sf::RenderTarget&);

	private:

		static BatchDataList	m_batchData;

	};
	
}

#endif // SZEN_SPRITEBATCH_HPP
