#ifndef SZEN_VEC4_HPP
#define SZEN_VEC4_HPP

#include <cmath>

namespace sz
{

	template <class T>
	class vec4
	{
	public:
		vec4();
		vec4(T v);
		vec4(T x, T y, T z, T w = 1);
		vec4(vec3<T> v, T w = 1);

		// Explicit cast from a vector type to another
		template <class U>
		explicit vec4(const vec4<U> &vector);

		// Returns length of the vector
		float length() const;

		// Normalizes vector and returns current value
		vec4& normalize();

		// Vector dot product
		float dot(const vec4& right);

		// Vector cross product
		vec4& cross(const vec4& right);

		// Vector components
		T x, y, z, w;
	};

	////////////////////////////////////
	template <class T>
	vec4<T> operator*(const vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T> operator*(const vec4<T>& lhs, T v);

	template <class T>
	vec4<T> operator*(T v, const vec4<T>& rhs);

	template <class T>
	vec4<T>& operator*=(vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T>& operator*=(vec4<T>& lhs, T v);

	////////////////////////////////////
	template <class T>
	vec4<T> operator/(const vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T> operator/(const vec4<T>& lhs, T v);

	template <class T>
	vec4<T>& operator/=(vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T>& operator/=(vec4<T>& lhs, T v);

	////////////////////////////////////
	template <class T>
	vec4<T> operator+(const vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T>& operator+=(vec4<T>& lhs, const vec4<T>& rhs);

	////////////////////////////////////
	template <class T>
	vec4<T> operator-(const vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T>& operator-=(vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	vec4<T> operator-(const vec4<T>& v);

	////////////////////////////////////
	template <class T>
	bool operator==(const vec4<T>& lhs, const vec4<T>& rhs);

	template <class T>
	bool operator!=(const vec4<T>& lhs, const vec4<T>& rhs);

	////////////////////////////////////
	template <class T>
	vec4<T> normalize(const vec4<T>& v);

	////////////////////////////////////
	template <class T>
	float length(const vec4<T>& v);

	////////////////////////////////////
	template <class T>
	float dot(const vec4<T>& a, const vec4<T>& b);

	////////////////////////////////////
	template <class T>
	vec4<T> cross(const vec4<T>& a, const vec4<T>& b);

	#include "vec4.inl"

	typedef vec4<float>			vec4f;
	typedef vec4<int>			vec4i;

}

#endif VEC4_HPP
