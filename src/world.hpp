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
		Pawn* spawnPlayer();
		void addNewPlayer(int _id);
		void removePlayer(int _id);
	private:
		sf::Sprite m_ground;
		int m_sizeX, m_sizeY;

		std::vector < std::unique_ptr < Actor > > m_gameObjects;
		std::vector < std::unique_ptr < Controller > > m_controllers;
		sf::Clock clock;
		int m_frameCount;
	};
}