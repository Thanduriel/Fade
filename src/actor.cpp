#include "actor.hpp"

namespace Game{
	using namespace sf;

	Actor::Actor(const Vector2f& _position, sf::Texture& _texture):
		m_sprite(_texture),
		m_position(_position),
		m_velocity(0.f, 0.f),
		m_dirAngle(0.f),
		m_isStatic(true),
		m_health(100.f),
		m_isDestroyed(false)
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
	}

	void Actor::draw(sf::RenderWindow& _window)
	{
		m_sprite.setPosition(m_position);
		_window.draw(m_sprite);
	}

	void Actor::process()
	{
		m_position += m_velocity;
	}

	void Actor::damage(float _amount)
	{
		if (m_isStatic) return;

		m_health -= _amount;
		if (m_health <= 0.f) m_isDestroyed = true;
	}
}