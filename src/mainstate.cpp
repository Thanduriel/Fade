#include <iostream>

#include "mainstate.hpp"
#include "lightsys.hpp"

namespace State{
	MainState::MainState()
	{
		m_ID = 1;
        m_nextGameState = m_ID;
	}

	uint32_t MainState::process()
	{
		m_world.process();
		if (m_nextGameState!=m_ID)
        {
            uint32_t tempGameState(m_nextGameState);
            m_nextGameState = m_ID;
            return tempGameState;
        }
		return m_nextGameState;
	}

	void MainState::processEvents(sf::Event& _event)
	{
		m_world.processEvent(_event);

		switch(_event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (_event.key.code == sf::Keyboard::Escape)
				m_nextGameState = 0;
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
			m_nextGameState = 100;
			break;
		}
		case sf::Event::Resized:
		{
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
		m_world.draw(_window);
		Graphic::g_lightSystem.draw(_window);
	}
}