#ifndef SZEN_ERRORSTREAM_HPP
#define SZEN_ERRORSTREAM_HPP

#include <string>
#include <sstream>

namespace sz
{

	class ErrorStream
	{
	public:

		enum Signal
		{
			console = 0,
			info	= 1,
			warning	= 2,
			error	= 3
		};

		template <typename T>
		friend ErrorStream& operator<<(ErrorStream& left, T right);
		
		friend ErrorStream& operator<<(ErrorStream& left, ErrorStream::Signal right);

	protected:

		std::stringstream out;

	};
	
	template <typename T>
	ErrorStream& operator<<(ErrorStream& left, T right)
	{
		left.out << right;

		return left;
	}

	// szen error output stream
	extern ErrorStream szerr;

}

#endif
