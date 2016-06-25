#include "item.hpp"
#include "pawn.hpp"
#include "resourcemanager.hpp"

namespace Game{
	Item::Item(const sf::Vector2f& _pos, sf::Texture& _texture):
		Actor(_pos, _texture),
		m_cd(0xfffffff),
		m_pawn(nullptr)
	{
		m_boundingRad = 10.f;
	};

	void Item::collision(Actor& _oth)
	{
		Pawn& pawn = reinterpret_cast<Pawn&>(_oth);
		m_canCollide = false;
		pawn.takeItem(*this);
		m_pawn = &pawn;
	}

	void Item::process()
	{
		--m_cd;
		if (m_cd == 0) endUse();
	}

	// *********************************************************** //

	Mine::Mine(const sf::Vector2f& _pos) : 
		Item(_pos, *g_resourceManager.getTexture("powerup_mine.png")),
		m_state(Pickable)
	{
		m_activeTime = 240;
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
		m_sprite.setColor(sf::Color(255, 0, 255, 255));
		m_state = Active;
		m_canCollide = true;	
	}

	// *********************************************************** //

	ClusterGun::ClusterGun(const sf::Vector2f& _pos) :
		Item(_pos, *g_resourceManager.getTexture("powerup_particle_gun.png"))
	{
		m_activeTime = 60 * 10;
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
}