#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"

namespace State{
    class OptionState: public Game::GameState
    {
    public:
		//takes the active window to change its view/ resolution
        OptionState(sf::RenderWindow& _window);

        virtual uint32_t process() override;
        virtual void processEvents(sf::Event& _event);
        virtual void draw(sf::RenderWindow& _window) override;

    private:
		void rescaleView(uint32_t _size);

		sf::RenderWindow& m_window;
        sf::Font font;
        sf::Text title;
        std::vector < std::unique_ptr < GUI::Gui > > m_gui;
        uint32_t m_state, m_size, m_walls;
        std::vector < sf::String > m_worldSizes, m_nWalls;
    };
}