#include <szen/Game/Camera.hpp>
#include <szen/System/Window.hpp>

#include <thor/Math.hpp>


using namespace sz;

namespace
{

	sf::View		_view;
	sf::View		m_interfaceView;

	sf::Vector2f	_position(0.f, 0.f);
	float			_rotation = 0.f;
	float			_zoom = 1.f;

	float			_shakeint = 0.f;

	float			_shakerot = 0.f;
	float			_shakezoom = 0.f;
	sf::Vector2f	_shakepos;

}

////////////////////////////////////////////////////
void Camera::updateScreenSize()
{
	_view.reset(sf::FloatRect(
		0.f, 0.f,
		static_cast<float>(Window::getVirtualSize().x),
		static_cast<float>(Window::getVirtualSize().y)
	));

	m_interfaceView.reset(sf::FloatRect(
		0.f, 0.f,
		static_cast<float>(Window::getVirtualSize().x),
		static_cast<float>(Window::getVirtualSize().y)
	));

	_view.setCenter(-_position);
	//_view.setCenter(static_cast<sf::Vector2f>(Window::getVirtualSize()) / -2.f);
	_view.setRotation(_rotation);
	_view.zoom(_zoom);
}

namespace
{
	float dir = 1.f;
}

#include <iostream>

////////////////////////////////////////////////////
sf::FloatRect Camera::getCameraBounds()
{
	auto pos = _view.getCenter();
	auto rot = _view.getRotation();
	auto size = _view.getSize();
	
	return sf::FloatRect(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, size.x, size.y);
}

////////////////////////////////////////////////////
void Camera::shake(float intensity)
{
	//if(_shakeint < 0.1f)
	{
		_shakerot = thor::random(-1.f, 1.0f);
		_shakepos = sf::Vector2f(thor::random(5.f, 10.f), thor::random(20.f, 30.f));
		//_shakezoom = thor::random(0.005f, 0.03f);
	}

	//if(_shakeint < 1.f)
	//{
	//	_shakeint = intensity * dir * 0.4f;
	//	dir *= 1.f;//thor::random(0, 1) == 0 ? -1.f : 1.f;
	//}

	_shakeint += intensity * 0.4f;
	_shakeint = std::min(3.f, _shakeint);
}

////////////////////////////////////////////////////
sf::View Camera::getCameraView()
{
	sf::View temp(_view);

	if(_shakeint > 0.f)
	{
		temp.zoom(1.f + sin(_shakeint * 3.f) * _shakezoom * _shakeint);
		temp.rotate(sin(_shakeint * 0.5f) * _shakerot * _shakeint * 0.6f);
		temp.move(sin(_shakeint * 3.f) * _shakepos * _shakeint);
	}

	return temp;
}

////////////////////////////////////////////////////
sf::View Camera::getInterfaceView()
{
	return m_interfaceView;
}

////////////////////////////////////////////////////
void Camera::update()
{
	_shakeint += (0.f - _shakeint) / 12.f;
}

////////////////////////////////////////////////////
void Camera::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

////////////////////////////////////////////////////
void Camera::setPosition(const sf::Vector2f &v)
{
	_position = v;
	_view.setCenter(-_position);
	//_view.setCenter(static_cast<sf::Vector2f>(Window::getVirtualSize()) / -2.f);
}

////////////////////////////////////////////////////
sf::Vector2f Camera::getPosition()
{
	return _position;
}

////////////////////////////////////////////////////
void Camera::translate(float x, float y)
{
	translate(sf::Vector2f(x, y));
}

////////////////////////////////////////////////////
void Camera::translate(const sf::Vector2f &v)
{
	_position.x -= v.x;
	_position.y += v.y;
	_view.setCenter(-_position);
	//_view.setCenter(static_cast<sf::Vector2f>(Window::getVirtualSize()) / -2.f);
}

////////////////////////////////////////////////////
void Camera::move(float distance, float angleOffset)
{
	float rad = (-_rotation + angleOffset) * 3.141592f / 180.f;
	translate(cos(rad) * distance, sin(rad) * distance);
}

////////////////////////////////////////////////////
void Camera::setRotation(float a)
{
	_rotation = -a;
	_view.setRotation(_rotation);
}

////////////////////////////////////////////////////
float Camera::getRotation()
{
	return _rotation;
}

////////////////////////////////////////////////////
void Camera::turn(float a)
{
	_rotation += -a;
	_view.setRotation(_rotation);
}

////////////////////////////////////////////////////
void Camera::setZoom(float zoom)
{
	zoom = std::max(0.1f, std::min(10.f, zoom));

	_view.zoom(1.f / _zoom * zoom);
	_zoom = zoom;
}

////////////////////////////////////////////////////
float Camera::getZoom()
{
	return _zoom;
}

////////////////////////////////////////////////////
void Camera::zoom(float zoom)
{
	zoom = 1.f / zoom;

	_view.zoom(zoom);
	_zoom *= zoom;

	//setZoom(_zoom * zoom);
}

////////////////////////////////////////////////////
sf::Vector2f Camera::getCenter()
{
	return static_cast<sf::Vector2f>(Window::getSize()) / 2.f;
}

////////////////////////////////////////////////////
sf::Vector2f Camera::getVirtualCenter()
{
	//return _view.getCenter();
	return static_cast<sf::Vector2f>(Window::getVirtualSize()) / 2.f;
}

////////////////////////////////////////////////////
sf::Vector2f Camera::getCameraOffset()
{
	return _position;
}

sf::Vector2f operator*(sf::Vector2f& lhs, sf::Vector2f& rhs)
{
	return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
}

////////////////////////////////////////////////////
sf::Vector2f Camera::screenToWorld(sf::Vector2f point)
{
	point = sf::Vector2f(
		(point.x / static_cast<float>(Window::getSize().x) - 0.5f) * Window::getVirtualSize().x,
		-(point.y / static_cast<float>(Window::getSize().y) - 0.5f) * Window::getVirtualSize().y
	);

	sf::Transform _transform;
	_transform.translate(sf::Vector2f(-_position.x, _position.y))
		      .rotate(-_rotation)
			  .scale(_zoom, _zoom);

	return _transform.transformPoint(point) * sf::Vector2f(1.f, -1.f);
}

////////////////////////////////////////////////////
sf::Vector2f Camera::worldToScreen(sf::Vector2f point)
{
	sf::Transform _transform;
	_transform.translate(sf::Vector2f(-_position.x, _position.y))
			  .rotate(-_rotation)
			  .scale(_zoom, _zoom);

	point = _transform.getInverse().transformPoint(point);

	point.x = (point.x / static_cast<float>(Window::getVirtualSize().x) + 0.5f) * Window::getSize().x;
	point.y = (point.y / -static_cast<float>(Window::getVirtualSize().y) + 0.5f) * Window::getSize().y;

	return point;
}
