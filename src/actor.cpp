#include "actor.hpp"

namespace Game{
	using namespace sf;

	Actor::Actor(const Vector2f& _position, sf::Texture& _texture):
		m_sprite(_texture),
		m_position(_position),
		m_dirAngle(0.f)
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
	}

	void Actor::draw(sf::RenderWindow& _window)
	{
		m_sprite.setPosition(m_position);
		_window.draw(m_sprite);
	}
}