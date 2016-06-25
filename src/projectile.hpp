#pragma once

#include "actor.hpp"

namespace Game{
	class Projectile : public Actor
	{
	public:
		Projectile(const sf::Vector2f& _pos, const sf::Vector2f& _vel, float _dmg);

		void collision(Actor& _oth) override;
	private:
		float m_damage;
	};
}