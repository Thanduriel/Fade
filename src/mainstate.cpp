#include <iostream>

#include "mainstate.hpp"
#include "lightsys.hpp"

namespace State{
	MainState::MainState()
	{
	}

	void MainState::process()
	{
		m_world.process();
	}

	void MainState::processEvents(sf::Event& _event)
	{
		m_world.processEvent(_event);

		switch(_event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (_event.key.code == sf::Keyboard::Escape)
				bFinished = true;
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
			bFinished = true;
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