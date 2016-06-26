#pragma once

#include <SFML/Audio.hpp>

#include "actor.hpp"
#include "item.hpp"
#include "projectile.hpp"
#include "lightsys.hpp"

namespace Game{
	class Pawn : public Actor
	{
	public:
		Pawn(const sf::Vector2f& _position, sf::Texture& _texture);

		void process() override;
		void draw(sf::RenderWindow& _window) override;
		void collision(Actor& _oth) override;
		void onDestroy() override;
		void stopSounds() override;
		void onDeath() override;

		void fire();
		void altFire(); //alternative fire mode or use item
		void setVelocity(const sf::Vector2f& _velocity) { m_velocity = _velocity; }
		void setProjType(ProjType _type){ m_projType = _type; }

		Graphic::LightInfo& getLightInfo() { return m_lightInfo; }
		float speedFactor() { return m_speedFactor; }
		void setSpeedFactor(float _factor) { m_speedFactor = _factor; }
		float alphaVal() { return m_alpha; }

		void takeItem(Item& _itm);
	private:
		Graphic::LightInfo& m_lightInfo;
		sf::Sprite m_weaponSprite;
		Graphic::CenteredSprite m_healthBarSprite;
		sf::IntRect m_healthRect;
		sf::Sound m_hoverSound;
		int m_healthRectDef;

		float m_fadeFactor;
		float m_alpha;

		//firing
		int m_cdMax;
		int m_cd; //cooldown in frames (16.67ms)

		float m_damage;
		ProjType m_projType;
		float m_speedFactor;

		Item* m_item;
	};
}