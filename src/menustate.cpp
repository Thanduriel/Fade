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
        font = *g_resourceManager.getFont("suburbia");

		uint32_t left = Constants::g_windowSizeX / 2 - 100;

        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(left-200,50);
        title.setScale(1., 1.);
        title.setColor(sf::Color::White);

		m_gui.emplace_back(new GUI::Gui("Play", left-100, 300));
		m_gui.emplace_back(new GUI::Gui("Options", left-100, 400));
		m_gui.emplace_back(new GUI::Gui("Credits", left-100, 500));
		m_gui.emplace_back(new GUI::Gui("Quit", left-100, 600));

        m_state = 0; // internal state
    }

	void MenuState::process()
    {
        m_gui[m_state]->focus();
        for (auto& gui : m_gui)
            gui->process();
    }

    void MenuState::processEvents(sf::Event& _event)
    {
        switch(_event.type)
        {
        case sf::Event::KeyPressed:
        {
            if (_event.key.code == sf::Keyboard::Escape)
				m_finished = true;
            else if (_event.key.code == sf::Keyboard::Up)
            {
                if (m_state > 0)
                {
                    m_gui[m_state]->unfocus();
                    m_state--;
                }
            }
            else if (_event.key.code == sf::Keyboard::Down)
            {
                if (m_state < (m_gui.size()-1))
                {
                    m_gui[m_state]->unfocus();
                    m_state++;
                }
            }
            else if (_event.key.code == sf::Keyboard::Return)
            {
                if (m_state == 0) // play
                {
					m_newState = new State::LobbyState(m_window);
                }
                else if (m_state == 1) // Options
                {
					m_newState = new State::OptionState(m_window);
                }
                else if (m_state == 2) // credits
                {
					m_newState = new State::CreditState();
                }
                else if (m_state == (m_gui.size()-1)) // quit
                {
					m_finished = true;
                }
            }

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
			m_finished = true;
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

    void MenuState::draw(sf::RenderWindow& _window)
    {
        _window.draw(title);
        for (auto& gui : m_gui)
            gui->draw(_window);
    }
}