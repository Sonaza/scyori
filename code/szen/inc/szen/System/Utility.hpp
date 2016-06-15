#ifndef SZEN_UTILITY_HPP
#define SZEN_UTILITY_HPP

#include <SFML/System.hpp>

#define PI (thor::Pi)
#define TWOPI (thor::Pi*2.f)

namespace sz
{
	
	extern float length(sf::Vector2f&);
	extern float lengthsqrt(sf::Vector2f&);

	extern float distance(sf::Vector2f&, sf::Vector2f&);
	extern float distancesqrt(sf::Vector2f&, sf::Vector2f&);

	extern sf::Vector2f normalize(sf::Vector2f&);

	extern float getAngle(const sf::Vector2f& a, const sf::Vector2f& b);
	extern float getAngle(const sf::Vector2f& v);

	extern float toRadians(float deg);
	extern float toDegrees(float rad);

	extern float wrapAngle(float a);
	extern float wrapAngleDeg(float a);

	extern float angleDiff(float a, float b);
	extern float angleDiffDeg(float a, float b);
	
}

#endif // SZEN_UTILITY_HPP
