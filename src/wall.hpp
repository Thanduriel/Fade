#pragma once

#include "actor.hpp"

namespace Game{
	class Wall : public Actor
	{
	public:
		Wall(const sf::Vector2f& _pos, const sf::Vector2f& _size);

		void collision(Actor& _oth) override;
		//sprite does not need to be rendered, they cast shadows
		virtual void draw(sf::RenderWindow& _window) override {};
		virtual bool testComplexCollision(Actor& _oth) override;

		const sf::Vector2f& bbBegin() { return m_bbBegin; }
		const sf::Vector2f& bbEnd() { return m_bbEnd; }
	private:
		//solves collision between the line vw and the circle _act
		bool lineCollision(const sf::Vector2f& v, const sf::Vector2f& w, Actor& _act);

		//actual bounding box
		sf::Vector2f m_bbBegin;
		sf::Vector2f m_bbEnd;
	};
}