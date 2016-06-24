#include "world.hpp"

using namespace sf;

namespace Game{
	World::World()
	{
		//test stuff

		sf::Texture texture;
		texture.create(16, 16);
		m_gameObjects.emplace_back(new Actor(sf::Vector2f(10.f, 10.f), texture));

		Pawn* player = new Pawn(sf::Vector2f(10.f, 10.f), texture);
		m_gameObjects.emplace_back(player);
		Controller* controller = new Controller();
		controller->possess(player);
		m_controllers.emplace_back(controller);
	}

	void World::process()
	{
		for (auto& controller : m_controllers) controller->process();
		for (auto& actor : m_gameObjects) actor->process();
	}

	void World::draw(sf::RenderWindow& _window)
	{
		for (auto& actor : m_gameObjects) actor->draw(_window);
	}

}