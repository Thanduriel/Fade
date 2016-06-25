#pragma once

#include "actor.hpp"
#include "item.hpp"

namespace Game{
	class Pawn : public Actor
	{
	public:
		Pawn(const sf::Vector2f& _position, sf::Texture& _texture);

		void process() override;
		void draw(sf::RenderWindow& _window) override;
		void collision(Actor& _oth) override;

		void fire();
		void altFire(); //alternative fire mode or use item
		void setVelocity(const sf::Vector2f& _velocity) { m_velocity = _velocity; }

		void takeItem(Item& _itm);
	private:
		sf::Sprite m_weaponSprite;
		Graphic::CenteredSprite m_healthBarSprite;
		sf::IntRect m_healthRect;
		int m_healthRectDef;

		float m_fadeFactor;
		float m_alpha;

		//firing
		int m_cdMax;
		int m_cd; //cooldown in frames (16.67ms)

		float m_damage;

		Item* m_item;
	};
}