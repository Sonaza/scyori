#ifndef GAME_ACTORENTITYBASE_HPP
#define GAME_ACTORENTITYBASE_HPP

#include <szen/szen.hpp>
#include <game/GameGlobals.hpp>

#include <type_traits>

#include <thor/Math.hpp>

class BulletEntityBase;

class ActorEntityBase : public sz::Entity
{
public:
	ActorEntityBase();
	~ActorEntityBase();

	enum Type
	{
		Unknown = -1,
		Player	= 0,
		Friend	= 1,
		Enemy	= 2,
		Planet	= 3
	};

	Type			m_type;

protected:

	template <class Type>
	void shoot(float angle, float offset, float velocity, typename std::enable_if< std::is_base_of<BulletEntityBase, Type>::value >::type* = 0)
	{
		Type* bullet = sz::World::spawn<Type>();
		shoot(bullet, angle, offset, velocity);
	}

	GameGlobalsPtr	p_gameGlobals;

private:

	void shoot(BulletEntityBase* bullet, float angle, float offset, float velocity);
	
};

#endif // GAME_ACTORENTITYBASE_HPP
