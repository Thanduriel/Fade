#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

// #include "LTBL\Light\LightSystem.h"

#include "actor.hpp"
#include "controller.hpp"

namespace Game{
	class World
	{
	public:
		World(int _sizeX, int _sizeY);

		void process();
		void processEvent(sf::Event& _keyEvent);
		void draw(sf::RenderWindow& _window);
		void stopSounds();

		void resize(int _sizeX, int _sizeY);

		// returns a position distant to existing game objects
		// @param _minDist minimum distance to dynamic game objects
		sf::Vector2f getDistantPosition(float _minDist);

		void spawnItem();
		//events will spawn [_amount, _amount * 2] items
		void setItemSpawn(int _amount) { m_itemSpawnFreq = _amount; };

		// removes walls and generates _count new ones
		// amount is currently limited by the light fragment shader to 3
		void generateWalls(int _count);
		Pawn* spawnPlayer(int _cid);
		void addNewPlayer(int _id, PlayerColor _color);
		void removePlayer(int _id);
	private:
		sf::Sprite m_ground;
		int m_sizeX, m_sizeY;

		std::unique_ptr< Actor > m_collisionDummy;
		std::vector < std::unique_ptr < Actor > > m_gameObjects;
		std::vector < std::unique_ptr < Controller > > m_controllers;
		std::vector < sf::Glsl::Vec2 > m_wallInfos;
		std::vector < class Wall* > m_walls;
		sf::Clock clock;
		int m_frameCount;
		int m_itemSpawnFreq;
	};
}