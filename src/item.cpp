#include <SFML/Audio.hpp>

#include "item.hpp"
#include "pawn.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"

namespace Game{
	Item::Item(const sf::Vector2f& _pos, sf::Texture& _texture):
		Actor(_pos, _texture),
		m_cd(0xfffffff),
		m_pawn(nullptr),
		m_lightInfo(Graphic::g_lightSystem.createLight())
	{
		m_boundingRad = 10.f;

		m_lightInfo->color = sf::Color(100, 255, 12, 255);
		m_lightInfo->radius = 80.f;
		m_lightInfo->setPosition(_pos);

		m_soundAppear.setBuffer(*g_resourceManager.getSound("sound_itemAppear"));
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
		Item(_pos, *g_resourceManager.getTexture("powerup_mine.png")),
		m_state(Pickable)
	{
		m_activeTime = c_mineChargeTime;
	}

	void Mine::collision(Actor& _oth)
	{
		if (m_state == Pickable) Item::collision(_oth);
		else if (m_state == Active)
		{
			_oth.damage(150.f);
			destroy();
		}
	}

	void Mine::use()
	{
		Item::use();
		m_sprite.setColor(sf::Color(0, 0, 255, 255));
		m_state = Ticking;
	}

	void Mine::endUse()
	{
		m_sprite.setColor(sf::Color(255, 210, 255, 255));
		m_sprite.scale(2.2f, 2.2f);
		m_state = Active;
		m_canCollide = true;
	}

	// *********************************************************** //

	Sentinel::Sentinel(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager.getTexture("sentinel.png"))
	{
		m_activeTime = c_sentinelChargeTime;
	}

	void Sentinel::collision(Actor& _oth)
	{
		Item::collision(_oth);
	}

	void Sentinel::use()
	{
		Item::use();
		m_state = Ticking;
	}

	void Sentinel::endUse()
	{
		if (m_state == Ticking)
		{
			m_sprite.setColor(sf::Color(255, 255, 12, 255));
			m_lightInfo = Graphic::g_lightSystem.createLight();
			m_lightInfo->color = sf::Color(255, 255, 255, 255);
			m_lightInfo->setPosition(m_position);
			m_lightInfo->radius = 256.f;

			m_state = Active;
			SetTimer(c_sentinelActiveTime);
		}
		else if (m_state == Active)
		{
			m_lightInfo.release();
			m_sprite.setColor(sf::Color(255, 210, 255, 255));
			m_sprite.scale(2.2f, 2.2f);
			destroy();
		}
	}

	// *********************************************************** //

	ClusterGun::ClusterGun(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager.getTexture("powerup_particle_gun.png"))
	{
		m_activeTime = c_clusterGunActiveTime;
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
		Item(_pos, *g_resourceManager.getTexture("powerup_aura.png"))
	{
		m_activeTime = c_lightAuraActiveTime;
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
		Item(_pos, *g_resourceManager.getTexture("powerup_health.png"))
	{
		m_activeTime = 1;
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
		Item(_pos, *g_resourceManager.getTexture("powerup_laser.png"))
	{
		m_activeTime = c_speedBonusActiveTime;
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
		Item(_pos, *g_resourceManager.getTexture("powerup_health.png"))
	{
		m_sprite.setColor(sf::Color(0, 0, 255, 255));
		m_activeTime = c_shieldActiveTime;
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