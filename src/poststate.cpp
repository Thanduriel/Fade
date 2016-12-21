#include "poststate.hpp"
#include "constants.hpp"
#include "resourcemanager.hpp"
#include "stats.hpp"
#include "mainstate.hpp"
#include "constants.hpp"

using namespace Stats;

namespace State{
	PostState::PostState(const GameSettings& _gameSettings)
	{
		m_font = *g_resourceManager.getFont("suburbia");

		uint32_t left = Constants::g_windowSizeX / 2 - 600;

		m_title.setFont(m_font);
		m_title.setCharacterSize(200);
		m_title.setString("game over");
		m_title.setPosition(left, 50);
		m_title.setScale(1., 1.);
		m_title.setColor(sf::Color::White);

		sf::Vector2f basePos = sf::Vector2f(Constants::g_windowSizeX * 0.5f - (_gameSettings.playerInfos.size()-1) * 200.f * 0.5f, 350.f);

		m_headerText.setFont(m_font);
		m_headerText.setPosition(basePos + sf::Vector2f(-430.f, 80.f));
		m_headerText.setScale(1.f, 1.f);
		m_headerText.setColor(sf::Color::White);
		m_headerText.setCharacterSize(50);
		m_headerText.setString("\nKills: \nDeaths: \nShots fired: \naccuracy: \nitems used: ");


		m_playerStatTexts.resize(_gameSettings.playerInfos.size());
		for (size_t i = 0; i < m_playerStatTexts.size(); ++i)
		{
			int id = _gameSettings.playerInfos[i].cid;
			sf::Vector2f pos(i * 200.f, 0.f);
			pos += basePos;
			m_players.emplace_back(std::make_unique<Game::Pawn>(pos,
				*g_resourceManager.getTexture("player_main.png"), 0));
			m_players.back()->switchColor(_gameSettings.playerInfos[i].playerColor);

			int shotsFired = g_statManager.Get(id, Stat::ShotsFired);

			static std::string tab = "   ";

			m_playerStatTexts[i].setFont(m_font);
			m_playerStatTexts[i].setPosition(pos + sf::Vector2f(-75.f, 80.f));
			m_playerStatTexts[i].setScale(1.f, 1.f);
			m_playerStatTexts[i].setColor(sf::Color::White);
			m_playerStatTexts[i].setCharacterSize(50);
			m_playerStatTexts[i].setString(tab + std::to_string(i) + "\n"
				+ tab + std::to_string(g_statManager.Get(id, Stat::Kills))
				+ "\n" + tab
				+ std::to_string(g_statManager.Get(id, Stat::Deaths))
				+ "\n" + tab + std::to_string(shotsFired)
				+ "\n" + tab + std::to_string((int)(100.f * g_statManager.Get(id, Stat::ShotsHit) / (float)(shotsFired ? shotsFired : 1)))
				+ "\n" + tab + std::to_string(g_statManager.Get(id, Stat::ItemsUsed))
				+ (g_statManager.Get(id, Stat::Kills) == g_statManager.getMax(Stat::Kills) ?
				"\n\nWINNER!" : ""));

			if (g_statManager.Get(id, Stat::Kills) == g_statManager.getMax(Stat::Kills))
				m_playerStatTexts[i].setColor(sf::Color::Green);
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
		case sf::Event::JoystickButtonPressed:
			if (_event.joystickButton.joystickId == 0)
				m_finished = true;
			break;
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
		_window.draw(m_headerText);
		for (auto& gui : m_gui)
			gui->draw(_window);

		for (auto& pawn : m_players) pawn->draw(_window);
		for (auto& text : m_playerStatTexts) _window.draw(text);
	}
}