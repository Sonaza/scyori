#ifndef SZEN_PHYSICSCONTACT_HPP
#define SZEN_PHYSICSCONTACT_HPP

class b2Contact;
class b2Fixture;

namespace sz
{

	class Entity;

	struct PhysicsContact
	{
		PhysicsContact(Entity* a, Entity* b, b2Contact* contact) :
			entityA(a),
			entityB(b),
			contact(contact),
			fixtureA(NULL),
			fixtureB(NULL)
		{}

		PhysicsContact(Entity* a, Entity* b, b2Fixture* fa, b2Fixture* fb) :
			entityA(a),
			entityB(b),
			contact(NULL),
			fixtureA(fa),
			fixtureB(fb)
		{}

		Entity*		entityA;
		Entity*		entityB;

		b2Contact*	contact;

		b2Fixture*	fixtureA;
		b2Fixture*	fixtureB;

	};

}

#endif // SZEN_PHYSICSCONTACT_HPP
