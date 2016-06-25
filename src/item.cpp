#include "item.hpp"
#include "pawn.hpp"
#include "resourcemanager.hpp"

namespace Game{
	Item::Item(const sf::Vector2f& _pos, sf::Texture& _texture):
		Actor(_pos, _texture)
	{
		m_boundingRad = 10.f;
	};

	void Item::collision(Actor& _oth)
	{
		Pawn& pawn = reinterpret_cast<Pawn&>(_oth);
		m_canCollide = false;
		pawn.takeItem(*this);
	}

	Mine::Mine(const sf::Vector2f& _pos) : 
		Item(_pos, *g_resourceManager.getTexture("player_main.png")),
		m_state(Pickable),
		m_cd(120)
	{

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
		m_state = Ticking;
	}

	void Mine::process()
	{

		if (m_state == Ticking)
		{
			--m_cd;
			if (m_cd == 0) {
				m_state = Active;
				m_canCollide = true;
			}
		}
	}
}