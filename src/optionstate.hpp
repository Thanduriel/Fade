#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"

namespace State{
    class OptionState: public Game::GameState
    {
    public:
        OptionState();

        virtual uint32_t process() override;
        virtual void processEvents(sf::Event& _event);
        virtual void draw(sf::RenderWindow& _window) override;

    private:
        sf::Font font;
        sf::Text title;
        std::vector < std::unique_ptr < GUI::Gui > > m_gui;
        uint32_t m_state, m_size, m_walls;
        std::vector < sf::String > m_worldSizes, m_nWalls;
    };
}