#include "poststate.hpp"
#include "constants.hpp"
#include "resourcemanager.hpp"
#include "stats.hpp"

using namespace Stats;

namespace State{
	PostState::PostState()
	{
		m_font = *g_resourceManager.getFont("suburbia");

		uint32_t left = Constants::g_windowSizeX / 2 - 600;

		m_title.setFont(m_font);
		m_title.setCharacterSize(200);
		m_title.setString("game over");
		m_title.setPosition(left, 50);
		m_title.setScale(1., 1.);
		m_title.setColor(sf::Color::White);

		m_playerStatTexts.resize(g_statManager.getPlayerCount());
		for (size_t i = 0; i < m_playerStatTexts.size(); ++i)
		{
			m_playerStatTexts[i].setFont(m_font);
			m_playerStatTexts[i].setPosition(sf::Vector2f(200.f + i * 200.f, 400.f));
			m_playerStatTexts[i].setScale(1.f, 1.f);
			m_playerStatTexts[i].setColor(sf::Color::White);
		//	m_playerStatTexts[i].setCharacterSize(30)
			m_playerStatTexts[i].setString(std::to_string(i) + " : " 
				+ std::to_string(g_statManager.Get(i, Stat::Kills))
				+ std::to_string(g_statManager.Get(i, Stat::Deaths)));
		}
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

		for (auto& text : m_playerStatTexts) _window.draw(text);
	}
}