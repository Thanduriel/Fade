#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"

namespace State{
    class MenuState: public Game::GameState
    {
    public:
        MenuState();

        virtual void process() override;
        virtual void processEvents(sf::Event& _event);
        virtual void draw(sf::RenderWindow& _window) override;

    private:
        sf::Font font;
        sf::Text title;
        std::vector < std::unique_ptr < GUI::Gui > > m_gui;
        uint m_state;
    };
}