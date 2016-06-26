#include <iostream>

#include "creditstate.hpp"
#include "resourcemanager.hpp"

namespace State{
    CreditState::CreditState()
    {
        font = *g_resourceManager.getFont("suburbia");

        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(400,50);
        title.setScale(1., 1.);
        title.setColor(sf::Color::White);

        m_gui.emplace_back(new GUI::Gui("Created by", 100, 300));
        m_gui.emplace_back(new GUI::Gui("Thanduriel and jokiel", 300, 400));
        m_gui.emplace_back(new GUI::Gui("During the", 100, 500));
        m_gui.emplace_back(new GUI::Gui("Acagamics GAMEJAM 2016", 300, 600));
        m_gui[1]->focus();
        m_gui[3]->focus();
        m_ID = 3;
        m_nextGameState = m_ID;
    }

    uint32_t CreditState::process()
    {
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

    void CreditState::processEvents(sf::Event& _event)
    {
        switch(_event.type)
        {
        case sf::Event::KeyPressed:
        {
            if (_event.key.code == sf::Keyboard::Escape)
                m_nextGameState = 0;
            else if (_event.key.code == sf::Keyboard::Return)
                m_nextGameState = 0;

            break;
        }
        case sf::Event::Closed:
        {
            m_nextGameState = 100;
            break;
        }
        // Default case
        default:
        {
            break;
        }
        }// switch(sf_event.type)
    }

    void CreditState::draw(sf::RenderWindow& _window)
    {
        _window.draw(title);
        for (auto& gui : m_gui)
            gui->draw(_window);
    }
}