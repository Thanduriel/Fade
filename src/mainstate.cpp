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

	void MainState::draw(sf::RenderWindow& _window)
	{
		m_world.draw(_window);
		Graphic::g_lightSystem.draw(_window);
	}
}