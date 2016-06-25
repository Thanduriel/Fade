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
		void draw(sf::RenderWindow& _window);

	private:
		std::vector< std::unique_ptr < Actor > > m_gameObjects;
		std::vector < std::unique_ptr < Controller > > m_controllers;
	};
}