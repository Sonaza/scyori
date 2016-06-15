//#include <szen/System/Utility.hpp>
#include <cmath>
#include <SFML/System.hpp>
#include <thor/Math.hpp>


#define PI (thor::Pi)
#define TWOPI (thor::Pi*2.f)

namespace sz
{

////////////////////////////////////////////////////
float length(sf::Vector2f& v)
{
	return v.x * v.x + v.y * v.y;
}

////////////////////////////////////////////////////
float lengthsqrt(sf::Vector2f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

////////////////////////////////////////////////////
extern float distance(sf::Vector2f& a, sf::Vector2f& b)
{
	return sz::length(b - a);
}

////////////////////////////////////////////////////
extern float distancesqrt(sf::Vector2f& a, sf::Vector2f& b)
{
	return sz::lengthsqrt(b - a);
}

////////////////////////////////////////////////////
sf::Vector2f normalize(sf::Vector2f& v)
{
	sf::Vector2f result;

	float len = sz::length(v);
	if(len > 0.f)
	{
		len = std::sqrt(len);
		result.x = v.x / len;
		result.y = v.y / len;
	}

	return result;
}

////////////////////////////////////////////////////
float getAngle(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return atan2(b.y - a.y, b.x - a.x);
}

////////////////////////////////////////////////////
float getAngle(const sf::Vector2f& v)
{
	return atan2(v.x, v.y);
}

////////////////////////////////////////////////////
float toRadians(float deg)
{
	return deg * 0.0174532925199433f;
}

////////////////////////////////////////////////////
float toDegrees(float rad)
{
	return rad * 57.29577951308233f;
}

////////////////////////////////////////////////////
float wrapAngle(float a)
{
	a = fmod(a + PI, TWOPI);
	if(a < 0.f) a += TWOPI;

	return a - PI;
}

////////////////////////////////////////////////////
float wrapAngleDeg(float a)
{
	a = fmod(a + 180.f, 360.f);
	if(a < 0.f) a += 360.f;

	return a - 180.f;
}

////////////////////////////////////////////////////
float angleDiff(float a, float b)
{
	/*float diff = fmod(b - a + PI, TWOPI);
	if(diff < 0.f) diff += TWOPI;
	return diff - PI;*/

	return atan2(sin(b-a), cos(a-b));
}

////////////////////////////////////////////////////
float angleDiffDeg(float a, float b)
{
	/*float diff = fmod(b - a + 180.f, 360.f);
	if(diff < 0.f) diff += 380.f;
	return diff - 180.f;*/

	return toDegrees(angleDiff(toRadians(a), toRadians(b)));
}

}
