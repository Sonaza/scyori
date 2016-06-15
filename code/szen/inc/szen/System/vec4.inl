////////////////////////////////////////////////////
template <class T>
inline vec4<T>::vec4() :
	x(0), y(0), z(0), w(1)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>::vec4(T v) :
	x(v), y(v), z(v), w(1)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>::vec4(T x, T y, T z, T w) :
	x(x), y(y), z(z), w(w)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>::vec4(vec3<T> v, T w) :
x(v.x), y(v.y), z(v.z), w(w)
{
}

////////////////////////////////////////////////////
template <class T>
template <class U>
inline vec4<T>::vec4(const vec4<U> &vector) :
	x(static_cast<T>(vector.x)),
	y(static_cast<T>(vector.y)),
	z(static_cast<T>(vector.z)),
	w(static_cast<T>(vector.w))
{
}

////////////////////////////////////////////////////
template <class T>
inline float vec4<T>::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& vec4<T>::normalize()
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
inline float vec4<T>::dot(const vec4<T>& b)
{
	return x * b.x + y * b.y + z * b.z;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& vec4<T>::cross(const vec4<T>& b)
{
	*this = vec4<T>(
		y * b.z - z * b.y,
		z * b.x - x * b.z,
		x * b.y - y * b.x
	);

	return *this;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator*(const vec4<T>& lhs, const vec4<T>& rhs)
{
	return vec4<T>(
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator*(const vec4<T>& lhs, T v)
{
	return vec4<T>(
		lhs.x * v,
		lhs.y * v,
		lhs.z * v
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator*(float v, const vec4<T>& rhs)
{
	return rhs * v;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& operator*=(vec4<T>& lhs, const vec4<T>& rhs)
{
	lhs = lhs * rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& operator*=(vec4<T>& lhs, T v)
{
	lhs = lhs * v;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator/(const vec4<T>& lhs, const vec4<T>& rhs)
{
	return vec4<T>(
		lhs.x / rhs.x,
		lhs.y / rhs.y,
		lhs.z / rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator/(const vec4<T>& lhs, T v)
{
	return vec4<T>(
		lhs.x / v,
		lhs.y / v,
		lhs.z / v
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& operator/=(vec4<T>& lhs, const vec4<T>& rhs)
{
	lhs = lhs / rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& operator/=(vec4<T>& lhs, T v)
{
	lhs = lhs / v;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator+(const vec4<T>& lhs, const vec4<T>& rhs)
{
	return vec4<T>(
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& operator+=(vec4<T>& lhs, const vec4<T>& rhs)
{
	lhs = lhs + rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator-(const vec4<T>& lhs, const vec4<T>& rhs)
{
	return vec4<T>(
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T>& operator-=(vec4<T>& lhs, const vec4<T>& rhs)
{
	lhs = lhs - rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> operator-(const vec4<T>& v)
{
	return v * -1.f;
}

////////////////////////////////////////////////////
template <class T>
inline bool operator==(const vec4<T>& lhs, const vec4<T>& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

////////////////////////////////////////////////////
template <class T>
inline bool operator!=(const vec4<T>& lhs, const vec4<T>& rhs)
{
	return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z) || (lhs.w != rhs.w);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> normalize(const vec4<T>& v)
{
	return vec4<T>(v).normalize();
}

////////////////////////////////////////////////////
template <class T>
inline float length(const vec4<T>& v)
{
	return v.length();
}

////////////////////////////////////////////////////
template <class T>
inline float dot(const vec4<T>& a, const vec4<T>& b)
{
	return vec4<T>(a).dot(b);
}

////////////////////////////////////////////////////
template <class T>
inline vec4<T> cross(const vec4<T>& a, const vec4<T>& b)
{
	return vec4<T>(a).cross(b);
}
