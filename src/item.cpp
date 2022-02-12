#include <SFML/Audio.hpp>

#include "item.hpp"
#include "pawn.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"

namespace Game{
	Item::Item(const sf::Vector2f& _pos, sf::Texture& _texture, int _activeTime = 0):
		Actor(_pos, _texture),
		m_activeTime(_activeTime),
		m_pawn(nullptr),
		m_lightInfo(Graphic::g_lightSystem->createLight()),
		m_cd(0xfffffff)
	{
		m_boundingRad = 10.f;

		m_lightInfo->color = sf::Color(100, 255, 12, 255);
		m_lightInfo->radius = 80.f;
		m_lightInfo->setPosition(_pos);

		m_soundAppear.setBuffer(*g_resourceManager->getSound("sound_itemAppear"));
		m_soundAppear.play();

		m_collisionType = CollisionType::Item;
	};

	void Item::collision(Actor& _oth)
	{
		if (_oth.isDead()) return;

		Pawn& pawn = reinterpret_cast<Pawn&>(_oth);
		m_canCollide = false;
		pawn.takeItem(*this);
		m_pawn = &pawn;

		m_lightInfo.release();
	}

	void Item::process()
	{
		m_sprite.rotate(1.f);
		--m_cd;
		if (m_cd == 0) endUse();
	}

	// *********************************************************** //

	using namespace Constants;

	Mine::Mine(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("powerup_mine.png")),
		m_state(State::Pickable)
	{
		m_activeTime = c_mineChargeTime;
	}

	void Mine::collision(Actor& _oth)
	{
		if (m_state == State::Pickable) Item::collision(_oth);
		else if (m_state == State::Active)
		{
			_oth.damage(150.f);
			destroy();
		}
	}

	void Mine::use()
	{
		Item::use();
		m_sprite.setColor(sf::Color(0, 0, 255, 255));
		m_state = State::Ticking;
	}

	void Mine::endUse()
	{
		m_sprite.setColor(sf::Color(255, 210, 255, 255));
		m_sprite.scale(sf::Vector2f(2.2f, 2.2f));
		m_state = State::Active;
		m_canCollide = true;
	}

	// *********************************************************** //

	Sentinel::Sentinel(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("sentinel.png"), c_sentinelChargeTime),
		m_state(State::Pickable)
	{
	}

	void Sentinel::collision(Actor& _oth)
	{
		Item::collision(_oth);
	}

	void Sentinel::use()
	{
		Item::use();
		m_state = State::Ticking;
	}

	void Sentinel::endUse()
	{
		if (m_state == State::Ticking)
		{
			m_sprite.setColor(sf::Color(255, 255, 12, 255));
			m_lightInfo = Graphic::g_lightSystem->createLight();
			m_lightInfo->color = sf::Color(255, 255, 255, 255);
			m_lightInfo->setPosition(m_position);
			m_lightInfo->radius = 256.f;

			m_state = State::Active;
			SetTimer(c_sentinelActiveTime);
		}
		else if (m_state == State::Active)
		{
			m_lightInfo.release();
			m_sprite.setColor(sf::Color(255, 210, 255, 255));
			m_sprite.scale(sf::Vector2f(2.2f, 2.2f));
			destroy();
		}
	}

	// *********************************************************** //

	ClusterGun::ClusterGun(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("powerup_particle_gun.png"), c_clusterGunActiveTime)
	{
	}

	void ClusterGun::use()
	{
		Item::use();
		m_pawn->setProjType(ProjType::Cluster);
	}

	void ClusterGun::endUse()
	{
		m_pawn->setProjType(ProjType::Standard);
		destroy();
	}

	// *********************************************************** //

	LightAura::LightAura(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("powerup_aura.png"), c_lightAuraActiveTime)
	{
	}

	void LightAura::use()
	{
		Item::use();

		m_pawn->getLightInfo().radius += c_lightAuraExtraRadius;
	}

	void LightAura::endUse()
	{
		m_pawn->getLightInfo().radius -= c_lightAuraExtraRadius;
		destroy();
	}

	// *********************************************************** //

	HealthBoost::HealthBoost(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("powerup_health.png"), 1)
	{
	}

	void HealthBoost::use()
	{
		Item::use();

		m_pawn->addHealth(100.f);
	}

	void HealthBoost::endUse()
	{
		destroy();
	}

	// *********************************************************** //

	SpeedBoost::SpeedBoost(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("powerup_laser.png"), c_speedBonusActiveTime)
	{
	}

	void SpeedBoost::use()
	{
		Item::use();

		m_pawn->setSpeedFactor(m_pawn->speedFactor() + c_speedBonusExtraSpeed);
	}

	void SpeedBoost::endUse()
	{
		m_pawn->setSpeedFactor(m_pawn->speedFactor() - c_speedBonusExtraSpeed);
		destroy();
	}

	// *********************************************************** //

	Shield::Shield(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager->getTexture("powerup_shield.png"), c_shieldActiveTime)
	{
	}

	void Shield::use()
	{
		Item::use();

		m_pawn->setReflective(true);
	}

	void Shield::endUse()
	{
		m_pawn->setReflective(false);
		destroy();
	}
}