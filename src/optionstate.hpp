#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"
#include "item.hpp"

namespace State{
    class OptionState: public Game::GameState
    {
    public:
		//takes the active window to change its view/ resolution
        OptionState(sf::RenderWindow& _window);

        virtual void process() override;
        virtual void processEvents(sf::Event& _event);
        virtual void draw(sf::RenderWindow& _window) override;

    private:
		//updates the displayed values
		void refreshGuiElement();

		sf::RenderWindow& m_window;
        sf::Font font;
        sf::Text title;
        GUI::Gui m_gui;
		std::vector < std::unique_ptr < Game::Item > > m_items;
		std::vector < sf::Text > m_itemTexts;
    };
}