#ifndef SZEN_TYPES_HPP
#define SZEN_TYPES_HPP

#include <szen/System/vec2.hpp>
#include <szen/System/vec3.hpp>
#include <szen/System/vec4.hpp>
#include <szen/System/mat4.hpp>

namespace sz
{
	// 8 bits integer types
	typedef signed   char int8;
	typedef unsigned char uint8;

	// 16 bits integer types
	typedef signed   short int16;
	typedef unsigned short uint16;

	// 32 bits integer types
	typedef signed   int int32;
	typedef unsigned int uint32;

	// 64 bits integer types
#if defined(_MSC_VER)
	typedef signed   __int64 int64;
	typedef unsigned __int64 uint64;
#else
	typedef signed   long long int64;
	typedef unsigned long long uint64;
#endif

}

#endif SZEN_TYPES_HPP
