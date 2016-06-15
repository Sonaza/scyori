#include <szen/Game/Components/TextRenderer.hpp>

#include <szen/System/SpriteBatch.hpp>
#include <szen/System/AssetManager.hpp>
#include <szen/System/Assets/FontAsset.hpp>

#include <szen/Game/Components/Transform.hpp>

using namespace sz;

////////////////////////////////////////////////////
TextRenderer::TextRenderer()
{
	m_text.setColor(sf::Color::White);
	m_text.setCharacterSize(30U);

	m_componentOrder = 45;
}

////////////////////////////////////////////////////
TextRenderer::~TextRenderer()
{
	
}

////////////////////////////////////////////////////
void TextRenderer::attached()
{
	m_entity->addDrawCallback(&TextRenderer::draw);
}

////////////////////////////////////////////////////
void TextRenderer::update()
{
	auto t = m_entity->getComponent<Transform>();

	//float time = clockan.getElapsedTime().asSeconds();
	//float s = cos(time) * 0.25f + 1.25f;
	//t->setScale(s, s);

	auto position = t->getPosition() * t->getRelativeScale();

	if(m_entity->getLayer() >= EntityLayer::Interface)
	{
		position += t->getAnchorDelta();
	}

	auto pivot = t->getPivot();

	auto bounds = m_text.getLocalBounds();
	m_origin = sf::Vector2f(
		bounds.width - bounds.left,
		bounds.height - bounds.top
	);

	m_origin.x += pivot.x * m_origin.y;
	m_origin.y += pivot.y * m_origin.y;

	m_origin.x = m_origin.x * 0.5f + pivot.x * m_origin.x;
	m_origin.y = m_origin.y * 0.5f + pivot.y * 2.f * m_origin.y;

	m_text.setOrigin(m_origin);
	m_text.setRotation(t->getRotation());
	m_text.setPosition(position);
	m_text.setScale(t->getScale());
}

////////////////////////////////////////////////////
void TextRenderer::parsePrefab(json::Value& val)
{
	if(val.isMember("font") && val["font"].isString())
	{
		setFont(val["font"].asString());
	}

	if(val.isMember("string") && val["string"].isString())
	{
		setText(val["string"].asString());
	}

	if(val.isMember("size") && val["size"].isConvertibleTo(json::uintValue))
	{
		setSize(
			static_cast<size_t>(val["size"].asDouble())
		);
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
		float o = static_cast<float>(val["opacity"].asDouble()) * 255.f;
		m_color.a = static_cast<sf::Uint8>(std::min(255.f, std::max(0.f, o)));
	}

	const std::string blend = val.get("blendmode", "alpha").asString();

	if(blend == "alpha")
	{
		m_blendmode = sf::BlendAlpha;
	}
	else if(blend == "additive")
	{
		m_blendmode = sf::BlendAdd;
	}
	else if(blend == "multiply")
	{
		m_blendmode = sf::BlendMultiply;
	}
	else if(blend == "none")
	{
		m_blendmode = sf::BlendNone;
	}
}

////////////////////////////////////////////////////
void TextRenderer::setFont(const std::string &assetID)
{
	FontAsset* asset = Asset::getFont(assetID);
	if(asset)
	{
		m_font = asset->getAsset();
		m_text.setFont(*m_font);
	}
}

////////////////////////////////////////////////////
void TextRenderer::setSize(const size_t size)
{
	m_size = static_cast<size_t>(size * m_entity->getComponent<Transform>()->getRelativeScale());
	m_text.setCharacterSize(m_size);
}

////////////////////////////////////////////////////
void TextRenderer::setBlendMode(const sf::BlendMode mode)
{
	m_blendmode = mode;
}

////////////////////////////////////////////////////
void TextRenderer::setColor(const sf::Color& color)
{
	m_color = color;
}

////////////////////////////////////////////////////
void TextRenderer::setOpacity(float a)
{
	a = std::min(255.f, std::max(0.f, a * 255.f));
	m_color.a = static_cast<sf::Uint8>(a);
}

////////////////////////////////////////////////////
void TextRenderer::setText(const std::string &text)
{
	m_string = text;
	m_text.setString(m_string);
}

////////////////////////////////////////////////////
std::string TextRenderer::getText() const
{
	return m_string;
}

////////////////////////////////////////////////////
void TextRenderer::draw(sf::RenderTarget&)
{
	auto pos = m_text.getPosition();

	float relativeScale = m_entity->getComponent<Transform>()->getRelativeScale();

	m_text.setColor(sf::Color(
		(sf::Uint8)(m_color.r * 0.12f),
		(sf::Uint8)(m_color.g * 0.12f),
		(sf::Uint8)(m_color.b * 0.12f),
		(sf::Uint8)(240 * m_color.a / 255.f)));
	m_text.setPosition(pos + sf::Vector2f(1.f, 3.f) * relativeScale);
	SpriteBatch::append(&m_text, m_entity->getLayer(), m_blendmode);

	m_text.setColor(m_color);
	m_text.setPosition(pos);
	SpriteBatch::append(&m_text, m_entity->getLayer(), m_blendmode);
}