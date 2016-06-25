#include "projectile.hpp"
#include "resourcemanager.hpp"

using namespace sf;

namespace Game{
	ProjectileFactory g_projectileFactory;

	Projectile::Projectile(const sf::Vector2f& _pos, const sf::Vector2f& _vel, float _dmg):
		Actor(_pos, *g_resourceManager.getTexture("wall.png")),
		m_damage(_dmg),
		m_lifeTime(160)
	{
		m_velocity = _vel;
		m_boundingRad = 5.f;
	}

	void Projectile::collision(Actor& _oth)
	{
		_oth.damage(m_damage);
		m_isDestroyed = true;
	}

	void Projectile::process()
	{
		Actor::process();

		--m_lifeTime;
		if (m_lifeTime == 0) m_isDestroyed = true;
	}

	// ******************************************************** //

	void ProjectileFactory::spawn(const sf::Vector2f& _pos, const sf::Vector2f& _dir, ProjType _type)
	{
		switch (_type)
		{
		case ProjType::Standard: add(new Projectile(_pos, _dir, 10.f));
			break;
		case ProjType::Cluster:
		{
			Vector2f off;
			//normal of direction
			off.x = -_dir.y;
			off.y = _dir.x;
			off *= 20.0f;

			add(new Projectile(_pos, _dir, 10.f));
			add(new Projectile(_pos + off, _dir, 10.f));
			add(new Projectile(_pos - off, _dir, 10.f));
			break;
		}
		}
	}
}