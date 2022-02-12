#pragma once

#include "actor.hpp"
#include "lightsys.hpp"

namespace Game{
	class Pawn;
	// item that will be taken on collision
	class Item : public Actor
	{
	public:
		Item(const sf::Vector2f& _pos, sf::Texture& _texture, int _activeTime);
		void collision(Actor& _oth) override;
		virtual void use() { m_cd = m_activeTime; }
		virtual void endUse() {}
		void process() override;
	protected:
		//sets the active timer to the given value
		// use this if you want to have an item with more timed states
		void SetTimer(int _cd) { m_cd = _cd; }

		int m_activeTime;
		Pawn* m_pawn;
		Graphic::LightInfoHandle m_lightInfo;
	private:
		int m_cd; // countdown or active time
		sf::Sound m_soundAppear;
	};

	// can be placed
	// explodes on collision
	class Mine : public Item
	{
	public:
		Mine(const sf::Vector2f& _pos);

		void collision(Actor& _oth) override;
		void use() override;
		void endUse() override;
	private:
		enum struct State {
			Pickable, Ticking, Active
		};
		State m_state;
	};

	class Sentinel : public Item
	{
	public:
		Sentinel(const sf::Vector2f& _pos);
		void collision(Actor& _oth) override;
		void use() override;
		void endUse() override;
	private:
		enum struct State {
			Pickable, Ticking, Active
		};
		State m_state;
	};

	// lets the player shoot multiple projectiles at once
	class ClusterGun : public Item
	{
	public:
		ClusterGun(const sf::Vector2f& _pos);
		void use() override;
		void endUse() override;
	};

	// increases the light radius temporary
	class LightAura : public Item
	{
	public:
		LightAura(const sf::Vector2f& _pos);

		void use() override;
		void endUse() override;

	private:
	};

	// instantly heal up to full health
	class HealthBoost : public Item
	{
	public:
		HealthBoost(const sf::Vector2f& _pos);

		void use() override;
		void endUse() override;
	};

	class SpeedBoost : public Item
	{
	public:
		SpeedBoost(const sf::Vector2f& _pos);

		void use() override;
		void endUse() override;
	};

	// puts on a child that reflects projectiles toward their origin
	class Shield : public Item
	{
	public:
		Shield(const sf::Vector2f& _pos);

		void use() override;
		void endUse() override;
	};
}