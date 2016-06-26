#include "projectile.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"

using namespace sf;

namespace Game{
	ProjectileFactory g_projectileFactory;

	Projectile::Projectile(const sf::Vector2f& _pos, const sf::Vector2f& _vel, float _dmg):
		Actor(_pos, *g_resourceManager.getTexture("projectile.png")),
		m_damage(_dmg),
		m_lifeTime(360)
	{
		m_velocity = _vel * Constants::c_projectileBaseSpeed;
		m_boundingRad = Constants::c_projectileRadius;
	}

	void Projectile::collision(Actor& _oth)
	{
		_oth.damage(m_damage);
		m_isDestroyed = true;
	}

	void Projectile::process()
	{
		Actor::process();
		m_sprite.rotate(float(360 - m_lifeTime)/20.f);

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