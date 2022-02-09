#include "pawn.hpp"
#include "resourcemanager.hpp"
#include "projectile.hpp"
#include "math.hpp"
#include "constants.hpp"
#include "lightsys.hpp"
#include "stats.hpp"
#include <cmath>

using namespace sf;

namespace Game{
	Pawn::Pawn(const sf::Vector2f& _position, sf::Texture& _texture, int _cid) :
		Actor(_position, _texture, _cid),
		m_lastHitId(_cid),
		m_lightInfo(Graphic::g_lightSystem->createLight()),
		m_weaponSprite(*g_resourceManager->getTexture("player_outer_halfring.png")),
		m_healthBarSprite(*g_resourceManager->getTexture("player_inner_ring.png")),
		m_fadeFactor(Constants::c_playerBaseFadeFactor),
		m_alpha(1.f),
		m_cdMax(Constants::c_baseReloadSpeed),//130
		m_cd(m_cdMax),
		m_damage(16.f),
		m_projType(ProjType::Standard),
		m_speedFactor(1.f),
		m_ammo(Constants::c_fastReloadCount),
		m_item(nullptr),
		m_lightState(Pawn::OnlyFire),
		m_playerColor(PlayerColor::White)
	{
		m_isStatic = false;
		m_collisionType = CollisionType::Player;

		m_soundFire.setBuffer(*g_resourceManager->getSound("sound_shot"));
		m_soundAltFire.setBuffer(*g_resourceManager->getSound("sound_altShot"));
		m_soundCollision.setBuffer(*g_resourceManager->getSound("sound_collision"));
		m_soundExplode.setBuffer(*g_resourceManager->getSound("sound_explode"));
		m_soundTakeItem.setBuffer(*g_resourceManager->getSound("sound_item_collect"));

		m_weaponSprite.setOrigin(sf::Vector2f(float(m_weaponSprite.getTextureRect().width)
			, m_weaponSprite.getTextureRect().height * 0.5f));
		m_healthBarSprite.rotate(180.f);
		m_healthBarSprite.setColor(sf::Color(138, 240, 12, 255));

	//	m_sprite.setScale(Constants::c_scaleFactor, c_scaleFactor);
		m_healthBarSprite.setScale(sf::Vector2f(Constants::c_scaleFactor, Constants::c_scaleFactor));
		m_weaponSprite.setScale(sf::Vector2f(Constants::c_scaleFactor, Constants::c_scaleFactor));
		m_weaponSprite.setColor(sf::Color(228, 10, 255, 255));

		m_healthRect = m_healthBarSprite.getTextureRect();
		m_healthRectDef = m_healthRect.height;

		m_weaponRect = m_weaponSprite.getTextureRect();
		m_weaponRectDef = m_weaponRect.width;
		//radius of the final scaled sprite
		m_boundingRad = (float)m_sprite.getTexture()->getSize().x * m_sprite.getScale().x * 0.5f;

		
		m_lightInfo->color = Color(255, 255, 255, 0);
		m_lightInfo->radius = m_boundingRad * 6.f;
	}
	// ************************************************ //
	void Pawn::process()
	{
	/*	if (m_isDead)
		{
			if (alphaVal() < Constants::c_deathTreshhold)
			{
				destroy();
			}
			return;
		}*/


		Actor::process();

		//fading
		//handled in the light shader now
	//	m_alpha *= m_fadeFactor;
	//	if (m_alpha < 0.1f) m_alpha = 1.f;

		--m_cd;
		if (m_cd <= 0)
		{
			++m_ammo;
			m_cd = static_cast<int>(m_cdMax * std::max(static_cast<float>(m_ammo) / Constants::c_fastReloadCount, 1.f));
		}

		if (m_item)
		{
			m_item->setPosition(m_position);
		}
	}

	// ******************************************** **//

	void Pawn::draw(sf::RenderWindow& _window)
	{
		m_lightInfo->setPosition(m_position);

		if(m_lightState != On) m_alpha *= m_fadeFactor;
		uint8_t a = (uint8_t)(m_alpha * 255.f);

		m_lightInfo->color.a = a;

	//	m_sprite.setColor(sf::Color(255, 255, 255, a));
		Actor::draw(_window);

		//health bar size depends on current health
		m_healthBarSprite.setPosition(m_position);
//		m_healthBarSprite.setColor(sf::Color(138, 240, 12, a));
		m_healthRect.height = static_cast<int>(m_healthRectDef * (m_health / m_healthMax));
		m_healthBarSprite.setTextureRect(m_healthRect);

		_window.draw(m_healthBarSprite);

		// weapon points in the direction the player is facing
		m_weaponSprite.setPosition(m_position);
		m_weaponRect.width = static_cast<int>(m_weaponRectDef 
			* std::min(m_ammo + (m_cdMax - m_cd) / static_cast<float>(m_cdMax),
			static_cast<float>(Constants::c_fastReloadCount)) 
			/ Constants::c_fastReloadCount + 1.f);
	//	m_weaponRect.left = m_weaponRectDef - m_weaponRect.width;
		m_weaponSprite.setTextureRect(m_weaponRect);
	//	m_weaponSprite.setColor(sf::Color(228, 10, 255, a));
		m_weaponSprite.setRotation(m_dirAngle / (2.f*3.1415f) * 360.f + 180.f);
		_window.draw(m_weaponSprite);
	}

	void Pawn::collision(Actor& _oth)
	{
		//make no sound while running against a wall
		if (!_oth.isStatic())
		{
			_oth.damage(1.f);
			m_soundCollision.play();
		}
		m_lastHitId = _oth.getCId();
	}

	void Pawn::stopSounds()
	{
	}

	void Pawn::onDestroy()
	{
		//destroy stuff depended on this pawn
		if (m_item) m_item->destroy();
		m_lightInfo.release();
	}

	void Pawn::onDeath()
	{
		m_soundExplode.play(); 
		//red circle
		m_alpha = 1.f;
		m_fadeFactor = Constants::c_playerBaseFadeFactor + 0.019f;
		m_lightInfo->radius *= 1.5f;
		m_lightInfo->color.g = 50;
		m_lightInfo->color.b = 50;
		m_canCollide = false;
		m_lightState = Off;

		m_velocity = Vector2f(0.f, 0.f);

		Stats::g_statManager.add(m_cid, Stats::Stat::Deaths);
		Stats::g_statManager.add(m_lastHitId, Stats::Stat::Kills);
	}

	void Pawn::onDamage()
	{
		m_alpha = std::max(m_alpha, 0.5f);
	}

	void Pawn::fire()
	{
		if (m_ammo > 0)
		{
			if (m_lightState == OnlyFire || m_lightState == On) m_alpha = 1.f;
			//spawn projectile
			Vector2f dir = normalize(Vector2f(cos(m_dirAngle), sin(m_dirAngle)));
			g_projectileFactory.spawn(m_position + dir * (m_boundingRad + Constants::c_projectileRadius + 6.f),
 				dir, m_projType, m_cid);
			m_soundFire.play();

			--m_ammo;
			Stats::g_statManager.add(m_cid, Stats::Stat::ShotsFired);
		}
	}

	void Pawn::altFire()
	{
		if (!m_item) return;
		m_item->use();
		m_item = nullptr;
		m_soundAltFire.play();

		Stats::g_statManager.add(m_cid, Stats::Stat::ItemsUsed);
	}

	void Pawn::takeItem(Item& _itm)
	{
		if (m_item) m_item->destroy();
		m_item = &_itm;
		m_soundTakeItem.play();
	}
}