#ifndef SZEN_PHYSICS_HPP
#define SZEN_PHYSICS_HPP

#include <szen/Game/Component.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

#include <Box2D/Box2D.h>
#include <szen/Game/PhysicsContact.hpp>

namespace sz
{
	
	class Physics : public Component
	{
	public:
		Physics();
		~Physics();
		
		void attached();
		void update();
		void parsePrefab(json::Value&);

		void setRotation(float a);
		void setPosition(sf::Vector2f v);

		sf::Vector2f getLinearVelocity();
		void setLinearVelocity(sf::Vector2f v);

		float getAngularVelocity();
		void setAngularVelocity(float w);
		void applyTorque(float torque);

		float getLinearDamping();
		void setLinearDamping(float v);

		void setSpeedLimit(float);

		inline b2Body* getBody() { return m_body; }

		enum BodyType
		{
			Static		= b2_staticBody,
			Dynamic		= b2_dynamicBody,
			Kinematic	= b2_kinematicBody,
		};

		// Creates a box body
		void createBox(sf::Vector2f size, BodyType type = Dynamic, bool isBullet = false);
		void createBox(float sx, float sy, BodyType type = Dynamic, bool isBullet = false);

		// Creates a circle body
		void createCircle(float radius, BodyType type = Dynamic, bool isBullet = false);

		// Creates a body from vertices
		void createPolygon(std::vector<sf::Vector2f> vertices, BodyType type = Dynamic, bool isBullet = false);

		// Set's rest of the body's properties
		void finalizeBody(float friction, float restitution, float density);

		// Apply a linear impulse force in certain direction
		void linearImpulse(float angleRadians, float force);
		// Apply a linear impulse force
		void linearImpulse(sf::Vector2f& force);

		// Directly accelerate velocity in direction
		void accelerate(float angleRadians, float amount);
		// Directly accelerate velocity
		void accelerate(sf::Vector2f& amount);

		// Directly brake velocity
		void brake(float intensity);

		std::vector<Entity*> queryRadius(float radius, uint32 limit = 0);

		static std::vector<Entity*> queryAABB(sf::Vector2f& point, sf::Vector2f& rect, uint32 limit = 0);
		static std::vector<Entity*> queryRadius(sf::Vector2f& point, float radius, uint32 limit = 0);

	protected:
		
		float		m_speedLimit;

		float		m_worldScale;

		b2Body*		m_body;
		b2World*	p_world;

		b2BodyDef		m_bodyDef;

		struct b2ShapeStruct
		{
			b2PolygonShape	polygon;
			b2CircleShape	circle;
		} m_bodyShape;

		b2FixtureDef	m_fixtureDef;

	private:

		void _createBody(BodyType type, const bool isBullet);

	};
	
}

#endif // SZEN_PHYSICS_HPP
