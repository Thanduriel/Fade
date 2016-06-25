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
		World();

		void process();
		void processEvent(sf::Event& _keyEvent);
		void draw(sf::RenderWindow& _window);

	private:
		std::vector < std::unique_ptr < Actor > > m_gameObjects;
		sf::Sprite m_ground;
		std::vector < std::unique_ptr < Controller > > m_controllers;
		sf::Clock clock;
	};
}