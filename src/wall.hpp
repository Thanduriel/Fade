#pragma once

#include "actor.hpp"

namespace Game{
	class Wall : public Actor
	{
	public:
		Wall(const sf::Vector2f& _pos, const sf::Vector2f& _size);

		void collision(Actor& _oth) override;

	private:
		//solves collision between the line vw and the circle _act
		void lineCollision(const sf::Vector2f& v, const sf::Vector2f& w, Actor& _act);

		//actual bounding box
		sf::Vector2f m_bbBegin;
		sf::Vector2f m_bbEnd;
	};
}