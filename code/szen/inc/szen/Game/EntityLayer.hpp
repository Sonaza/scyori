#ifndef SZEN_ENTITYLAYER_HPP
#define SZEN_ENTITYLAYER_HPP

namespace sz
{

	struct EntityLayer
	{
		typedef int Level;
		typedef int Id;

		enum
		{
			Background	= 0,
			Foreground	= 1000,
			Interface	= 2000
		};

	};

}

#endif // SZEN_ENTITYLAYER_HPP
