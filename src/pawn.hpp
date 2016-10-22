#pragma once

#include <SFML/Audio.hpp>
#include <array>

#include "actor.hpp"
#include "item.hpp"
#include "projectile.hpp"
#include "lightsys.hpp"

namespace Game{

	enum PlayerColor
	{
		White,
		Green,
		Orange,
		Blue,
		Beige,
		Yellow,
		Greeen,
		Red,
		Count
	};

	const std::array<sf::Color, Count> PLAYERCOLORS =
	{
		//	sf::Color(255, 0, 90),
		sf::Color(255, 255, 255),
		sf::Color(20, 220, 120),
		sf::Color(160, 160, 255),
		sf::Color(230, 160, 20),
		sf::Color(225, 225, 200),
		sf::Color::Yellow,
		sf::Color(0,255,0),
		sf::Color(255,140,140)
	};

	class Pawn : public Actor
	{
	public:
		Pawn(const sf::Vector2f& _position, sf::Texture& _texture, int _cid);
		void process() override;
		void draw(sf::RenderWindow& _window) override;
		void collision(Actor& _oth) override;
		void onDestroy() override;
		void stopSounds() override;
		void onDeath() override;
		void onDamage() override;

		void fire();
		void altFire(); //alternative fire mode or use item
		void setVelocity(const sf::Vector2f& _velocity) { m_velocity = _velocity; }
		void setProjType(ProjType _type){ m_projType = _type; }

		Graphic::LightInfo& getLightInfo() { return *m_lightInfo.get(); }
		float speedFactor() { return m_speedFactor; }
		void setSpeedFactor(float _factor) { m_speedFactor = _factor; }
		float alphaVal() { return m_alpha; }

		void takeItem(Item& _itm);

		enum LightState{
			Off,
			On,
			OnlyFire,
			Count
		};

		void setLightState(LightState _state){ m_lightState = _state; }
		LightState lightState(){ return m_lightState; }

		PlayerColor getPlayerColor() const { return m_playerColor; }
		void switchColor(PlayerColor _col) { m_playerColor = _col; setColor(PLAYERCOLORS[m_playerColor]); }
		void switchColor() { m_playerColor = (PlayerColor)((m_playerColor + 1) % PLAYERCOLORS.size()); setColor(PLAYERCOLORS[m_playerColor]); }
	private:
		int m_lastHitId; //cid of the last actor a collision occurred with

		Graphic::LightInfoHandle m_lightInfo;
		sf::Sprite m_weaponSprite;
		Graphic::CenteredSprite m_healthBarSprite;
		sf::IntRect m_healthRect;
		sf::IntRect m_weaponRect;
		int m_weaponRectDef;
		sf::Sound m_soundFire, m_soundAltFire, m_soundCollision, m_soundExplode, m_soundTakeItem;
		int m_healthRectDef;

		float m_fadeFactor;
		float m_alpha;

		//firing
		int m_cdMax;
		int m_cd; //cooldown in frames (16.67ms)

		float m_damage;
		ProjType m_projType;
		float m_speedFactor;
		int m_ammo;

		Item* m_item;

		LightState m_lightState;

		PlayerColor m_playerColor;
	};
}