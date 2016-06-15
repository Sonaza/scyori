#ifndef SZEN_TRANSFORM_HPP
#define SZEN_TRANSFORM_HPP

#include <cassert>

#include <szen/Game/Component.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace sz
{

	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		void			attached();
		void			parsePrefab(json::Value&);

		void			setPosition(sf::Vector2f v);
		void			setPosition(float x, float y);
		sf::Vector2f	getPosition();

		void			move(sf::Vector2f v);
		void			move(float x, float y);

		void			setPivot(sf::Vector2f v);
		void			setPivot(float x, float y);
		sf::Vector2f	getPivot();

		void			setDepth(float z);
		float			getDepth();
		
		void			setRotation(float a);
		float			getRotation();

		void			setScale(sf::Vector2f v);
		void			setScale(float x, float y);
		sf::Vector2f	getScale();

		void			setAnchor(float x, float y);
		sf::Vector2f	getAnchor();
		sf::Vector2f	getAnchorDelta();

		void			getMatrix(sf::Transform&);

		// Interface elements only
		float			getRelativeScale();

		void			setInheritPosition(bool);
		void			setInheritRotation(bool);
		void			setInheritScale(bool);

	private:

		struct Inheritances
		{
			Inheritances() : position(true), rotation(true), scale(true) {}

			bool position;
			bool rotation;
			bool scale;
		} m_inheritances;

		sf::Vector3f	m_position;
		sf::Vector2f	m_pivot;
		float			m_rotation;
		sf::Vector2f	m_scale;

		bool			m_isAnchored;
		sf::Vector2f	m_anchor;
		float			m_nativeWidth;

		sf::Transform	m_matrix;

		float			m_relativeScale;

	};

}

#endif // SZEN_TRANSFORM_HPP
