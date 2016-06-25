#pragma once

#include "actor.hpp"

namespace Game{
	class Wall : public Actor
	{
	public:
		Wall(const sf::Vector2f& _pos, const sf::Vector2f& _size);

		void collision(Actor& _oth) override;

	private:
		//actual bounding box
		sf::Vector2f m_bbBegin;
		sf::Vector2f m_bbEnd;
	};
}