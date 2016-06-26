#include <iostream>

#include "menustate.hpp"
#include "resourcemanager.hpp"

namespace State{
    MenuState::MenuState()
    {
        font = *g_resourceManager.getFont("suburbia");


        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(400,50);
        title.setScale(1., 1.);
        title.setColor(sf::Color::White);

        m_gui.emplace_back(new GUI::Gui("Play", 400, 300));
        m_gui.emplace_back(new GUI::Gui("Options", 400, 400));
        m_gui.emplace_back(new GUI::Gui("Credits", 400, 500));
        m_gui.emplace_back(new GUI::Gui("Quit", 400, 600));

        m_state = 0; // internal state
        m_ID = 0;
        m_nextGameState = m_ID;
    }

    uint32_t MenuState::process()
    {
        m_gui[m_state]->focus();
        for (auto& gui : m_gui)
            gui->process();
        if (m_nextGameState!=m_ID)
        {
            uint32_t tempGameState(m_nextGameState);
            m_nextGameState = m_ID;
            return tempGameState;
        }
        return m_nextGameState;
    }

    void MenuState::processEvents(sf::Event& _event)
    {
        switch(_event.type)
        {
        case sf::Event::KeyPressed:
        {
            if (_event.key.code == sf::Keyboard::Escape)
                m_nextGameState = 100;
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
                    m_nextGameState = 1;
                }
                else if (m_state == 1) // Options
                {
                    m_nextGameState = 2;
                }
                else if (m_state == 2) // credits
                {
                    m_nextGameState = 3;
                }
                else if (m_state == (m_gui.size()-1)) // quit
                {
                    m_nextGameState = 100;
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
            m_nextGameState = 100;
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