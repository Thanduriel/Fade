#pragma once

#include <SFML/Audio.hpp>

#include "centeredsprite.hpp"

namespace Game{

	static const bool COLLISIONTABLE[5][5] = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 1,
		0, 0, 0, 1, 1,
		0, 0, 1, 1, 1,
		0, 1, 1, 1, 1
	};
	
	enum class CollisionType : int
	{
		None = 0,
		Item,
		Projectile,
		World,
		Player
	};

	class Actor
	{
	public:
		Actor(const sf::Vector2f& _position, sf::Texture& _texture, int _cid = -1);
		virtual ~Actor(){};

		int getCId() { return m_cid; }

		virtual void process();
		virtual void draw(sf::RenderWindow& _window);
		virtual void onDestroy(){};
		virtual void stopSounds(){};
		virtual void onDeath(){};
		virtual void onDamage(){};
		virtual void collision(Actor&){};

		// only used when hasComplexShape() true
		// @return Whether a collision actually occured
		virtual bool testComplexCollision(Actor&){ return true; };

		//apply damage to this actor
		void damage(float _amount);
		void addHealth(float _amount) { m_health = std::min(m_health + _amount, m_healthMax); };

		void setPosition(const sf::Vector2f& _pos) { m_position = _pos; }
		//set angle in radients
		void setDirAngle(float _angle) { m_dirAngle = _angle; };

		const sf::Vector2f& position() const { return m_position; }
		const sf::Vector2f& velocity() const { return m_velocity; }
		float boundingRad() { return m_boundingRad; }
		bool isStatic() const { return m_isStatic; }
		bool canCollide() const { return m_canCollide; }
		CollisionType collisionType() { return m_collisionType; }

		bool hasComplexShape() { return m_hasComplexShape; }

		void destroy() { m_isDestroyed = true; onDestroy(); }
		bool isDestroyed() { return m_isDestroyed; }

		bool isDead() const { return m_isDead; }
		void setReflective(bool _reflective) { m_isReflective = _reflective; }
		bool isReflective() const { return m_isReflective; }

		void setColor(sf::Color _color) { m_sprite.setColor(_color); };
	protected:
		int m_cid; //id of the related controller; -1 for world objects

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
		CollisionType m_collisionType;
		bool m_isDestroyed;
		bool m_isDead;
		bool m_isReflective; // reflects projectiles
	};
}