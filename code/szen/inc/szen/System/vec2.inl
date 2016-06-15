////////////////////////////////////////////////////
template <class T>
inline vec2<T>::vec2() :
	x(0), y(0)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>::vec2(T v) :
	x(v), y(v)
{
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>::vec2(T x, T y) :
	x(x), y(y)
{
}

////////////////////////////////////////////////////
template <class T>
template <class U>
inline vec2<T>::vec2(const vec2<U> &vector) :
	x(static_cast<T>(vector.x)),
	y(static_cast<T>(vector.y))
{
}

////////////////////////////////////////////////////
template <class T>
inline float vec2<T>::length() const
{
	return std::sqrt(x * x + y * y);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& vec2<T>::normalize()
{
	float len = length();
	if(len != 0.f)
	{
		x /= len;
		y /= len;
	}

	return *this;
}

////////////////////////////////////////////////////
template <class T>
inline float vec2<T>::dot(const vec2<T>& b)
{
	return x * b.x + y * b.y;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator*(const vec2<T>& lhs, const vec2<T>& rhs)
{
	return vec2<T>(
		lhs.x * rhs.x,
		lhs.y * rhs.y
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator*(const vec2<T>& lhs, T v)
{
	return vec2<T>(
		lhs.x * v,
		lhs.y * v
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator*(float v, const vec2<T>& rhs)
{
	return rhs * v;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& operator*=(vec2<T>& lhs, const vec2<T>& rhs)
{
	lhs = lhs * rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& operator*=(vec2<T>& lhs, T v)
{
	lhs = lhs * v;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator/(const vec2<T>& lhs, const vec2<T>& rhs)
{
	return vec2<T>(
		lhs.x / rhs.x,
		lhs.y / rhs.y
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator/(const vec2<T>& lhs, T v)
{
	return vec2<T>(
		lhs.x / v,
		lhs.y / v
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& operator/=(vec2<T>& lhs, const vec2<T>& rhs)
{
	lhs = lhs / rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& operator/=(vec2<T>& lhs, T v)
{
	lhs = lhs / v;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator+(const vec2<T>& lhs, const vec2<T>& rhs)
{
	return vec2<T>(
		lhs.x + rhs.x,
		lhs.y + rhs.y
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& operator+=(vec2<T>& lhs, const vec2<T>& rhs)
{
	lhs = lhs + rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator-(const vec2<T>& lhs, const vec2<T>& rhs)
{
	return vec2<T>(
		lhs.x - rhs.x,
		lhs.y - rhs.y
	);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T>& operator-=(vec2<T>& lhs, const vec2<T>& rhs)
{
	lhs = lhs - rhs;
	return lhs;
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> operator-(const vec2<T>& v)
{
	return v * -1.f;
}

////////////////////////////////////////////////////
template <class T>
inline bool operator==(const vec2<T>& lhs, const vec2<T>& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

////////////////////////////////////////////////////
template <class T>
inline bool operator!=(const vec2<T>& lhs, const vec2<T>& rhs)
{
	return (lhs.x != rhs.x) || (lhs.y != rhs.y);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> normalize(const vec2<T>& v)
{
	return vec2<T>(v).normalize();
}

////////////////////////////////////////////////////
template <class T>
inline float length(const vec2<T>& v)
{
	return v.length();
}

////////////////////////////////////////////////////
template <class T>
inline float dot(const vec2<T>& a, const vec2<T>& b)
{
	return vec2<T>(a).dot(b);
}

////////////////////////////////////////////////////
template <class T>
inline vec2<T> cross(const vec2<T>& a, const vec2<T>& b)
{
	return vec2<T>(a).cross(b);
}
