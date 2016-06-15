#ifndef GAME_BULLETENTITYBASE_HPP
#define GAME_BULLETENTITYBASE_HPP

#include <szen/szen.hpp>

class BulletEntityBase : public sz::Entity
{
public:
	BulletEntityBase();
	virtual ~BulletEntityBase();

	void begincontact(sz::PhysicsContact&);
	void endcontact(sz::PhysicsContact&);

	bool collisionCallback(sz::PhysicsContact&);

	void update();

	virtual void derivedUpdate() =0;

	virtual bool onImpact(Entity* hit) =0;

	void setDamage();
	float getDamage();

protected:

	friend class EnemyEntityBase;
	friend class ActorEntityBase;

	void shoot(sf::Vector2f position, float angle, float offset, float velocity);

	float		m_damage;

	Entity*		m_shooter;
	float		m_speed;

	float		m_angle;

private:

	sf::Vector2f m_direction;

};

#endif // GAME_BULLETENTITYBASE_HPP
