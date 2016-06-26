#include "pawn.hpp"
#include "resourcemanager.hpp"
#include "projectile.hpp"
#include "math.hpp"
#include "constants.hpp"
#include "lightsys.hpp"

using namespace sf;

namespace Game{
	Pawn::Pawn(const sf::Vector2f& _position, sf::Texture& _texture) :
		Actor(_position, _texture),
		m_lightInfo(Graphic::g_lightSystem.createLight()),
		m_weaponSprite(*g_resourceManager.getTexture("player_outer_halfring.png")),
		m_healthBarSprite(*g_resourceManager.getTexture("player_inner_ring.png")),
		m_fadeFactor(0.992f),
		m_alpha(1.f),
		m_cdMax(130),
		m_damage(10.f),
		m_item(nullptr),
		m_projType(ProjType::Standard),
		m_speedFactor(1.f)
	{
		m_isStatic = false;

		m_soundFire.setBuffer(*g_resourceManager.getSound("sound_shot"));
		m_soundAltFire.setBuffer(*g_resourceManager.getSound("sound_altShot"));
		m_soundCollision.setBuffer(*g_resourceManager.getSound("sound_collision"));

		m_weaponSprite.setOrigin(sf::Vector2f(0.f, m_weaponSprite.getTextureRect().height * 0.5f));//_texture.getSize().x
		m_healthBarSprite.rotate(180.f);
		m_healthBarSprite.setColor(sf::Color(138, 240, 12, 255));

	//	m_sprite.setScale(Constants::c_scaleFactor, c_scaleFactor);
		m_healthBarSprite.setScale(Constants::c_scaleFactor, Constants::c_scaleFactor);
		m_weaponSprite.setScale(Constants::c_scaleFactor, Constants::c_scaleFactor);
		m_weaponSprite.setColor(sf::Color(228, 10, 255, 255));

		m_healthRect = m_healthBarSprite.getTextureRect();
		m_healthRectDef = m_healthRect.height;
		//radius of the final scaled sprite
		m_boundingRad = (float)m_sprite.getTexture()->getSize().x * m_sprite.getScale().x * 0.5f;

		m_lightInfo.color = Color(255, 255, 255, 0);
		m_lightInfo.radius = m_boundingRad * 8.f;
	}

	// ********************************************************* //

	void Pawn::process()
	{
		if (m_isDeath)
		{
			if (alphaVal() < 0.1f)
			{
				destroy();
			}
			return;
		}

		Actor::process();

		//fading
		//handled in the light shader now
	//	m_alpha *= m_fadeFactor;
	//	if (m_alpha < 0.1f) m_alpha = 1.f;

		--m_cd;
	//	if (m_cd <= 0) {
	//		fire(); altFire();
	//	}

		if (m_item)
		{
			m_item->setPosition(m_position);
		}
	}

	// ********************************************************* //

	void Pawn::draw(sf::RenderWindow& _window)
	{
		m_lightInfo.setPosition(m_position);

		m_alpha *= m_fadeFactor;
		uint8_t a = (uint8_t)(m_alpha * 255.f);

		m_lightInfo.color.a = a;

	//	m_sprite.setColor(sf::Color(255, 255, 255, a));
		Actor::draw(_window);

		//health bar size depends on current health
		m_healthBarSprite.setPosition(m_position);
//		m_healthBarSprite.setColor(sf::Color(138, 240, 12, a));
		m_healthRect.height = m_healthRectDef * m_health / m_healthMax;
		m_healthBarSprite.setTextureRect(m_healthRect);

		_window.draw(m_healthBarSprite);

		// weapon points in the direction the player is facing
		m_weaponSprite.setPosition(m_position);
	//	m_weaponSprite.setColor(sf::Color(228, 10, 255, a));
		m_weaponSprite.setRotation(m_dirAngle / (2.f*3.1415f) * 360.f);
		_window.draw(m_weaponSprite);
	}

	void Pawn::collision(Actor& _oth)
	{
		_oth.damage(1.f);
		m_soundCollision.play();
	}

	void Pawn::stopSounds()
	{
	}

	void Pawn::onDestroy()
	{
		//destroy stuff depended on this pawn
		if (m_item) m_item->destroy();
	}

	void Pawn::onDeath()
	{
		m_alpha = 1.f;
		m_fadeFactor = 0.997;
		m_lightInfo.radius *= 3.f;
		m_lightInfo.color.g = 50;
		m_lightInfo.color.b = 50;
		m_canCollide = false;
	}

	// ********************************************************* //

	void Pawn::fire()
	{
		if (m_cd <= 0)
		{
			m_alpha = 1.f;
			//spawn projectile
			Vector2f dir = normalize(Vector2f(cos(m_dirAngle), sin(m_dirAngle)));
			g_projectileFactory.spawn(m_position + dir * (m_boundingRad + Constants::c_projectileRadius + 6.f),
				dir, m_projType);
			m_soundFire.play();

			m_cd = m_cdMax;
		}
	}

	void Pawn::altFire()
	{
		if (!m_item) return;
		m_item->use();
		m_item = nullptr;
		m_soundAltFire.play();
	}

	void Pawn::takeItem(Item& _itm)
	{
		if (m_item) m_item->destroy();
		m_item = &_itm;
	}
}