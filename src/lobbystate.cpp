#include <iostream>
#include <array>

#include "lobbystate.hpp"
#include "mainstate.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "controller.hpp"


//possible choices for the corresponding settings

const std::array<std::string, 4> ITEMSPAWN =
{
	"0", "1..2", "2..4", "3..6"
};

const std::array<std::string, 2> ENDCONDITIONS =
{
	"time", "kills"
};

const std::array<std::array<int, 4>, 2> ENDVALUE =
{
	1, 2, 3, 5,
	1, 3, 5, 11
};
enum Button
{
	WorldSize,
	NumWalls,
	ItemSpawn,
	EndCondition,
	EndValue,
	Play,
	Back,
	//last entry
	Count
};

using namespace sf;
using namespace Constants;

namespace State{
	LobbyState::LobbyState(sf::RenderWindow& _window) :
		m_window(_window),
		m_endCondition(0),
		m_endValue(0),
		m_itemSpawn(2)
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

		m_controlsText.setFont(font);
		m_controlsText.setCharacterSize(50);
		m_controlsText.setString("B         : switch light \nrback : use item \nlback : shoot");
		m_controlsText.setPosition(g_windowSizeX - 590, g_windowSizeY - 200);
		m_controlsText.setScale(1., 1.);
		m_controlsText.setFillColor(sf::Color(150,150,150));

		m_lobbyControlsText.setFont(font);
		m_lobbyControlsText.setCharacterSize(50);
		m_lobbyControlsText.setString("start : join game\nX         : change color");
		m_lobbyControlsText.setPosition(40, g_windowSizeY - 200 + 50);
		m_lobbyControlsText.setScale(1., 1.);
		m_lobbyControlsText.setFillColor(sf::Color(150, 150, 150));

		m_gui.reserve(Button::Count);

		//size
		m_gui.emplace_back(new GUI::ExtGuiElement("World:", left, 280, [&]()
		{
			if (++m_size == m_worldSizes.size())
				m_size = 0;
			rescaleView(m_size);
		}, rightOff));
		//walls
		m_gui.emplace_back(new GUI::ExtGuiElement("Walls:", left, 360, [&]()
		{
			if (++m_walls == m_nWalls.size())
				m_walls = 0;
			Constants::g_numWalls = m_walls * 3;
		}, rightOff));
		//item spawns
		m_gui.emplace_back(new GUI::ExtGuiElement("Item spawns:", left, 440, [&]()
		{
			if (++m_itemSpawn == ITEMSPAWN.size())
				m_itemSpawn = 0;
		}, rightOff));
		//end condition
		m_gui.emplace_back(new GUI::ExtGuiElement("End Condition:	", left, 520, [&]()
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
			config.itemSpawns = m_itemSpawn;

			for (auto& player : m_connectedPlayers)
				if(player.get()) config.playerInfos.emplace_back(player->getCId(), player->getPlayerColor());
			m_newState = new State::MainState(config, m_connectedPlayers);
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
		m_nWalls.push_back("Some");
		m_nWalls.push_back("Many");
		m_walls = 0;

		m_state = 0;

		m_connectedPlayers.resize(8);
		m_connectedPlayerNames.resize(8);
		for (size_t i = 0; i < m_connectedPlayerNames.size(); ++i)
		{
			if (sf::Joystick::isConnected(i)) switchPlayer(i);

			m_connectedPlayerNames[i].setString(std::to_string(i));
			m_connectedPlayerNames[i].setPosition(calcScreenPos(i) + sf::Vector2f(60.f, -38.f));
			m_connectedPlayerNames[i].setScale(1.f, 1.f);
			m_connectedPlayerNames[i].setFillColor(sf::Color::Cyan);
			m_connectedPlayerNames[i].setFont(font);
			m_connectedPlayerNames[i].setCharacterSize(76);
		}
		refreshGuiElement();

		//test
		for (int i = 1; i < 3; ++i) switchPlayer(i);
	}

	void LobbyState::process()
	{
		m_gui.process();
	}

	void LobbyState::processEvents(sf::Event& _event)
	{
		GameState::processEvents(_event);
		m_gui.processEvents(_event);

		if( _event.type == sf::Event::JoystickConnected)
		{
			switchPlayer(_event.joystickConnect.joystickId);
		}
		else if( _event.type == sf::Event::JoystickDisconnected)
		{
			switchPlayer(_event.joystickConnect.joystickId);
		}
		if (_event.type == sf::Event::JoystickButtonPressed)
		{
			if (_event.joystickButton.button == 7) switchPlayer(_event.joystickButton.joystickId);
			else if (_event.joystickButton.button == 2 && m_connectedPlayers[_event.joystickButton.joystickId].get())
			{
				m_connectedPlayers[_event.joystickButton.joystickId]->switchColor();
			}
		}

		refreshGuiElement();
	}

	void LobbyState::draw(sf::RenderWindow& _window)
	{
		_window.draw(title);
		m_gui.draw(_window);
		_window.draw(m_controlsText);
		_window.draw(m_lobbyControlsText);

		for (size_t i = 0; i < m_connectedPlayers.size(); ++i)
		{
			if (!m_connectedPlayers[i].get()) continue;

			m_connectedPlayers[i]->draw(_window);
			_window.draw(m_connectedPlayerNames[i]);
		}
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
		static_cast<GUI::ExtGuiElement*>(m_gui[WorldSize].get())->setText2(m_worldSizes[m_size]);
		static_cast<GUI::ExtGuiElement*>(m_gui[NumWalls].get())->setText2(m_nWalls[m_walls]);
		static_cast<GUI::ExtGuiElement*>(m_gui[ItemSpawn].get())->setText2(ITEMSPAWN[m_itemSpawn]);
		static_cast<GUI::ExtGuiElement*>(m_gui[EndCondition].get())->setText2(ENDCONDITIONS[m_endCondition]);
		static_cast<GUI::ExtGuiElement*>(m_gui[EndValue].get())->setText2(std::to_string(ENDVALUE[m_endCondition][m_endValue])
			+ (m_endCondition ? "" : "min"));
	}

	// *********************************************** //

	void LobbyState::switchPlayer(int _cid)
	{
		if (m_connectedPlayers[_cid].get())
		{
			auto ptr = m_connectedPlayers[_cid].release();
			delete ptr;
		}
		else
		{
			m_connectedPlayers[_cid] = std::make_unique<Game::Pawn>(calcScreenPos(_cid),
				*g_resourceManager.getTexture("player_main.png"), _cid);
		}
	}
}