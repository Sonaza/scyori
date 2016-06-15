#ifndef SZEN_ENTITY_HPP
#define SZEN_ENTITY_HPP

#include <map>
#include <vector>
#include <functional>
#include <cassert>

#include <SFML/Graphics.hpp>

#include <szen/System/ErrorStream.hpp>
#include <szen/Game/ComponentType.hpp>

#include <type_traits>

#include <szen/Game/EntityLayer.hpp>
#include <szen/Game/PhysicsContact.hpp>

#include <szen/System/PausableClock.hpp>

class b2Contact;

namespace sz
{

	class Component;
	class World;

	typedef std::map<ComponentType*, Component*>	ComponentList;
	typedef std::vector<Component*>					SortedComponentList;

	typedef std::function<void(sf::RenderTarget&)>	DrawFunction;
	typedef std::function<void(PhysicsContact&)>	PhysicsContactFunction;
	typedef std::function<bool(PhysicsContact&)>	CollisionFilterFunction;

	class Entity : public sf::Drawable
	{
		friend class World;
		friend class WorldContactListener;
		friend class WorldContactFilter;

	public:
		Entity();
		virtual ~Entity();

		bool loadFromPrefab(const std::string& path);

		// Overload of sf::Drawable draw function
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Sets a draw callback for entity and binds it to the correct component
		template <class Class>
		void addDrawCallback(void (Class::*f)(sf::RenderTarget&));

		///////////////////////////////////////
		// Component related functionality

		// Check by type if a component has been attached
		template <class T>
		bool hasComponent();

		bool hasComponents() const;

		// Retrieve component by type
		template <class T>
		T* getComponent();

		// Updates all attached components
		void updateComponents();

		virtual void onLoad() {}

		// Update entity
		virtual void update() {}
		virtual void internalUpdate() {}

#pragma region Template functions

		template <class T>
		T* attach();

		template <class T, class Arg1>
		T* attach(Arg1&& a1);

		template <class T, class Arg1, class Arg2>
		T* attach(Arg1&& a1, Arg2&& a2);

		template <class T, class Arg1, class Arg2, class Arg3>
		T* attach(Arg1&& a1, Arg2&& a2, Arg3&& a3);

		template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
		T* attach(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4);

		template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
		T* attach(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5);

		template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
		T* attach(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5, Arg6&& a6);

		///////////////////////////////////////
		// Function callbacks

		/*template <class Class>
		std::function<void()> operator[](void (Class::*func)(void));

		template <class Class, class Arg1>
		std::function<void(Arg1)> operator[](void (Class::*func)(Arg1));*/

		template <class Class>
		void call(void (Class::*func)(void));

		template <class Class, class Arg1>
		void call(void (Class::*func)(Arg1), Arg1 a1);

		template <class Class, class Arg1, class Arg2>
		void call(void (Class::*func)(Arg1, Arg2), Arg1 a1, Arg2 a2);

		template <class Class, class Arg1, class Arg2, class Arg3>
		void call(void (Class::*func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3);

		template <class Class, class Arg1, class Arg2, class Arg3, class Arg4>
		void call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4);

		template <class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
		void call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5);

		template <class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
		void call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6);
		
		///////////////////////////////////////////////////
		// Templated request functions

		template <class Return, class Class>
		Return call(Return (Class::*func)(void));

		template <class Return, class Class, class Arg1>
		Return call(Return (Class::*func)(Arg1), Arg1 a1);

		template <class Return, class Class, class Arg1, class Arg2>
		Return call(Return (Class::*func)(Arg1, Arg2), Arg1 a1, Arg2 a2);

		template <class Return, class Class, class Arg1, class Arg2, class Arg3>
		Return call(Return (Class::*func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3);

		template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4>
		Return call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4);

		template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
		Return call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5);

		template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
		Return call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6);

#pragma endregion Template functions

		///////////////////////////////////////
		// Entity physics callbacks

		// Set a free callback function for begin contact
		void setBeginContactCallback(void (*f)(PhysicsContact&));

		// Set component callback for begin contact
		template <class Class>
		void setBeginContactCallback(void (Class::*f)(PhysicsContact&), typename std::enable_if< std::is_base_of<Component, Class>::value >::type* = 0);

		// Set entity callback for begin contact
		template <class Class>
		void setBeginContactCallback(void (Class::*f)(PhysicsContact&), typename std::enable_if< std::is_base_of<Entity, Class>::value >::type* = 0);

		// Set a free callback function for end contact
		void setEndContactCallback(void (*f)(PhysicsContact&));

		// Set component callback for end contact
		template <class Class>
		void setEndContactCallback(void (Class::*f)(PhysicsContact&), typename std::enable_if< std::is_base_of<Component, Class>::value >::type* = 0);

		// Set entity callback for end contact
		template <class Class>
		void setEndContactCallback(void (Class::*f)(PhysicsContact&), typename std::enable_if< std::is_base_of<Entity, Class>::value >::type* = 0);

		// Set entity callback for collision filter
		template <class Class>
		void setCollisionFilterCallback(bool (Class::*f)(PhysicsContact&), typename std::enable_if< std::is_base_of<Entity, Class>::value >::type* = 0);

		///////////////////////////////////////
		// Entity states

		// Toggle entity
		void setEnabled(bool enabled);

		// Returns if the entity is enabled or disabled
		bool isEnabled() const;

		// Sets entity up for destroying
		void destroyEntity();

		// Returns whether the entity is due for removal
		bool isDestroyed() const;

		// Sets entity tag
		void setTag(const std::string &);

		// Retrieves entity tag
		std::string	getTag() const;

		void setLayer(EntityLayer::Level layerlevel, EntityLayer::Id layerid = 0);
		EntityLayer::Id getLayer() const;

		void setParent(Entity*);
		Entity* getParent() const;

		bool hasParent();

	protected:

		// Physics function callbacks
		PhysicsContactFunction		m_beginContactCallback;
		PhysicsContactFunction		m_endContactCallback;

		CollisionFilterFunction		m_collisionFilterCallback;

		// Draw function callback
		std::vector<DrawFunction>	m_drawCallbacks;

		static bool layerSorter(Entity*, Entity*);

		struct CollisionFilter
		{
			CollisionFilter() : categoryBits(0x1), maskBits(0xFFFFFFFF) {}

			// Entity's own category
			sf::Uint32	categoryBits;
			// Categories that entity will collide with
			sf::Uint32	maskBits;
		} m_collisionfilter;

		struct Clocks
		{
			Clocks() : total(0.f), delta(0.f) {}

			float total;
			float delta;

			sf::Time totalTime;
			sf::Time deltaTime;
		} Time;

	private:

		bool				m_isParent;

		Entity*				m_parentEntity;

		std::string			m_tag;
		EntityLayer::Id		m_entityLayer;

		// True when entity is marked destroyed and will be deleted on the next loop run
		bool	m_isDestroyed;
		// True if entity is enabled
		bool	m_isEnabled;

		// List of components that have been attached to the entity
		ComponentList		m_components;
		SortedComponentList	m_sortedComponents;

		bool	m_pendingSort;

		// A private component attacher
		template <class T>
		T* attachComponent(T* c);

		void sortComponents();

		void updateClocks();

		PausableClock	m_totalTimeClock;
		PausableClock	m_deltaTimeClock;

	};

	/////////////////////////////////////////////
	template <class Class>
	void Entity::addDrawCallback(void (Class::*func)(sf::RenderTarget&))
	{
		// Check if callback has already been set
		//assert(!m_drawCallback);

		// Attempt to bind function pointer to the component instance
		if(Class* component = getComponent<Class>())
		{
			m_drawCallbacks.insert(m_drawCallbacks.end(), std::bind(func, component, std::placeholders::_1));
			//m_drawCallbacks.push_back(std::bind(func, component, std::placeholders::_1));
		}
	}

	/////////////////////////////////////////////
	template <class T>
	bool Entity::hasComponent()
	{
		// Attempt to find component by type
		return m_components.find(ComponentType::getTypeFor<T>()) != m_components.end();
	}

	/////////////////////////////////////////////
	template <class T>
	T* Entity::getComponent()
	{
		// Retrieve component pointer
		Component* c = m_components[ComponentType::getTypeFor<T>()];

		// If component pointer is valid, cast it to the requested type
		return c ? dynamic_cast<T*>(c) : NULL;
	}
	
	/////////////////////////////////////////////
	template <class T>
	T* Entity::attachComponent(T* component)
	{
		// Component must not be already attached
		assert(component->m_entity == NULL);

		// Set component's entity pointer 
		component->m_entity = this;

		// Insert new component to the component list
		m_components.insert(std::make_pair(
			ComponentType::getTypeFor<T>(), component
		));

		// Tell component it has been attached
		component->attached();

		m_pendingSort = true;

		return component;
	}

#pragma region Template Definitions

	/////////////////////////////////////////////
	template <class T>
	T* Entity::attach()
	{
		T* c = new T;
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class T, class Arg1>
	T* Entity::attach(Arg1&& a1)
	{
		T* c = new T(a1);
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class T, class Arg1, class Arg2>
	T* Entity::attach(Arg1&& a1, Arg2&& a2)
	{
		T* c = new T(a1, a2);
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3>
	T* Entity::attach(Arg1&& a1, Arg2&& a2, Arg3&& a3)
	{
		T* c = new T(a1, a2, a3);
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
	T* Entity::attach(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4)
	{
		T* c = new T(a1, a2, a3, a4);
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	T* Entity::attach(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5)
	{
		T* c = new T(a1, a2, a3, a4, a5);
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	T* Entity::attach(Arg1&& a1, Arg2&& a2, Arg3&& a3, Arg4&& a4, Arg5&& a5, Arg6&& a6)
	{
		T* c = new T(a1, a2, a3, a4, a5, a6);
		return attachComponent(c);
	}

	/////////////////////////////////////////////
	template <class Class>
	void Entity::call(void (Class::*func)())
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)();
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1>
	void Entity::call(void (Class::*func)(Arg1), Arg1 a1)
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)(a1);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2>
	void Entity::call(void (Class::*func)(Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)(a1, a2);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3>
	void Entity::call(void (Class::*func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)(a1, a2, a3);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3, class Arg4>
	void Entity::call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)(a1, a2, a3, a4);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	void Entity::call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)(a1, a2, a3, a4, a5);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	void Entity::call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		assert(hasComponent<Class>());
		(getComponent<Class>()->*func)(a1, a2, a3, a4, a5, a6);
	}

	///////////////////////////////////////////////////
	// Templated request functions

	/////////////////////////////////////////////
	template <class Return, class Class>
	Return Entity::call(Return (Class::*func)())
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)();
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1>
	Return Entity::call(Return (Class::*func)(Arg1), Arg1 a1)
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)(a1);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2>
	Return Entity::call(Return (Class::*func)(Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)(a1, a2);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3>
	Return Entity::call(Return (Class::*func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)(a1, a2, a3);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4>
	Return Entity::call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)(a1, a2, a3, a4);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	Return Entity::call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)(a1, a2, a3, a4, a5);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	Return Entity::call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		assert(hasComponent<Class>());
		return (getComponent<Class>()->*func)(a1, a2, a3, a4, a5, a6);
	}

#pragma endregion Template Definitions

	/////////////////////////////////////////////
	// Entity physics callbacks

	/////////////////////////////////////////////
	template <class Class>
	void Entity::setBeginContactCallback(void (Class::*func)(PhysicsContact&), typename std::enable_if< std::is_base_of<Component, Class>::value >::type*)
	{
		// Check if callback has already been set
		//assert(!m_beginContactCallback);

		// Attempt to bind function pointer to the component instance
		if(Class* component = getComponent<Class>())
		{
			m_beginContactCallback = std::bind(func, component, std::placeholders::_1);
		}
	}
	
	/////////////////////////////////////////////
	template <class Class>
	void Entity::setBeginContactCallback(void (Class::*func)(PhysicsContact&), typename std::enable_if< std::is_base_of<Entity, Class>::value >::type*)
	{
		// Check if callback has already been set
		//assert(!m_beginContactCallback);

		// Attempt to bind function pointer
		m_beginContactCallback = std::bind(func, dynamic_cast<Class*>(this), std::placeholders::_1);
	}
		
	/////////////////////////////////////////////
	template <class Class>
	void Entity::setEndContactCallback(void (Class::*func)(PhysicsContact&), typename std::enable_if< std::is_base_of<Component, Class>::value >::type*)
	{
		// Check if callback has already been set
		//assert(!m_endContactCallback);

		// Attempt to bind function pointer to the component instance
		if(Class* component = getComponent<Class>())
		{
			m_endContactCallback = std::bind(func, component, std::placeholders::_1);
		}
	}
	
	/////////////////////////////////////////////
	template <class Class>
	void Entity::setEndContactCallback(void (Class::*func)(PhysicsContact&), typename std::enable_if< std::is_base_of<Entity, Class>::value >::type*)
	{
		// Check if callback has already been set
		//assert(!m_endContactCallback);

		// Attempt to bind function pointer
		m_endContactCallback = std::bind(func, this, std::placeholders::_1);
	}

	// Set entity callback for collision filter
	template <class Class>
	void Entity::setCollisionFilterCallback(bool (Class::*f)(PhysicsContact&), typename std::enable_if< std::is_base_of<Entity, Class>::value >::type*)
	{
		// Check if callback has already been set
		//assert(!m_collisionFilterCallback);

		// Attempt to bind function pointer
		m_collisionFilterCallback = std::bind(f, dynamic_cast<Class*>(this), std::placeholders::_1);
	}

}

#endif // SZEN_ENTITY_HPP
