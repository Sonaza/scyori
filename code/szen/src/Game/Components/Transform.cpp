#include <szen/Game/Components/Transform.hpp>

//#include <szen/Game/Components/Physics.hpp>

#include <algorithm>
#include <szen/Game/Camera.hpp>
#include <szen/System/Window.hpp>

using namespace sz;

////////////////////////////////////////////////////
Transform::Transform() :
	m_position		(0.f, 0.f, 0.f),
	m_pivot			(0.f, 0.f),
	m_rotation		(0.f),
	m_anchor		(0.f, 0.f),
	m_scale			(1.f, 1.f),

	m_nativeWidth	(0.f),
	m_isAnchored	(false)
{
	m_componentOrder = 5;
}

////////////////////////////////////////////////////
Transform::~Transform()
{
	
}

////////////////////////////////////////////////////
void Transform::attached()
{
	
}

////////////////////////////////////////////////////
void Transform::parsePrefab(json::Value& val)
{
	if(val.isMember("position") && val["position"].isArray() && (val["position"].size() == 2 || val["position"].size() == 3))
	{
		m_position.x = static_cast<float>(val["position"][0U].asDouble());
		m_position.y = static_cast<float>(val["position"][1U].asDouble());

		// Set depth if it is defined
		if(val["position"].size() == 3)
		{
			m_position.z = static_cast<float>(val["position"][2U].asDouble());
		}
	}

	if(val.isMember("depth"))
	{
		json::Value depth = val["depth"];

		if(depth.isArray() && depth.size() == 1)
		{
			m_position.z = static_cast<float>(depth[0U].asDouble());
		}
		else if(depth.isConvertibleTo(json::realValue))
		{
			m_position.z = static_cast<float>(depth.asDouble());
		}
	}

	if(val.isMember("pivot") && val["pivot"].isArray() && val["pivot"].size() == 2)
	{
		m_pivot.x = static_cast<float>(val["pivot"][0U].asDouble());
		m_pivot.y = static_cast<float>(val["pivot"][1U].asDouble());
	}

	if(val.isMember("rotation"))
	{
		json::Value rotation = val["rotation"];

		if(rotation.isArray() && rotation.size() == 1)
		{
			m_rotation = static_cast<float>(rotation[0U].asDouble());
		}
		else if(rotation.isConvertibleTo(json::realValue))
		{
			m_rotation = static_cast<float>(rotation.asDouble());
		}
	}

	if(val.isMember("scale") && val["scale"].isArray() && val["scale"].size() == 2)
	{
		m_scale.x = static_cast<float>(val["scale"][0U].asDouble());
		m_scale.y = static_cast<float>(val["scale"][1U].asDouble());
	}

	if(val.isMember("anchor") && val["anchor"].isArray() && val["anchor"].size() == 2)// && m_entity->getLayer() >= EntityLayer::Interface)
	{
		std::map<std::string, float> point;

		point["left"]	= -0.5f;
		point["right"]	=  0.5f;

		point["top"]	= -0.5f;
		point["bottom"]	=  0.5f;

		point["center"]	=  0.0f;

		//const std::string shasd = val["anchor"][2U].asString();

		setAnchor(
			point[val["anchor"][0U].asString()],
			point[val["anchor"][1U].asString()]
		);

		if(val.isMember("native") && val["native"].isConvertibleTo(json::realValue))
		{
			m_nativeWidth = static_cast<float>(val["native"].asDouble());
		}
		else
		{
			m_nativeWidth = static_cast<float>(Window::getSize().x);
		}
	}
}

////////////////////////////////////////////////////
void Transform::setPosition(sf::Vector2f v)
{
	m_position.x = v.x;
	m_position.y = -v.y;
}

////////////////////////////////////////////////////
void Transform::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

////////////////////////////////////////////////////
void Transform::move(sf::Vector2f v)
{
	m_position.x += v.x;
	m_position.y += v.y;
}

////////////////////////////////////////////////////
void Transform::move(float x, float y)
{
	move(sf::Vector2f(x, y));
}

////////////////////////////////////////////////////
sf::Vector2f Transform::getPosition()
{
	sf::Vector2f result(m_position.x, m_position.y);

	Entity* parent = m_entity->getParent();
	if(m_inheritances.position && parent != NULL) result += parent->call(&Transform::getPosition);

	return result;
}

////////////////////////////////////////////////////
void Transform::setPivot(sf::Vector2f v)
{
	m_pivot = v;
}

////////////////////////////////////////////////////
void Transform::setPivot(float x, float y)
{
	setPivot(sf::Vector2f(x, y));
}

////////////////////////////////////////////////////
sf::Vector2f Transform::getPivot()
{
	return m_pivot;
}

////////////////////////////////////////////////////
void Transform::setDepth(float z)
{
	m_position.z = z;
}

////////////////////////////////////////////////////
float Transform::getDepth()
{
	return m_position.z;
}

////////////////////////////////////////////////////
void Transform::setRotation(float a)
{
	m_rotation = a;
}

////////////////////////////////////////////////////
float Transform::getRotation()
{
	float result = m_rotation;

	Entity* parent = m_entity->getParent();
	if(m_inheritances.rotation && parent != NULL) result += parent->call(&Transform::getRotation);

	return result;
}

////////////////////////////////////////////////////
void Transform::setScale(sf::Vector2f v)
{
	m_scale = v;
}

////////////////////////////////////////////////////
void Transform::setScale(float x, float y)
{
	setScale(sf::Vector2f(x, y));
}

namespace
{

	////////////////////////////////////////////////////
	sf::Vector2f operator*(sf::Vector2f& lhs, sf::Vector2f& rhs)
	{
		return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	////////////////////////////////////////////////////
	sf::Vector2f operator*=(sf::Vector2f& lhs, sf::Vector2f& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		return lhs;
	}

}

////////////////////////////////////////////////////
sf::Vector2f Transform::getScale()
{
	sf::Vector2f result = m_scale;

	sf::Vector2f herp;

	Entity* parent = m_entity->getParent();
	if(m_inheritances.scale && parent != NULL)
	{
		result *= parent->getComponent<Transform>()->getScale();
		//result.x *= scale.x;
		//result.y *= scale.y;
	}

	/*if(parent != NULL && parent->getTag() == "station_anchor")
	{
		std::cout << result.x;
	}*/

	return result;
}

////////////////////////////////////////////////////
void Transform::setAnchor(float x, float y)
{
	// Cannot anchor non-interface elements
	if(m_entity->getLayer() < EntityLayer::Interface) return;

	m_anchor.x = x;
	m_anchor.y = y;

	m_isAnchored = true;
}

////////////////////////////////////////////////////
sf::Vector2f Transform::getAnchor()
{
	return m_anchor;
}

////////////////////////////////////////////////////
sf::Vector2f Transform::getAnchorDelta()
{
	sf::Vector2f screen = static_cast<sf::Vector2f>(Window::getSize());
	return m_anchor * screen + screen * 0.5f;
}

////////////////////////////////////////////////////
void Transform::getMatrix(sf::Transform& matrix)
{
	sf::Vector2f position = getPosition();
	if(m_entity->getLayer() >= EntityLayer::Interface) position *= getRelativeScale();

	position -= Camera::getCameraOffset() * std::min(50.f, m_position.z) / 51.f;

	matrix = sf::Transform::Identity;

	float scaleModifier = 1.f;

	if(m_isAnchored)
	{
		matrix.translate(getAnchorDelta());
		scaleModifier = static_cast<float>(Window::getSize().x) / m_nativeWidth;
	}

	matrix.translate(position);
	matrix.rotate(getRotation());
	matrix.scale(getScale() * scaleModifier);
	matrix.translate(-getPivot());
}

////////////////////////////////////////////////////
float Transform::getRelativeScale()
{
	return static_cast<float>(Window::getSize().x) / m_nativeWidth;
}

////////////////////////////////////////////////////
void Transform::setInheritPosition(bool v)
{
	m_inheritances.position = v;
}

////////////////////////////////////////////////////
void Transform::setInheritRotation(bool v)
{
	m_inheritances.rotation = v;
}

////////////////////////////////////////////////////
void Transform::setInheritScale(bool v)
{
	m_inheritances.scale = v;
}