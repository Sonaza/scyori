#ifndef SZEN_VEC2_HPP
#define SZEN_VEC2_HPP

#include <cmath>

namespace sz
{

	template <class T>
	class vec2
	{
	public:
		vec2();
		vec2(T v);
		vec2(T x, T y);

		// Explicit cast from a vector type to another
		template <class U>
		explicit vec2(const vec2<U> &vector);

		// Returns length of the vector
		float length() const;

		// Normalizes vector and returns current value
		vec2& normalize();

		// Vector dot product
		float dot(const vec2& right);

		// Vector components
		T x, y;
	};

	////////////////////////////////////
	template <class T>
	vec2<T> operator*(const vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T> operator*(const vec2<T>& lhs, T v);

	template <class T>
	vec2<T> operator*(T v, const vec2<T>& rhs);

	template <class T>
	vec2<T>& operator*=(vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T>& operator*=(vec2<T>& lhs, T v);

	////////////////////////////////////
	template <class T>
	vec2<T> operator/(const vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T> operator/(const vec2<T>& lhs, T v);
	
	template <class T>
	vec2<T>& operator/=(vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T>& operator/=(vec2<T>& lhs, T v);

	////////////////////////////////////
	template <class T>
	vec2<T> operator+(const vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T>& operator+=(vec2<T>& lhs, const vec2<T>& rhs);

	////////////////////////////////////
	template <class T>
	vec2<T> operator-(const vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T>& operator-=(vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	vec2<T> operator-(const vec2<T>& v);

	////////////////////////////////////
	template <class T>
	bool operator==(const vec2<T>& lhs, const vec2<T>& rhs);

	template <class T>
	bool operator!=(const vec2<T>& lhs, const vec2<T>& rhs);

	////////////////////////////////////
	template <class T>
	vec2<T> normalize(const vec2<T>& v);

	////////////////////////////////////
	template <class T>
	float length(const vec2<T>& v);

	////////////////////////////////////
	template <class T>
	float dot(const vec2<T>& a, const vec2<T>& b);
	
	#include "vec2.inl"

	typedef vec2<float>			vec2f;
	typedef vec2<int>			vec2i;
	typedef vec2<unsigned int>	vec2u;

}

#endif // SZEN_VEC2_HPP
