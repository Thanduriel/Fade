#pragma once

#include "centeredsprite.hpp"

namespace Game{

	class Actor
	{
	public:
		Actor(const sf::Vector2f& _position, sf::Texture& _texture);

		virtual void process();
		virtual void draw(sf::RenderWindow& _window);

		virtual void collision(Actor& _oth){};

		void damage(float _amount);

		void setPosition(const sf::Vector2f& _pos) { m_position = _pos; }
		//set angle in radients
		void setDirAngle(float _angle) { m_dirAngle = _angle; };

		const sf::Vector2f& position() { return m_position; }
		const sf::Vector2f& velocity() { return m_velocity; }
		float boundingRad() { return m_boundingRad; }
		bool isStatic() { return m_isStatic; }
		bool isDestroyed() { return m_isDestroyed; }
	protected:
		Graphic::CenteredSprite m_sprite;
		sf::Vector2f m_position;
		sf::Vector2f m_velocity;
		float m_dirAngle;

		float m_health;

		float m_boundingRad;
		bool m_isStatic;
		bool m_isDestroyed;
	};
}