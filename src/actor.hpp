#pragma once

#include <SFML/Audio.hpp>

#include "centeredsprite.hpp"

namespace Game{

	class Actor
	{
	public:
		Actor(const sf::Vector2f& _position, sf::Texture& _texture);

		virtual void process();
		virtual void draw(sf::RenderWindow& _window);
		virtual void onDestroy(){};
		virtual void stopSounds(){};

		virtual void collision(Actor& _oth){};

		void damage(float _amount);
		void addHealth(float _amount) { m_health = std::max(m_health + _amount, m_healthMax); };

		void setPosition(const sf::Vector2f& _pos) { m_position = _pos; }
		//set angle in radients
		void setDirAngle(float _angle) { m_dirAngle = _angle; };

		const sf::Vector2f& position() { return m_position; }
		const sf::Vector2f& velocity() { return m_velocity; }
		float boundingRad() { return m_boundingRad; }
		bool isStatic() { return m_isStatic; }
		bool canCollide() { return m_canCollide; }

		void destroy() { m_isDestroyed = true; }
		bool isDestroyed() { return m_isDestroyed; }

		bool isDeath() { return m_isDeath; }
	protected:
		Graphic::CenteredSprite m_sprite;
		sf::Vector2f m_position;
		sf::Vector2f m_velocity;
		float m_dirAngle;

		float m_health;
		float m_healthMax;

		float m_boundingRad;
		bool m_isStatic;
		bool m_canCollide;
		bool m_isDestroyed;
		bool m_isDeath;
	};
}