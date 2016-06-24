#pragma once

#include "centeredsprite.hpp"

namespace Game{

	class Actor
	{
	public:
		Actor(const sf::Vector2f& _position, sf::Texture& _texture);

		virtual void process(){};
		virtual void draw(sf::RenderWindow& _window);

	protected:
		Graphic::CenteredSprite m_sprite;
		sf::Vector2f m_position;
		float m_dirAngle;
	};
}