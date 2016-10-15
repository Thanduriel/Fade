#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"

namespace State{
    class MenuState: public Game::GameState
    {
    public:
		MenuState(sf::RenderWindow& _window);

        virtual void process() override;
        virtual void processEvents(sf::Event& _event);
        virtual void draw(sf::RenderWindow& _window) override;

    private:
        sf::Font font;
        sf::Text title;
       GUI::Gui m_gui;
        uint32_t m_state;
		sf::RenderWindow& m_window;
    };
}