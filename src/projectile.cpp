#include "projectile.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "stats.hpp"

using namespace sf;

namespace Game{
	ProjectileFactory g_projectileFactory;

	Projectile::Projectile(const sf::Vector2f& _pos, const sf::Vector2f& _vel, float _dmg, int _cid):
		Actor(_pos, *g_resourceManager->getTexture("projectile.png"), _cid),
		m_damage(_dmg),
		m_lifeTime(360),
		m_lightInfo(Graphic::g_lightSystem->createLight())
	{
		m_isStatic = false;
		m_isDead = true;
		m_collisionType = CollisionType::Projectile;
		m_velocity = _vel * Constants::c_projectileBaseSpeed;
		m_boundingRad = Constants::c_projectileRadius;

		m_sprite.setColor(Color(228, 255, 255, 255));
		m_lightInfo->color = Color(228, 10, 255, 255);//Color(50, 120, 255, 255);
		m_lightInfo->radius = m_boundingRad * 8.f;
		m_lightInfo->setPosition(m_position);
	}

	void Projectile::collision(Actor& _oth)
	{
		if (_oth.isReflective())
		{
			m_velocity = -m_velocity;
			m_position += normalize(m_position - _oth.position()) * _oth.boundingRad();
			return;
		}
		//other player got hit
		if (_oth.getCId() >= 0) Stats::g_statManager.add(m_cid, Stats::Stat::ShotsHit);
		_oth.damage(m_damage);
		destroy();
	}

	void Projectile::process()
	{
		Actor::process();

		m_lightInfo->setPosition(m_position);

		m_sprite.rotate(float(360 - m_lifeTime)/20.f);

		--m_lifeTime;
		if (m_lifeTime == 0) destroy();
	}

	void Projectile::onDestroy()
	{
		m_lightInfo.release();
	}

	// ******************************************************** //

	void ProjectileFactory::spawn(const sf::Vector2f& _pos, const sf::Vector2f& _dir, ProjType _type, int _cid)
	{
		switch (_type)
		{
		case ProjType::Standard: add(new Projectile(_pos, _dir, 13.f, _cid));
			break;
		case ProjType::Cluster:
		{
			Vector2f off;
			//normal of direction
			off.x = -_dir.y;
			off.y = _dir.x;
			off *= 20.0f;

			add(new Projectile(_pos, _dir, 9.f, _cid));
			add(new Projectile(_pos + off, _dir, 9.f, _cid));
			add(new Projectile(_pos - off, _dir, 9.f, _cid));
			break;
		}
		}
	}
}