#include <iostream>

#include "mainstate.hpp"
#include "poststate.hpp"
#include "lightsys.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "stats.hpp"

namespace State{
	MainState::MainState(const GameSettings& _gameSettings) :
		m_gameSettings(_gameSettings),
		m_gameTime(0),
		m_world(Constants::g_windowSizeX, Constants::g_windowSizeY)
	{
		Stats::g_statManager.Reset();
	}

	void MainState::process()
	{
		m_gameTime++;
		m_world.process();

		//check end condition
		if (m_gameSettings.winCondition == WinCondition::Time)
		{
			if (m_gameTime / 60 >= m_gameSettings.value)
			{
				m_finished = true;
				m_newState = new State::PostState();
			}
		}
		else
		{
		//	Stats::g_statManager.sort(Stats::Stat::Kills);
			if (Stats::g_statManager.getMax(Stats::Stat::Kills) >= m_gameSettings.value)
			{
				m_finished = true;
				m_newState = new State::PostState();
			}
		}

		if (m_newState || m_finished)
        {
        	this->m_world.stopSounds();
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
			m_world.addNewPlayer(_event.joystickConnect.joystickId);
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
		m_world.resize(g_worldScale * g_windowSizeX, g_worldScale * g_windowSizeY);
		m_world.generateWalls(g_numWalls);
	}
}