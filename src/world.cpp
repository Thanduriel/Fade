#include <algorithm>

#include "world.hpp"
#include "resourcemanager.hpp"
#include "math.hpp"
#include "wall.hpp"
#include "projectile.hpp"
#include "constants.hpp"
#include "item.hpp"

using namespace sf;

namespace Game{
	World::World()
	{
		// determine number of connected joysticks/gamepads with Axis
		for (uint32_t i(0); i<5; i++)
		{
			if (sf::Joystick::isConnected(i))
			{
				if (sf::Joystick::isConnected(i) &&
					sf::Joystick::hasAxis(i, sf::Joystick::X) &&
					sf::Joystick::hasAxis(i, sf::Joystick::Y) &&
					sf::Joystick::hasAxis(i, sf::Joystick::U) &&
					sf::Joystick::hasAxis(i, sf::Joystick::R))
				{
					Pawn* player = new Pawn(sf::Vector2f(500.f, 500.f),
						*g_resourceManager.getTexture("player_main.png"));
					Controller* controller = new PlayerController(i);
					controller->possess(player);
					m_gameObjects.emplace_back(player);
					m_controllers.emplace_back(controller);
				}
			}
		}
		Texture* texture = g_resourceManager.getTexture("ground.png");
		texture->setRepeated(true);
		m_ground.setTexture(*texture);
		IntRect rect;
		rect.left = 0;
		rect.top = 0;
		rect.width = Constants::c_windowSizeX;
		rect.height = Constants::c_windowSizeY;
		m_ground.setTextureRect(rect);
		//test stuff

		Pawn* player = new Pawn(sf::Vector2f(900.f, 450.f),
			*g_resourceManager.getTexture("player_main.png"));
		m_gameObjects.emplace_back(player);
		Controller* controller = new PlayerController(0);
		controller->possess(player);
		m_controllers.emplace_back(controller);

		m_gameObjects.emplace_back(new Pawn(sf::Vector2f(600.f, 800.f),
			*g_resourceManager.getTexture("player_main.png")));

		m_gameObjects.emplace_back(new Mine(sf::Vector2f(123.f, 321.f)));
		m_gameObjects.emplace_back(new ClusterGun(sf::Vector2f(123.f, 421.f)));
		// Pawn* player = new Pawn(sf::Vector2f(900.f, 450.f),
		// 	*g_resourceManager.getTexture("player_main.png"));
		// Controller* controller = new PlayerController(0);
		// controller->possess(player);
		// m_gameObjects.emplace_back(player);
		// m_controllers.emplace_back(controller);
		// m_gameObjects.emplace_back(new Wall(sf::Vector2f(700.f, 400.f), sf::Vector2f(300.f, 10.f)));

		clock.restart();
	}

	void World::processEvent(sf::Event& _event)
	{
		if (_event.type == sf::Event::JoystickButtonPressed)
			for (auto& controller : m_controllers) controller->processEvent(_event);
	}

	void World::process()
	{
		for (auto& controller : m_controllers) controller->process();
		for (auto& actor : m_gameObjects) actor->process();

		//collect projectiles
		for (auto proj : g_projectileFactory.collect())
			m_gameObjects.emplace_back(proj);
		g_projectileFactory.clear();

		//test for intersections
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			auto& first = *m_gameObjects[i];
			if (first.isStatic() || !first.canCollide()) continue;
			for (size_t j = i+1; j < m_gameObjects.size(); ++j)
			{
				auto& second = *m_gameObjects[j];
				if (!second.canCollide()) continue;

				float r2 = first.boundingRad() + second.boundingRad();
				r2 *= r2;
				if (distSq(first.position(), second.position()) < r2)
				{
					first.collision(second);
					second.collision(first);
				}
			}

			Vector2f newPos = first.position();
			//check map boundries
			if (first.position().x - first.boundingRad() < 0.f) newPos.x = first.boundingRad();
			else if (first.position().x + first.boundingRad() > Constants::c_windowSizeX) newPos.x = Constants::c_windowSizeX - first.boundingRad();
			if (first.position().y - first.boundingRad() < 0.f) newPos.y = first.boundingRad();
			else if (first.position().y + first.boundingRad() > Constants::c_windowSizeY) newPos.y = Constants::c_windowSizeY - first.boundingRad();

			first.setPosition(newPos);
		}

		auto it = std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
			[](const std::unique_ptr<Actor>& _ptr){return _ptr->isDestroyed(); });

		m_gameObjects.resize(std::distance(m_gameObjects.begin(), it));

	}

	void World::draw(sf::RenderWindow& _window)
	{
		_window.draw(m_ground);
		for (auto& actor : m_gameObjects) actor->draw(_window);
	}

}