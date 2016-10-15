#include <iostream>

#include "creditstate.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"

namespace State{
    CreditState::CreditState()
    {
        font = *g_resourceManager.getFont("suburbia");

         uint32_t left = Constants::g_windowSizeX / 2 - 200;

        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(left-100,50);
        title.setScale(1., 1.);
        title.setColor(sf::Color::White);

        m_gui.emplace_back(new GUI::GuiElement("Created by", left-300, 300));
        m_gui.emplace_back(new GUI::GuiElement("Thanduriel and jokiel", left-100, 400));
        m_gui.emplace_back(new GUI::GuiElement("During the", left-300, 500));
        m_gui.emplace_back(new GUI::GuiElement("Acagamics GAMEJAM 2016", left-100, 600));
        m_gui.emplace_back(new GUI::GuiElement("Music by", left-300, 700));
        m_gui.emplace_back(new GUI::GuiElement("Streako", left-100, 800));
        m_gui[1]->focus();
        m_gui[3]->focus();
        m_gui[5]->focus();
    }

    void CreditState::process()
    {
        for (auto& gui : m_gui)
            gui->process();
    }

    void CreditState::processEvents(sf::Event& _event)
    {
        switch(_event.type)
        {
        case sf::Event::KeyPressed:
		case sf::Event::JoystickButtonPressed:
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

    void CreditState::draw(sf::RenderWindow& _window)
    {
        _window.draw(title);
        for (auto& gui : m_gui)
            gui->draw(_window);
    }
}