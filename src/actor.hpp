#pragma once

#include <SFML/Audio.hpp>

#include "centeredsprite.hpp"

namespace Game{

	class Actor
	{
	public:
		Actor(const sf::Vector2f& _position, sf::Texture& _texture);
		virtual ~Actor(){};

		virtual void process();
		virtual void draw(sf::RenderWindow& _window);
		virtual void onDestroy(){};
		virtual void stopSounds(){};
		virtual void onDeath(){};
		virtual void onDamage(){};
		virtual void collision(Actor& _oth){};

		// only used when hasComplexShape() true
		// @return Whether a collision actually occured
		virtual bool testComplexCollision(Actor& _oth){ return true; };

		//apply damage to this actor
		void damage(float _amount);
		void addHealth(float _amount) { m_health = std::min(m_health + _amount, m_healthMax); };

		void setPosition(const sf::Vector2f& _pos) { m_position = _pos; }
		//set angle in radients
		void setDirAngle(float _angle) { m_dirAngle = _angle; };

		const sf::Vector2f& position() { return m_position; }
		const sf::Vector2f& velocity() { return m_velocity; }
		float boundingRad() { return m_boundingRad; }
		bool isStatic() { return m_isStatic; }
		bool canCollide() { return m_canCollide; }

		bool hasComplexShape() { return m_hasComplexShape; }

		void destroy() { m_isDestroyed = true; onDestroy(); }
		bool isDestroyed() { return m_isDestroyed; }

		bool isDead() { return m_isDead; }
		void setReflective(bool _reflective) { m_isReflective = _reflective; }
		bool isReflective() { return m_isReflective; }
	protected:
		Graphic::CenteredSprite m_sprite;
		sf::Vector2f m_position;
		sf::Vector2f m_velocity;
		float m_dirAngle;

		float m_health;
		float m_healthMax;

		float m_boundingRad;
		bool m_hasComplexShape;
		bool m_isStatic;
		bool m_canCollide;
		bool m_isDestroyed;
		bool m_isDead;
		bool m_isReflective; // reflects projectiles
	};
}