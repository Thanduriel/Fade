#include "actor.hpp"
#include "constants.hpp"

namespace Game{
	using namespace sf;

	Actor::Actor(const Vector2f& _position, sf::Texture& _texture, int _cid):
		m_cid(_cid),
		m_sprite(_texture),
		m_position(_position),
		m_velocity(0.f, 0.f),
		m_dirAngle(0.f),
		m_isStatic(true),
		m_canCollide(true),
		m_health(100.f),
		m_healthMax(100.f),
		m_isDestroyed(false),
		m_isDead(false),
		m_isReflective(false),
		m_hasComplexShape(false),
		m_boundingRad(15.f) //this value is only used by the dummy
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
		m_sprite.setScale(Constants::c_scaleFactor, Constants::c_scaleFactor);
	}

	void Actor::draw(sf::RenderWindow& _window)
	{
		m_sprite.setPosition(m_position);
		_window.draw(m_sprite);
	}

	void Actor::process()
	{
		m_position += m_velocity;
	}

	void Actor::damage(float _amount)
	{
		if (m_isStatic) return;
		onDamage();

		m_health = std::max(m_health -_amount, 0.f);
		if (m_health <= 0.f) {
			m_isDead = true;
			onDeath();
		}
	}
}