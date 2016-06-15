////////////////////////////////////////////////////
template <class T>
inline vec3<T>::vec3() :
	x(0), y(0), z(0)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>::vec3(T v) :
	x(v), y(v), z(v)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>::vec3(T x, T y, T z) :
	x(x), y(y), z(z)
{
}

////////////////////////////////////////////////////
template <class T>
template <class U>
inline vec3<T>::vec3(const vec3<U> &vector) :
	x(static_cast<T>(vector.x)),
	y(static_cast<T>(vector.y)),
	z(static_cast<T>(vector.z))
{
}

////////////////////////////////////////////////////
template <class T>
inline float vec3<T>::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& vec3<T>::normalize()
{
	float len = length();
	if(len != 0.f)
	{
		x /= len;
		y /= len;
		z /= len;
	}

	return *this;
}

////////////////////////////////////////////////////
template <class T>
inline float vec3<T>::dot(const vec3<T>& b)
{
	return x * b.x + y * b.y + z * b.z;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& vec3<T>::cross(const vec3<T>& b)
{
	*this = vec3<T>(
		y * b.z - z * b.y,
		z * b.x - x * b.z,
		x * b.y - y * b.x
	);

	return *this;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator*(const vec3<T>& lhs, const vec3<T>& rhs)
{
	return vec3<T>(
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator*(const vec3<T>& lhs, T v)
{
	return vec3<T>(
		lhs.x * v,
		lhs.y * v,
		lhs.z * v
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator*(float v, const vec3<T>& rhs)
{
	return rhs * v;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& operator*=(vec3<T>& lhs, const vec3<T>& rhs)
{
	lhs = lhs * rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& operator*=(vec3<T>& lhs, T v)
{
	lhs = lhs * v;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator/(const vec3<T>& lhs, const vec3<T>& rhs)
{
	return vec3<T>(
		lhs.x / rhs.x,
		lhs.y / rhs.y,
		lhs.z / rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator/(const vec3<T>& lhs, T v)
{
	return vec3<T>(
		lhs.x / v,
		lhs.y / v,
		lhs.z / v
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& operator/=(vec3<T>& lhs, const vec3<T>& rhs)
{
	lhs = lhs / rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& operator/=(vec3<T>& lhs, T v)
{
	lhs = lhs / v;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator+(const vec3<T>& lhs, const vec3<T>& rhs)
{
	return vec3<T>(
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& operator+=(vec3<T>& lhs, const vec3<T>& rhs)
{
	lhs = lhs + rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator-(const vec3<T>& lhs, const vec3<T>& rhs)
{
	return vec3<T>(
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T>& operator-=(vec3<T>& lhs, const vec3<T>& rhs)
{
	lhs = lhs - rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> operator-(const vec3<T>& v)
{
	return v * -1.f;
}

////////////////////////////////////////////////////
template <class T>
inline bool operator==(const vec3<T>& lhs, const vec3<T>& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

////////////////////////////////////////////////////
template <class T>
inline bool operator!=(const vec3<T>& lhs, const vec3<T>& rhs)
{
	return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> normalize(const vec3<T>& v)
{
	return vec3<T>(v).normalize();
}

////////////////////////////////////////////////////
template <class T>
inline float length(const vec3<T>& v)
{
	return v.length();
}

////////////////////////////////////////////////////
template <class T>
inline float dot(const vec3<T>& a, const vec3<T>& b)
{
	return vec3<T>(a).dot(b);
}

////////////////////////////////////////////////////
template <class T>
inline vec3<T> cross(const vec3<T>& a, const vec3<T>& b)
{
	return vec3<T>(a).cross(b);
}
