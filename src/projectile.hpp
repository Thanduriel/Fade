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

	typedef std::vector < Projectile* > ProjCntainer;

	class ProjectileFactory
	{
	public:
		void add(Projectile* _proj) { m_projectiles.push_back(_proj); }

		const ProjCntainer& collect() { return m_projectiles; }
		void clear() { m_projectiles.clear(); }
	private:
		ProjCntainer m_projectiles;
	};

	extern ProjectileFactory g_projectileFactory;
}