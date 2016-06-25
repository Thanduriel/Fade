#pragma once

#include "actor.hpp"

namespace Game{
	class Pawn;
	// item that will be taken on collision
	class Item : public Actor
	{
	public:
		Item(const sf::Vector2f& _pos, sf::Texture& _texture);
		void collision(Actor& _oth) override;

		virtual void use(){ m_cd = m_activeTime; };
		virtual void endUse(){};
		void process() override;
	protected:
		int m_activeTime;
		Pawn* m_pawn;
	private:
		int m_cd; // countdown or active time
	};

	class Mine : public Item
	{
	public:
		Mine(const sf::Vector2f& _pos);

		void collision(Actor& _oth) override;
		void use() override;
		void endUse() override;
	private:
		enum{
			Pickable, Ticking, Active
		}m_state;
	};

	class ClusterGun : public Item
	{
	public:
		ClusterGun(const sf::Vector2f& _pos);
		void use() override;
		void endUse() override;
	};
}