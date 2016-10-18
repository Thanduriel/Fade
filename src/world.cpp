#include <algorithm>
#include <assert.h>

#include "world.hpp"
#include "resourcemanager.hpp"
#include "math.hpp"
#include "wall.hpp"
#include "projectile.hpp"
#include "constants.hpp"
#include "item.hpp"
#include "utils.hpp"
#include "camera.hpp"

using namespace sf;

namespace Game{

	World::World(int _sizeX, int _sizeY)
	{
		Texture* texture = g_resourceManager.getTexture("ground.png");
		texture->setRepeated(true);
		m_ground.setTexture(*texture);

		using namespace Constants;
		resize(g_worldScale * g_windowSizeX, g_worldScale * g_windowSizeY);
		generateWalls(g_numWalls);

		// m_gameObjects[0] should always be a dummy that objects can collide with 
		// when reaching the map boundaries
		m_collisionDummy = std::make_unique<Actor>(Vector2f(-100.f, -100.f), *g_resourceManager.getTexture("wall.png"));

		// determine number of connected joysticks/gamepads with Axis
	/*	for (uint32_t i(0); i<5; i++)
		{
			if (sf::Joystick::isConnected(i))
			{
				if (sf::Joystick::isConnected(i) &&
					sf::Joystick::hasAxis(i, sf::Joystick::X) &&
					sf::Joystick::hasAxis(i, sf::Joystick::Y) &&
					sf::Joystick::hasAxis(i, sf::Joystick::U) &&
					sf::Joystick::hasAxis(i, sf::Joystick::R))
				{
					addNewPlayer(i);
				}
			}
		}*/

#ifdef _DEBUG
		addNewPlayer(-1);
#endif
		//test stuff
		/*
		m_gameObjects.emplace_back(new Sentinel(sf::Vector2f(123.f, 121.f)));
		m_gameObjects.emplace_back(new HealthBoost(sf::Vector2f(123.f, 221.f)));
		m_gameObjects.emplace_back(new Mine(sf::Vector2f(123.f, 321.f)));
		m_gameObjects.emplace_back(new ClusterGun(sf::Vector2f(123.f, 421.f)));
		m_gameObjects.emplace_back(new LightAura(sf::Vector2f(123.f, 521.f)));
		*/
		clock.restart();
	}

	// *************************************************** //

	void World::processEvent(sf::Event& _event)
	{
		if (_event.type == sf::Event::JoystickButtonPressed)
			for (auto& controller : m_controllers) controller->processEvent(_event);
	}

	// *************************************************** //

	void World::process()
	{
		// game events
		++m_frameCount;
		if (m_frameCount % Constants::c_eventFrequency == 0)
			for (uint32_t i = 0; i < util::rand(3, 1); ++i) spawnItem();

		for (auto& controller : m_controllers)
		{
			controller->process();
			// revive
			if (!controller->hasPawn())
			{
				controller->possess(spawnPlayer(controller->getId()));
				controller->getPawn()->switchColor(controller->getColorId());
			}
		}
		for (auto& actor : m_gameObjects) actor->process();

		//collect projectiles
		for (auto proj : g_projectileFactory.collect())
			m_gameObjects.emplace_back(proj);
		g_projectileFactory.clear();

		//test for intersections
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			auto& first = *m_gameObjects[i];
			if (!first.canCollide()) continue;
			for (size_t j = i+1; j < m_gameObjects.size(); ++j)
			{
				auto& second = *m_gameObjects[j];
				if (!COLLISIONTABLE[(int)first.collisionType()][(int)second.collisionType()]) continue;
				//todo remove this line should be obsolete
				if (first.isStatic() && second.isStatic() || !second.canCollide()) continue;

				float r2 = first.boundingRad() + second.boundingRad();
				r2 *= r2;
				if (distSq(first.position(), second.position()) < r2 
					&& (!first.hasComplexShape() || first.testComplexCollision(second)) 
					&& (!second.hasComplexShape() || second.testComplexCollision(first)))
				{
					first.collision(second);
					second.collision(first);
				}
			}

			//check map boundries
			if (first.isStatic()) continue;
			Vector2f newPos = first.position();

			if (first.position().x - first.boundingRad() < 0.f) newPos.x = first.boundingRad();
			else if (first.position().x + first.boundingRad() > m_sizeX) newPos.x = m_sizeX - first.boundingRad();
			if (first.position().y - first.boundingRad() < 0.f) newPos.y = first.boundingRad();
			else if (first.position().y + first.boundingRad() > m_sizeY) newPos.y = m_sizeY - first.boundingRad();

			if (newPos != first.position()) first.collision(*m_collisionDummy);
			first.setPosition(newPos);
		}

		auto it = std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
			[](const std::unique_ptr<Actor>& _ptr){return _ptr->isDestroyed(); });

		m_gameObjects.resize(std::distance(m_gameObjects.begin(), it));

	}

	// *************************************************** //

	void World::draw(sf::RenderWindow& _window)
	{
		_window.draw(m_ground);
		for (auto& actor : m_gameObjects) actor->draw(_window);
	}

	void World::stopSounds()
	{
		for (auto& actor : m_gameObjects) actor->stopSounds();
	}

	// *************************************************** //

	void World::resize(int _sizeX, int _sizeY)
	{		
		m_sizeX = _sizeX;
		m_sizeY = _sizeY;
		//ground texture
		IntRect rect;
		rect.left = 0;
		rect.top = 0;
		rect.width = _sizeX;
		rect.height = _sizeY;
		m_ground.setTextureRect(rect);

		//center camera on the world
		Graphic::g_camera.setCenter((float)_sizeX*0.5f, (float)_sizeY * 0.5f);
		//
		//Graphic::g_lightSystem.resize();

	}

	// *************************************************** //

	sf::Vector2f World::getDistantPosition(float _minDist)
	{
		_minDist *= _minDist; //compare sqr
		sf::Vector2f pos;
		bool isValid;
		do{
			pos.x = util::rand(m_sizeX - 50, 50);
			pos.y = util::rand(m_sizeY - 50, 50);

			isValid = true;
			for (auto& actor : m_gameObjects)
				if (!actor->isStatic() && distSq(actor->position(), pos) < _minDist)
				{
					isValid = false;
					break;
				}

			if (isValid)
			{
				m_collisionDummy->setPosition(pos);
				//should not be inside a wall
				for (auto wall : m_walls)
				{
					if (wall->testComplexCollision(*m_collisionDummy))
					{
						isValid = false;
						break;
					}
				}
			}
		}while(!isValid);

		return pos;
	}

	// *************************************************** //

	void World::generateWalls(int _count)
	{
		for (auto wall : m_walls) wall->destroy();
		m_walls.clear();
		m_wallInfos.clear();
		m_wallInfos.reserve(_count * 3);
		for (int i = 0; i < _count; ++i)
		{
			Vector2f size;
			if (i % 2/*util::rand(1)*/)
				size = Vector2f((float)Constants::c_wallThickness, (float)util::rand(Constants::c_wallMaxLength, 50));
			else
				size = Vector2f((float)util::rand(Constants::c_wallMaxLength, 50), (float)Constants::c_wallThickness);
			Vector2f center(util::rand(m_sizeX - (int)size.x / 2, (int)size.x / 2),
				util::rand(m_sizeY - (int)size.y / 2, (int)size.y / 2));
			Wall* wall = new Wall(center, size);
			m_gameObjects.emplace_back(wall);
			m_walls.push_back(wall);
			m_wallInfos.push_back(wall->bbBegin());
			m_wallInfos.push_back(wall->bbEnd());
			m_wallInfos.push_back(center); // center
		}

		Graphic::g_lightSystem.setWalls(m_wallInfos.data(), m_wallInfos.size(), (float)Constants::g_windowSizeX / (float)m_sizeX);
	}

	// *************************************************** //

	void World::spawnItem()
	{
		sf::Vector2f pos = getDistantPosition(50.f);

		Item* item;
		switch (util::rand(6))
		{
		case 0: item = new Mine(pos); break;
		case 1: item = new LightAura(pos); break;
		case 2: item = new HealthBoost(pos); break;
		case 3: item = new SpeedBoost(pos); break;
		case 4: item = new ClusterGun(pos); break;
		case 5: item = new Sentinel(pos); break;
		case 6: item = new Shield(pos); break;
		}

		assert(item);
		m_gameObjects.emplace_back(item);
	}

	// *************************************************** //

	Pawn* World::spawnPlayer(int _cid)
	{
		Pawn* player = new Pawn(getDistantPosition(200.f),
			*g_resourceManager.getTexture("player_main.png"), _cid);
		m_gameObjects.emplace_back(player);

		return player;
	}

	// *************************************************** //

	void World::addNewPlayer(int _id, Pawn* _parent)
	{
		Controller* controller = _id >= 0 ? (Controller*)new PlayerController(_id)
			: new AiController();
		controller->possess(spawnPlayer(_id));
		m_controllers.emplace_back(controller);

		if (_parent)
		{
			controller->setColorId(_parent->getColorId());
			controller->getPawn()->switchColor(_parent->getColorId());
		}
	}

	// *************************************************** //

	void World::removePlayer(int _id)
	{
		auto it = std::find_if(m_controllers.begin(), m_controllers.end(),
			[=](const std::unique_ptr<Controller>& _controller){return _controller->getId() == _id; });

		if (it != m_controllers.end() && (*it)->hasPawn())
		{
			(*it)->getPawn()->damage(42000.f); //should definitely kill him
			m_controllers.erase(it);
		}
	}
}