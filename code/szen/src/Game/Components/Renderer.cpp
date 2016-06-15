#include <szen/Game/Components/Renderer.hpp>

#include <szen/System/AssetManager.hpp>
#include <szen/System/Assets/TextureAsset.hpp>
#include <szen/System/Assets/ShaderAsset.hpp>

#include <szen/System/SpriteBatch.hpp>

#include <szen/Game/Components/Transform.hpp>

#include <szen/Game/Camera.hpp>

using namespace sz;

////////////////////////////////////////////////////
Renderer::Renderer() :
	m_texture			(NULL),
	m_shader			(NULL),
	m_textureAsset		(NULL),
	m_shaderAsset		(NULL),
	m_color				(sf::Color::White),
	m_isPrerendering	(false)
{
	m_componentOrder = 40;
}

////////////////////////////////////////////////////
Renderer::Renderer(const std::string &textureID, const std::string &shaderID) :
	m_texture			(NULL),
	m_shader			(NULL),
	m_textureAsset		(NULL),
	m_shaderAsset		(NULL),
	m_color				(sf::Color::White),
	m_isPrerendering	(false)
{
	m_componentOrder = 40;

	setTexture(textureID);

	if(!shaderID.empty())
	{
		setShader(shaderID);
	}
}

////////////////////////////////////////////////////
Renderer::~Renderer()
{
	
}

////////////////////////////////////////////////////
sf::Shader* Renderer::getShader()
{
	return m_shader;
}

////////////////////////////////////////////////////
void Renderer::attached()
{
	//m_componentOrder = Order::Renderer;

	m_entity->addDrawCallback(&Renderer::draw);
}

////////////////////////////////////////////////////
void Renderer::parsePrefab(json::Value& val)
{
	if(val.isMember("texture"))
	{
		setTexture(val["texture"].asString());
	}

	if(val.isMember("color"))
	{
		sf::Uint32 size = val["color"].size();
		if(size == 3 || size == 4)
		{
			m_color.r = static_cast<sf::Uint8>(val["color"][0U].asUInt());
			m_color.g = static_cast<sf::Uint8>(val["color"][1U].asUInt());
			m_color.b = static_cast<sf::Uint8>(val["color"][2U].asUInt());

			if(size == 4)
			{
				m_color.a = static_cast<sf::Uint8>(val["color"][3U].asUInt());
			}
		}
	}

	if(val.isMember("opacity") && val["opacity"].isConvertibleTo(json::realValue))
	{
		float o = (float)val["opacity"].asDouble() * 255.f;
		m_color.a = static_cast<sf::Uint8>(std::min(255.f, std::max(0.f, o)));
	}

	if(val.isMember("shader"))
	{
		json::Value shader = val["shader"];

		if(shader.isConvertibleTo(json::stringValue))
		{
			setShader(shader.asString());
		}
		else if(shader.isObject())
		{
			setShader(shader["asset"].asString());

			if(shader.isMember("param") && !shader["param"].empty())
			{
				json::Value parameters = shader["param"];

				for(json::Value::iterator it = parameters.begin(); it != parameters.end(); ++it)
				{
					const std::string name = it.memberName();

					json::Value v = *it;
					if(v.isArray())
					{
						switch(v.size())
						{
						case 1:
							m_shader->setParameter(name,
								(float)v[0U].asDouble());
						break;

						case 2:
							m_shader->setParameter(name,
								(float)v[0U].asDouble(),
								(float)v[1U].asDouble());
						break;

						case 3:
							m_shader->setParameter(name,
								(float)v[0U].asDouble(),
								(float)v[1U].asDouble(),
								(float)v[2U].asDouble());
						break;

						case 4:
						{
							m_shader->setParameter(name,
								(float)v[0U].asDouble(),
								(float)v[1U].asDouble(),
								(float)v[2U].asDouble(),
								(float)v[3U].asDouble());
						break;
						}

						default:
							szerr << "Incorrect amount of parameter arguments in prefab." << ErrorStream::error;
							break;
						}
					}
					else if(v.isConvertibleTo(json::realValue))
					{
						m_shader->setParameter(name,
							(float)v.asDouble());
					}
				}
			}
		}
	}

	const std::string blend = val.get("blendmode", "alpha").asString();

	if(blend == "alpha")
	{
		m_renderStates.blendMode = sf::BlendAlpha;
	}
	else if(blend == "additive")
	{
		m_renderStates.blendMode = sf::BlendAdd;
	}
	else if(blend == "multiply")
	{
		m_renderStates.blendMode = sf::BlendMultiply;
	}
	else if(blend == "none")
	{
		m_renderStates.blendMode = sf::BlendNone;
	}
}

////////////////////////////////////////////////////
void Renderer::update()
{
	if(m_shader)
	{
		m_shader->setParameter("u_time", m_shaderClock.getElapsedTime().asSeconds());
	}

	Transform* t = m_entity->getComponent<Transform>();

	t->getMatrix(m_transform);

	//m_transform.scale(sf::Vector2f(m_subrect.width, m_subrect.height));
}

////////////////////////////////////////////////////
void Renderer::setPrerender(sf::Uint32 w, sf::Uint32 h)
{
	m_isPrerendering = true;

	m_prerenderSize.x = w;
	m_prerenderSize.y = h;

	m_prerenderTexture.create(w, h);
	m_prerenderTexture.clear(sf::Color::Transparent);
	
	m_prerendered = &m_prerenderTexture.getTexture();
	const_cast<sf::Texture*>(m_prerendered)->setSmooth(true);
}

////////////////////////////////////////////////////
void Renderer::setTexture(const std::string &assetID)
{
	TextureAsset* asset = Asset::getTexture(assetID);

	if(asset)
	{
		m_textureAsset = asset;
		m_texture = asset->getAsset();

		m_textureSize = m_texture->getSize();
		m_subrect = sf::FloatRect(0.f, 0.f, (float)m_textureSize.x, (float)m_textureSize.y);

		// Set sprite's origin point in the center
		//m_sprite->setOrigin(m_size.x / 2.f, m_size.y / 2.f);

		//m_vertexArray.resize(4);
		//m_vertexArray.setPrimitiveType(sf::Quads);
	}
	else
	{
		szerr << "Unable to set renderer texture! Asset ID: " << assetID << ErrorStream::error;
	}
}

////////////////////////////////////////////////////
void Renderer::setShader(const std::string &assetID)
{
	ShaderAsset* asset = Asset::getShader(assetID);

	if(asset)
	{
		m_shaderAsset = asset;
		m_shader = asset->getAsset();
	}
}

////////////////////////////////////////////////////
void Renderer::setBlendMode(const sf::BlendMode mode)
{
	m_renderStates.blendMode = mode;
}

////////////////////////////////////////////////////
void Renderer::setColor(const sf::Color& color)
{
	m_color = color;
}

////////////////////////////////////////////////////
void Renderer::setOpacity(float alpha)
{
	m_color.a = static_cast<sf::Uint8>(std::min(255.f, std::max(0.f, alpha * 255.f)));
}

////////////////////////////////////////////////////
sf::Color Renderer::getColor()
{
	return m_color;
}

////////////////////////////////////////////////////
void Renderer::setTextureRect(sf::IntRect& rect)
{
	m_subrect = static_cast<sf::FloatRect>(rect);
}

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
////////////////////////////////////////////////////
void Renderer::draw(sf::RenderTarget& target)
{
	auto vert = sf::Vector2f(m_subrect.width, m_subrect.height) * 0.5f;

	sf::Vertex quad[4];
	quad[0] = sf::Vertex(m_transform.transformPoint(-vert.x, -vert.y), m_color, sf::Vector2f(m_subrect.left, m_subrect.top));
	quad[1] = sf::Vertex(m_transform.transformPoint( vert.x, -vert.y), m_color, sf::Vector2f(m_subrect.left + m_subrect.width, m_subrect.top));
	quad[2] = sf::Vertex(m_transform.transformPoint( vert.x,  vert.y), m_color, sf::Vector2f(m_subrect.left + m_subrect.width, m_subrect.top + m_subrect.height));
	quad[3] = sf::Vertex(m_transform.transformPoint(-vert.x,  vert.y), m_color, sf::Vector2f(m_subrect.left, m_subrect.top + m_subrect.height));

	sf::Vector2f vmin, vmax;

	vmin.x = quad[0].position.x;
	vmin.y = quad[0].position.y;
	vmax.x = quad[0].position.x;
	vmax.y = quad[0].position.y;

	for(int i=1; i < 4; ++i)
	{
		vmin.x = std::min(quad[i].position.x, vmin.x);
		vmin.y = std::min(quad[i].position.y, vmin.y);
		vmax.x = std::max(quad[i].position.x, vmax.x);
		vmax.y = std::max(quad[i].position.y, vmax.y);
	}

	sf::FloatRect bounds(vmin.x, vmin.y, vmax.x - vmin.x, vmax.y - vmin.y);

	sf::FloatRect cbounds = sz::Camera::getCameraBounds();
	
	if(m_entity->getLayer() >= EntityLayer::Interface || bounds.intersects(cbounds))
	{
		if(m_isPrerendering)
		{
			sf::Vector2f v = static_cast<sf::Vector2f>(m_prerenderSize);
			sf::Vector2f t = static_cast<sf::Vector2f>(m_textureSize);

			sf::VertexArray va(sf::Quads, 4);
			va[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
			va[1] = sf::Vertex(sf::Vector2f(v.x, 0.f), sf::Vector2f(t.x, 0.f));
			va[2] = sf::Vertex(sf::Vector2f(v.x, v.y), sf::Vector2f(t.x, t.y));
			va[3] = sf::Vertex(sf::Vector2f(0.f, v.y), sf::Vector2f(0.f, t.y));

			sf::RenderStates rs;
			rs.texture = m_texture;
			rs.shader = m_shader;

			m_prerenderTexture.clear(sf::Color::Transparent);
			m_prerenderTexture.draw(va, rs);
			m_prerenderTexture.display();

			sf::Vector2f rel(v.x / t.x, v.y / t.y);

			quad[0].texCoords.x *= rel.x;
			quad[0].texCoords.y *= rel.y;
			quad[1].texCoords.x *= rel.x;
			quad[1].texCoords.y *= rel.y;
			quad[2].texCoords.x *= rel.x;
			quad[2].texCoords.y *= rel.y;
			quad[3].texCoords.x *= rel.x;
			quad[3].texCoords.y *= rel.y;

			SpriteBatch::append(
				quad, m_entity->getLayer(),
				m_prerendered, NULL, m_renderStates.blendMode);
		}
		else
		{
			SpriteBatch::append(
				quad, m_entity->getLayer(),
				m_texture, m_shaderAsset, m_renderStates.blendMode);
		}
	}
}

////////////////////////////////////////////////////
sf::Vector2u Renderer::getTextureSize() const
{
	return m_textureSize;
}