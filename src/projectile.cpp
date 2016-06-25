#include "projectile.hpp"
#include "resourcemanager.hpp"

using namespace sf;

namespace Game{
	ProjectileFactory g_projectileFactory;

	Projectile::Projectile(const sf::Vector2f& _pos, const sf::Vector2f& _vel, float _dmg):
		Actor(_pos, *g_resourceManager.getTexture("wall.png")),
		m_damage(_dmg)
	{
		m_sprite.setScale(0.2f, 0.2f);
		m_velocity = _vel;
		m_boundingRad = 3.f;
	}

	void Projectile::collision(Actor& _oth)
	{
		_oth.damage(m_damage);
		m_isDestroyed = true;
	}
}