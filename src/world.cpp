#include <algorithm>
#include <assert.h>

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
					Pawn* player = new Pawn(getDistantPosition(200.f),
							*g_resourceManager.getTexture("player_main.png"));
					Controller* controller = new PlayerController(i);
					controller->possess(player);
					m_gameObjects.emplace_back(player);
					m_controllers.emplace_back(controller);
				}
			}
		}
		//ground texture
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

		m_gameObjects.emplace_back(new SpeedBoost(sf::Vector2f(123.f, 121.f)));
		m_gameObjects.emplace_back(new HealthBoost(sf::Vector2f(123.f, 221.f)));
		m_gameObjects.emplace_back(new Mine(sf::Vector2f(123.f, 321.f)));
		m_gameObjects.emplace_back(new ClusterGun(sf::Vector2f(123.f, 421.f)));
		m_gameObjects.emplace_back(new LightAura(sf::Vector2f(123.f, 521.f)));

		clock.restart();
	}

	void World::processEvent(sf::Event& _event)
	{
		if (_event.type == sf::Event::JoystickButtonPressed)
			for (auto& controller : m_controllers) controller->processEvent(_event);
	}

	void World::process()
	{
		// game events
		++m_frameCount;
		if (m_frameCount % Constants::c_eventFrequency == 0) spawnItem();

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


	inline unsigned int rnd()
	{
		static unsigned int num = 0x13624F13;
		num ^= num << 13;
		num ^= num >> 17;
		num ^= num << 5;

		return num;
	}

	inline uint32_t rand(uint32_t _max, uint32_t _min = 0)
	{
		return(rnd() % (_max + 1 - _min)) + _min;
	}

	sf::Vector2f World::getDistantPosition(float _minDist)
	{
		_minDist *= _minDist; //compare sqr
		sf::Vector2f pos;
		bool isValid;
		do{
			pos.x = rand(Constants::c_windowSizeX-10, 10);
			pos.y = rand(Constants::c_windowSizeY-10, 10);

			isValid = true;
			for (auto& actor : m_gameObjects)
				if (!actor->isStatic() && distSq(actor->position(), pos) < _minDist)
				{
					isValid = false;
					break;
				}
		}while(!isValid);

		return pos;
	}

	void World::spawnItem()
	{
		sf::Vector2f pos = getDistantPosition(50.f);

		Item* item;
		switch (rand(3))
		{
		case 0: item = new Mine(pos); break;
		case 1: item = new LightAura(pos); break;
		case 2: item = new HealthBoost(pos); break;
		case 3: item = new SpeedBoost(pos); break;
		}

		assert(item);
		m_gameObjects.emplace_back(item);
	}
}