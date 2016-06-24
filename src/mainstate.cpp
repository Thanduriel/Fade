#include "mainstate.hpp"
#include <iostream>

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
	}
}