#include "world.hpp"
#include "resourcemanager.hpp"
#include "math.hpp"
#include "wall.hpp"

using namespace sf;

namespace Game{
	World::World()
	{
		//test stuff

		sf::Texture texture;
		texture.create(16, 16);
		m_gameObjects.emplace_back(new Actor(sf::Vector2f(10.f, 10.f), texture));

		Pawn* player = new Pawn(sf::Vector2f(200.f, 200.f),
			*g_resourceManager.getTexture("player_main.png"));
		m_gameObjects.emplace_back(player);
		Controller* controller = new Controller();
		controller->possess(player);
		m_controllers.emplace_back(controller);

		m_gameObjects.emplace_back(new Pawn(sf::Vector2f(600.f, 800.f),
			*g_resourceManager.getTexture("player_main.png")));

		m_gameObjects.emplace_back(new Wall(sf::Vector2f(700.f, 400.f), sf::Vector2f(300.f, 10.f)));
	}

	void World::process()
	{
		for (auto& controller : m_controllers) controller->process();
		for (auto& actor : m_gameObjects) actor->process();

		//test for intersections
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			auto& first = *m_gameObjects[i];
			if (first.isStatic()) continue;
			for (size_t j = i+1; j < m_gameObjects.size(); ++j)
			{
				auto& second = *m_gameObjects[j];
				float r2 = first.boundingRad() + second.boundingRad();
				r2 *= r2;
				if (distSq(first.position(), second.position()) < r2)
				{
					first.collision(second);
					second.collision(first);
				}
			}
		}
	}

	void World::draw(sf::RenderWindow& _window)
	{
		for (auto& actor : m_gameObjects) actor->draw(_window);
	}

}