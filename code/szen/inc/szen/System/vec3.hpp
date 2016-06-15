#ifndef SZEN_VEC3_HPP
#define SZEN_VEC3_HPP

#include <cmath>

namespace sz
{

	template <class T>
	class vec3
	{
	public:
		static const vec3<float> zero;
		static const vec3<float> up;
		static const vec3<float> forward;
		static const vec3<float> right;

		vec3();
		vec3(T v);
		vec3(T x, T y, T z);

		// Explicit cast from a vector type to another
		template <class U>
		explicit vec3(const vec3<U> &vector);

		// Returns length of the vector
		float length() const;

		// Normalizes vector and returns current value
		vec3& normalize();

		// Vector dot product
		float dot(const vec3& right);

		// Vector cross product
		vec3& cross(const vec3& right);

		// Vector components
		T x, y, z;
	};

	////////////////////////////////////
	template <class T>
	vec3<T> operator*(const vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T> operator*(const vec3<T>& lhs, T v);

	template <class T>
	vec3<T> operator*(T v, const vec3<T>& rhs);

	template <class T>
	vec3<T>& operator*=(vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T>& operator*=(vec3<T>& lhs, T v);

	////////////////////////////////////
	template <class T>
	vec3<T> operator/(const vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T> operator/(const vec3<T>& lhs, T v);

	template <class T>
	vec3<T>& operator/=(vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T>& operator/=(vec3<T>& lhs, T v);

	////////////////////////////////////
	template <class T>
	vec3<T> operator+(const vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T>& operator+=(vec3<T>& lhs, const vec3<T>& rhs);

	////////////////////////////////////
	template <class T>
	vec3<T> operator-(const vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T>& operator-=(vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	vec3<T> operator-(const vec3<T>& v);

	////////////////////////////////////
	template <class T>
	bool operator==(const vec3<T>& lhs, const vec3<T>& rhs);

	template <class T>
	bool operator!=(const vec3<T>& lhs, const vec3<T>& rhs);

	////////////////////////////////////
	template <class T>
	vec3<T> normalize(const vec3<T>& v);

	////////////////////////////////////
	template <class T>
	float length(const vec3<T>& v);

	////////////////////////////////////
	template <class T>
	float dot(const vec3<T>& a, const vec3<T>& b);

	////////////////////////////////////
	template <class T>
	vec3<T> cross(const vec3<T>& a, const vec3<T>& b);

	#include "vec3.inl"

	typedef vec3<float>		vec3f;
	typedef vec3<int>		vec3i;

	// Set a few world global directions
	const vec3<float> vec3<float>::zero(0.f, 0.f, 0.f);
	const vec3<float> vec3<float>::up(0.f, 1.f, 0.f);
	const vec3<float> vec3<float>::forward(0.f, 0.f, 1.f);
	const vec3<float> vec3<float>::right(1.f, 0.f, 0.f);

}

#endif SZEN_VEC3_HPP
