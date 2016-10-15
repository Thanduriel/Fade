#include <iostream>
#include <array>

#include "lobbystate.hpp"
#include "mainstate.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"
#include "camera.hpp"


//possible choices for the corresponding settings
const std::array<std::string, 2> ENDCONDITIONS =
{
	"time", "kills"
};

const std::array<std::array<int, 4>, 2> ENDVALUE =
{
	2, 5, 7, 10,
	1, 3, 5, 11
};
enum Button
{
	WorldSize,
	NumWalls,
	EndCondition,
	EndValue,
	Play,
	Back,
	//last entry
	Count
};

using namespace sf;

namespace State{
	LobbyState::LobbyState(sf::RenderWindow& _window) :
		m_window(_window),
		m_endCondition(0),
		m_endValue(0)
	{
		font = *g_resourceManager.getFont("suburbia");

		uint32_t left = Constants::g_windowSizeX / 2 - 450;
		Vector2f rightOff = Vector2f(600.f, 0.f);

		title.setFont(font);
		title.setCharacterSize(200);
		title.setString("FADE");
		title.setPosition(left + 150, 50);
		title.setScale(1., 1.);
		title.setColor(sf::Color::White);

		m_gui.reserve(Button::Count);

		//size
		m_gui.emplace_back(new GUI::ExtGuiElement("World:", left, 300, [&]()
		{
			if (++m_size == m_worldSizes.size())
				m_size = 0;
			rescaleView(m_size);
		}, rightOff));
		//walls
		m_gui.emplace_back(new GUI::ExtGuiElement("Walls:", left, 400, [&]()
		{
			if (++m_walls == m_nWalls.size())
				m_walls = 0;
			Constants::g_numWalls = m_walls * 3;
		}, rightOff));
		//end condition
		m_gui.emplace_back(new GUI::ExtGuiElement("End Condition:	", left, 500, [&]()
		{
			if (++m_endCondition == ENDCONDITIONS.size())
				m_endCondition = 0;
		}, rightOff));
		//value for win
		m_gui.emplace_back(new GUI::ExtGuiElement("Amount: ", left, 600, [&]()
		{
			if (++m_endValue == ENDVALUE[m_endCondition].size())
				m_endValue = 0;
		}, rightOff));
		//start game
		m_gui.emplace_back(new GUI::ExtGuiElement("start game", left + 150, 750, [&]()
		{
			GameSettings config;
			config.winCondition = m_endCondition ? WinCondition::Kills : WinCondition::Time;
			config.value = ENDVALUE[m_endCondition][m_endValue] * (m_endCondition ? 1 : 60);
			m_newState = new State::MainState(config);
		}));
		//back
		m_gui.emplace_back(new GUI::ExtGuiElement("Back", left + 150, 850, [&]()
		{
			m_finished = true;
		}));
		m_gui.init();

		m_worldSizes.push_back("Small");
		m_worldSizes.push_back("Medium");
		m_worldSizes.push_back("Large");
		m_worldSizes.push_back("Auto");
		m_size = 1; //medium seems to be the appropriate default
		rescaleView(1);

		m_nWalls.push_back("None");
		m_nWalls.push_back("Few");
		m_nWalls.push_back("Many");
		m_nWalls.push_back("Random");
		m_walls = 0;

		m_state = 0;

		refreshGuiElement();
	}

	void LobbyState::process()
	{
		m_gui.process();
	}

	void LobbyState::processEvents(sf::Event& _event)
	{
		GameState::processEvents(_event);
		m_gui.processEvents(_event);

		refreshGuiElement();
	}

	void LobbyState::draw(sf::RenderWindow& _window)
	{
		_window.draw(title);
		m_gui.draw(_window);
	}

	// *********************************************** //

	void LobbyState::rescaleView(uint32_t _size)
	{
		float scale = (float)(_size + 1) * 0.5f;
		Constants::g_worldScale = scale;
		sf::View view = m_window.getDefaultView();
		view.zoom(scale);
		Graphic::g_camera = view;
	}
	
	// *********************************************** //

	void LobbyState::refreshGuiElement()
	{
		reinterpret_cast<GUI::ExtGuiElement*>(m_gui[WorldSize].get())->setText2(m_worldSizes[m_size]);
		reinterpret_cast<GUI::ExtGuiElement*>(m_gui[NumWalls].get())->setText2(m_nWalls[m_walls]);
		reinterpret_cast<GUI::ExtGuiElement*>(m_gui[EndCondition].get())->setText2(ENDCONDITIONS[m_endCondition]);
		reinterpret_cast<GUI::ExtGuiElement*>(m_gui[EndValue].get())->setText2(std::to_string(ENDVALUE[m_endCondition][m_endValue])
			+ (m_endCondition ? "" : "min"));
	}
}