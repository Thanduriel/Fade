#include <iostream>

#include "mainstate.hpp"
#include "poststate.hpp"
#include "lightsys.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "stats.hpp"
#include "resourcemanager.hpp"

namespace State{
	MainState::MainState(const GameSettings& _gameSettings,
		const std::vector < std::unique_ptr< Game::Pawn >>& _players) :
		m_gameSettings(_gameSettings),
		m_gameTime(0),
		m_world(Constants::g_windowSizeX, Constants::g_windowSizeY),
		m_gameOver(false)
	{
		Stats::g_statManager.Reset();

		m_gameEndSound.setBuffer(*g_resourceManager.getSound("sound_newWorld"));

		m_world.setItemSpawn(_gameSettings.itemSpawns);
		for (auto& playerInfo : _gameSettings.playerInfos)
		{
			m_world.addNewPlayer(playerInfo.cid, playerInfo.playerColor);
			Stats::g_statManager.Add(playerInfo.cid, Stats::Stat::IsActive);
		}
	}

	void MainState::process()
	{
		m_gameTime++;
		if (!m_gameOver)
		{
			m_world.process();

			//check end condition
			if (m_gameSettings.winCondition == WinCondition::Time)
			{
				if (m_gameTime / 60 >= m_gameSettings.value)
				{
					gameOver();
				}
			}
			else
			{
				//	Stats::g_statManager.sort(Stats::Stat::Kills);
				if (Stats::g_statManager.getMax(Stats::Stat::Kills) >= m_gameSettings.value)
				{
					gameOver();
				}
			}
		}
		else
		{
			processGameOver();
		}

		if (m_newState || m_finished)
        {
        	m_world.stopSounds();
        }
	}

	void MainState::processEvents(sf::Event& _event)
	{
		m_world.processEvent(_event);

		switch(_event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (_event.key.code == sf::Keyboard::Escape)
			{
				m_finished = true;
			}
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			break;
		}
		case sf::Event::JoystickButtonPressed:
		{
			break;
		}
		case sf::Event::Closed:
		{
			m_finished = true;
			break;
		}
		case sf::Event::Resized:
		{
			break;
		}
		case sf::Event::JoystickConnected:
		{
			m_world.addNewPlayer(_event.joystickConnect.joystickId, Game::PlayerColor::White);
			break;
		}
		case sf::Event::JoystickDisconnected:
		{
			m_world.removePlayer(_event.joystickConnect.joystickId);
			break;
		}
		// Default case
		default:
		{
			break;
		}
		}// switch(sf_event.type)
	}

	void MainState::draw(sf::RenderWindow& _window)
	{
		_window.setView(Graphic::g_camera);
		m_world.draw(_window);
		
		//lightsystem is drawn in screen space
		//thus it changes the view to defaultView!
		Graphic::g_lightSystem.draw(_window);
	}

	using namespace Constants;
	void MainState::onActivate()
	{
	}

	// ************************************************* //

	void MainState::gameOver()
	{
		m_gameOver = true;
		m_gameEndTime = m_gameTime;
	//	m_gameEndSound.play();

		Graphic::g_lightSystem.setWalls(nullptr, 0);
		m_lightInfo = Graphic::g_lightSystem.createLight();
		m_lightInfo->radius = 100.f;
		m_lightInfo->color = sf::Color(255, 255, 255);
		m_lightInfo->position = sf::Vector2f(0.f, 0.f);
	}

	void MainState::processGameOver()
	{
		m_lightInfo->radius += 10;
		if (m_gameTime - m_gameEndTime < 60 * 5) return;

		m_finished = true;
		m_newState = new State::PostState(m_gameSettings);
	}
}