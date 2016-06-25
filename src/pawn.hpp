#pragma once

#include "actor.hpp"

namespace Game{
	class Pawn : public Actor
	{
	public:
		Pawn(const sf::Vector2f& _position, sf::Texture& _texture);

		void process() override;
		void draw(sf::RenderWindow& _window) override;
		void collision(Actor& _oth) override;

		void setVelocity(const sf::Vector2f& _velocity) { m_velocity = _velocity; }
	private:
		sf::Sprite m_weaponSprite;
	};
}