#pragma once

#include "actor.hpp"

namespace Game{
	// item that will be taken on collision
	class Item : public Actor
	{
	public:
		Item(const sf::Vector2f& _pos, sf::Texture& _texture);
		void collision(Actor& _oth) override;

		virtual void use(){};
	};

	class Mine : public Item
	{
	public:
		Mine(const sf::Vector2f& _pos);

		void collision(Actor& _oth) override;
		void use() override;
		void process() override;
	private:
		enum{
			Pickable, Ticking, Active
		}m_state;
		int m_cd; // countdown
	};
}