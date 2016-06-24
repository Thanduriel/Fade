#pragma once

#include <SFML\Graphics.hpp>

namespace Game{
	class Actor
	{
	public:
		Actor(const sf::Vector2f& _position, sf::Texture& _texture);

		virtual void process(){};
		virtual void draw(sf::RenderWindow& _window);

	protected:
		sf::Sprite m_sprite;
		sf::Vector2f m_position;
	};
}