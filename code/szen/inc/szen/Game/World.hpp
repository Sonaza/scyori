#ifndef SZEN_WORLD_HPP
#define SZEN_WORLD_HPP

#include <vector>
#include <cassert>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <szen/Game/Entity.hpp>
#include <szen/Game/PhysicsContact.hpp>

namespace sz
{
	
	class Core;

	typedef std::vector<Entity*> EntityList;

	class WorldContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

		//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	};

	class WorldContactFilter : public b2ContactFilter
	{
	public:
		bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	};

	class World
	{
	public:
		static void init(float gx, float gy);
		static void init(const sf::Vector2f &gravity);
		static void uninit();

		static void setWindow(sf::RenderWindow &);

		static void setProperties(const sf::Vector2f &gravity);
		static void setProperties(float timeStep, sf::Int32 velocityIterations = 0, sf::Int32 positionIterations = 0);

		static float getWorldScale();
		static b2World* getBox2DWorld();

		//static void callGlobal(const std::string &type, boost::any data);
		//static void callGlobal(const Message &msg);

		// Clear all entities
		static void clearEntities();

		// World update which updates physics and entities
		static void updateWorld();

		//static void ute(EntityList::iterator b, EntityList::iterator e);

		// Draws world entities
		static void renderWorld();
		// Draws interface entities
		static void renderInterface();

		static size_t getEntityCount();

		static bool exists(Entity*);

		template<class T>
		static void destroyAllOf();

		/////////////////////////////////
		// Entity creation

		template <class T>
		static T* spawn();

		template <class T, class Arg1>
		static T* spawn(Arg1&& a1);

		template <class T, class Arg1, class Arg2>
		static T* spawn(Arg1&& a1, Arg2&& a2);

		template <class T, class Arg1, class Arg2, class Arg3>
		static T* spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3);

		template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
		static T* spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4);

		template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
		static T* spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5);

		template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
		static T* spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5, Arg6&& a6);
		
		static Entity* findWithTag(const std::string& tag);

		static std::vector<Entity*> getChildren(Entity*);

		static void destroyChildren(Entity*);

	protected:

		friend class Entity;
		static bool m_pendingEntitySort;

	private:

		static void sortEntities();
		static void addPendingEntities(const bool update = true);

		static EntityList	m_entities;
		static EntityList	m_pendingEntities;

		// Pointer to box 2d world instance
		static b2World*		m_world;

		// Physics time step amount
		static float		m_physicsStep;
		// Amount of velocity iterations per step
		static sf::Int32	m_velIterations;
		// Amount of position iterations per step
		static sf::Int32	m_posIterations;

		// Physics world scale or how many pixels per "meter"
		static float		m_worldScale;

		template <class T>
		static T* addEntity(T* e);

	};

	//////////////////////////////////////////
	template <class T>
	static T* World::addEntity(T* e)
	{
		assert(e);

		m_pendingEntities.push_back(e);
		m_pendingEntitySort = true;

		e->onLoad();

		return e;
	}

	//////////////////////////////////////////
	template <class T>
	T* World::spawn()
	{
		T* e = new T;
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T, class Arg1>
	T* World::spawn(Arg1&& a1)
	{
		T* e = new T(a1);
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T, class Arg1, class Arg2>
	T* World::spawn(Arg1&& a1, Arg2&& a2)
	{
		T* e = new T(a1, a2);
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3>
	T* World::spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3)
	{
		T* e = new T(a1, a2, a3);
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
	T* World::spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4)
	{
		T* e = new T(a1, a2, a3, a4);
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	T* World::spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5)
	{
		T* e = new T(a1, a2, a3, a4, a5);
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	T* World::spawn(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5, Arg6&& a6)
	{
		T* e = new T(a1, a2, a3, a4, a5, a6);
		return addEntity(e);
	}

	//////////////////////////////////////////
	template <class T>
	void World::destroyAllOf()
	{
		for(EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
		{
			T* e = dynamic_cast<T*>(*it);
			if(e)
			{
				e->destroyEntity();
			}
		}
	}
	
}

#endif // SZEN_WORLD_HPP
