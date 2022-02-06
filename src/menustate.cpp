#include <iostream>

#include "menustate.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"
#include "mainstate.hpp"
#include "creditstate.hpp"
#include "optionstate.hpp"
#include "lobbystate.hpp"

namespace State{
	MenuState::MenuState(sf::RenderWindow& _window)
		: m_window(_window)
    {
        font = *g_resourceManager->getFont("suburbia");

		const uint32_t left = Constants::g_windowSizeX / 2 - 100;

        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(sf::Vector2f(left-200.f,50.f));
        title.setScale(sf::Vector2f(1.f, 1.f));
        title.setFillColor(sf::Color::White);

		//play
		m_gui.emplace_back(new GUI::GuiElement("Play", left - 100, 300, [&]()
		{
			m_newState = new State::LobbyState(m_window);
		}));
		//options
		m_gui.emplace_back(new GUI::GuiElement("Items", left - 100, 400, [&]()
		{
			m_newState = new State::OptionState(m_window);
		}));
		//controls
	/*	m_gui.emplace_back(new GUI::GuiElement("Items", left - 100, 500, [&]()
		{
		//	m_newState = new State::OptionState(m_window);
		}));*/

		//credits
		m_gui.emplace_back(new GUI::GuiElement("Credits", left - 100, 500, [&]()
		{
			m_newState = new State::CreditState();
		}));
		//quit
		m_gui.emplace_back(new GUI::GuiElement("Quit", left - 100, 600, [&]()
		{
			m_finished = true;
		}));
		m_gui.init();

        m_state = 0; // internal state
    }

	void MenuState::process()
    {
		m_gui.process();
    }

    void MenuState::processEvents(sf::Event& _event)
    {
		GameState::processEvents(_event);
		m_gui.processEvents(_event);
    }

    void MenuState::draw(sf::RenderWindow& _window)
    {
        _window.draw(title);
		m_gui.draw(_window);
    }
}