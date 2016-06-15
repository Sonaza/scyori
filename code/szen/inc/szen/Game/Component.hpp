#ifndef SZEN_COMPONENT_HPP
#define SZEN_COMPONENT_HPP

#include <cassert>
#include <string>

#include <szen/Game/Entity.hpp>

#include <json/json.h>

namespace sz
{

	class Component
	{
		friend class Entity;

	public:
		Component();
		virtual ~Component();

		// Called when component is attached to a parent entity
		virtual void attached() =0;

		// Optional component update
		virtual void update() {}

		virtual void parsePrefab(json::Value&) =0;

		struct Order
		{
			enum ComponentOrder
			{
				Transform	= 0,
				Physics		= 1,
				Animation	= 9,
				Renderer	= 10
			};
		};
		
		///////////////////////////////////////
		// Function callbacks

		template <class Class>
		void call(void (Class::*func)());

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
		Return call(Return (Class::*func)());

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
		
	protected:

		// Component's parent entity
		Entity*			m_entity;

		sf::Int32		m_componentOrder;
		
	};

	/////////////////////////////////////////////
	template <class Class>
	void Component::call(void (Class::*func)())
	{
		assert(m_entity);
		m_entity->call(func);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1>
	void Component::call(void (Class::*func)(Arg1), Arg1 a1)
	{
		assert(m_entity);
		m_entity->call(func, a1);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2>
	void Component::call(void (Class::*func)(Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		assert(m_entity);
		m_entity->call(func, a1, a2);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3>
	void Component::call(void (Class::*func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		assert(m_entity);
		m_entity->call(func, a1, a2, a3);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3, class Arg4>
	void Component::call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		assert(m_entity);
		m_entity->call(func, a1, a2, a3, a4);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	void Component::call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		assert(m_entity);
		m_entity->call(func, a1, a2, a3, a4, a5);
	}

	/////////////////////////////////////////////
	template <class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	void Component::call(void (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		assert(m_entity);
		m_entity->call(func, a1, a2, a3, a4, a5, a6);
	}

	///////////////////////////////////////////////////
	// Templated request functions

	/////////////////////////////////////////////
	template <class Return, class Class>
	Return Component::call(Return (Class::*func)())
	{
		assert(m_entity);
		return m_entity->call(func);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1>
	Return Component::call(Return (Class::*func)(Arg1), Arg1 a1)
	{
		assert(m_entity);
		return m_entity->call(func, a1);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2>
	Return Component::call(Return (Class::*func)(Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		assert(m_entity);
		return m_entity->call(func, a1, a2);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3>
	Return Component::call(Return (Class::*func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		assert(m_entity);
		return m_entity->call(func, a1, a2, a3);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4>
	Return Component::call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		assert(m_entity);
		return m_entity->call(func, a1, a2, a3, a4);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
	Return Component::call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		assert(m_entity);
		return m_entity->call(func, a1, a2, a3, a4, a5);
	}

	/////////////////////////////////////////////
	template <class Return, class Class, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	Return Component::call(Return (Class::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6)
	{
		assert(m_entity);
		return m_entity->call(func, a1, a2, a3, a4, a5, a6);
	}
	
}

#endif // SZEN_COMPONENT_HPP
