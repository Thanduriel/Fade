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

		void fire();
		void altFire(){}; //alternative fire mode
		void setVelocity(const sf::Vector2f& _velocity) { m_velocity = _velocity; }
	private:
		sf::Sprite m_weaponSprite;
		
		float m_fadeFactor;
		float m_alpha;

		//firing
		int m_cdMax;
		int m_cd; //cooldown in frames (16.67ms)

		float m_damage;
	};
}