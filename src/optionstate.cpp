#include <iostream>

#include "optionstate.hpp"
#include "resourcemanager.hpp"
#include "constants.hpp"
#include "camera.hpp"

namespace Graphic{
	sf::View g_camera;
}

namespace State{
	OptionState::OptionState(sf::RenderWindow& _window) :
		m_window(_window)
    {
        font = *g_resourceManager.getFont("suburbia");

        uint32_t left = Constants::g_windowSizeX / 2 - 200;

        title.setFont(font);
        title.setCharacterSize(200);
        title.setString("FADE");
        title.setPosition(left - 100,50);
        title.setScale(1., 1.);
        title.setColor(sf::Color::White);

        m_gui.emplace_back(new GUI::Gui("World: Small", left-100, 300));
        m_gui.emplace_back(new GUI::Gui("Walls:", left-100, 400));
        m_gui.emplace_back(new GUI::Gui("Back", left-100, 500));

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

        m_gui[0]->setText(sf::String("World: ") + m_worldSizes[m_size]);
        m_gui[1]->setText(sf::String("Walls: ") + m_nWalls[m_walls]);
        m_state = 0;
        m_ID = 2;
        m_nextGameState = m_ID;
    }

    uint32_t OptionState::process()
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

    void OptionState::processEvents(sf::Event& _event)
    {
        switch(_event.type)
        {
        case sf::Event::KeyPressed:
        {
            if (_event.key.code == sf::Keyboard::Escape)
                m_nextGameState = 0;
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
                if (m_state == 0) // change worldSize
                {
                    if (++m_size == m_worldSizes.size())
                        m_size = 0;
					rescaleView(m_size);
                }
                else if (m_state == 1) // change nWalls
                {
                    if (++m_walls == m_nWalls.size())
                        m_walls = 0;
					Constants::g_numWalls = m_walls * 3;
                }
                else if (m_state == 2) // back to main menu
                {
                    m_nextGameState = 0;
                }
            }
            m_gui[0]->setText(sf::String("World: ") + m_worldSizes[m_size]);
            m_gui[1]->setText(sf::String("Walls: ") + m_nWalls[m_walls]);

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

    void OptionState::draw(sf::RenderWindow& _window)
    {
        _window.draw(title);
        for (auto& gui : m_gui)
            gui->draw(_window);
    }

	// *********************************************** //

	void OptionState::rescaleView(uint32_t _size)
	{
		float scale = (float)(_size+1) * 0.5f;
		Constants::g_worldScale = scale;
		sf::View view = m_window.getDefaultView();
		view.zoom(scale);
		Graphic::g_camera = view;
	}
}