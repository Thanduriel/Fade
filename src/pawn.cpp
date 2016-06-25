#include "pawn.hpp"
#include "resourcemanager.hpp"
#include "projectile.hpp"
#include "math.hpp"

using namespace sf;

namespace Game{
	Pawn::Pawn(const sf::Vector2f& _position, sf::Texture& _texture) :
		Actor(_position, _texture),
		m_weaponSprite(*g_resourceManager.getTexture("player_outer_halfring.png")),
		m_healthBarSprite(*g_resourceManager.getTexture("player_inner_ring.png")),
		m_fadeFactor(0.992f),
		m_alpha(1.f),
		m_cdMax(130),
		m_damage(10.f)
	{
		m_isStatic = false;

		m_weaponSprite.setOrigin(sf::Vector2f(0.f, m_weaponSprite.getTextureRect().height * 0.5f));//_texture.getSize().x
		m_healthBarSprite.rotate(180.f);

		m_sprite.setScale(0.4f, 0.4f);
		m_healthBarSprite.setScale(0.4f, 0.4f);
		m_weaponSprite.setScale(0.4f, 0.4f);

		m_healthRect = m_healthBarSprite.getTextureRect();
		m_healthRectDef = m_healthRect.height;
		//radius of the final scaled sprite
		m_boundingRad = (float)m_sprite.getTexture()->getSize().x * m_sprite.getScale().x * 0.5f;
	}

	void Pawn::process()
	{
		Actor::process();

		//fading
		m_alpha *= m_fadeFactor;
	//	if (m_alpha < 0.1f) m_alpha = 1.f;

		--m_cd;
		if (m_cd <= 0) fire();
	}

	void Pawn::draw(sf::RenderWindow& _window)
	{
		uint8_t a = (uint8_t)(m_alpha * 255.f);
		m_sprite.setColor(sf::Color(255, 255, 255, a));
		Actor::draw(_window);

		m_healthBarSprite.setPosition(m_position);
		m_healthBarSprite.setColor(sf::Color(138, 240, 12, a));
		m_healthRect.height = m_healthRectDef * m_health / m_healthMax;
		m_healthBarSprite.setTextureRect(m_healthRect);

		_window.draw(m_healthBarSprite);

		m_weaponSprite.setPosition(m_position);
		m_weaponSprite.setColor(sf::Color(228, 10, 255, a));
		m_weaponSprite.setRotation(m_dirAngle / (2.f*3.1415f) * 360.f);
		_window.draw(m_weaponSprite);
	}

	void Pawn::collision(Actor& _oth)
	{
		_oth.damage(1.f);
	}

	void Pawn::fire()
	{
		if (m_cd <= 0)
		{
			m_alpha = 1.f;
			//spawn projectile
			Vector2f dir = normalize(Vector2f(cos(m_dirAngle), sin(m_dirAngle)));
			g_projectileFactory.add(new Projectile(m_position + dir * m_boundingRad * 1.1f,
				dir * 10.f, m_damage));

			m_cd = m_cdMax;
		}
	}
}