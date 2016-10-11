#include "poststate.hpp"
#include "constants.hpp"
#include "resourcemanager.hpp"

namespace State{
	PostState::PostState()
	{
		m_font = *g_resourceManager.getFont("suburbia");

		uint32_t left = Constants::g_windowSizeX / 2 - 200;

		m_title.setFont(m_font);
		m_title.setCharacterSize(200);
		m_title.setString("game over");
		m_title.setPosition(left - 100, 50);
		m_title.setScale(1., 1.);
		m_title.setColor(sf::Color::White);
	}

	void PostState::process()
	{
		for (auto& gui : m_gui)
			gui->process();
	}

	void PostState::processEvents(sf::Event& _event)
	{
		switch (_event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (_event.key.code == sf::Keyboard::Escape)
				m_finished = true;
			else if (_event.key.code == sf::Keyboard::Return)
				m_finished = true;

			break;
		}
		case sf::Event::Closed:
		{
			m_finished = true;
			break;
		}
		// Default case
		default:
		{
			break;
		}
		}// switch(sf_event.type)
	}

	void PostState::draw(sf::RenderWindow& _window)
	{
		_window.draw(m_title);
		for (auto& gui : m_gui)
			gui->draw(_window);
	}
}