#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"

namespace State{
	class CreditState: public Game::GameState
	{
	public:
		CreditState();

		virtual void process() override;
		virtual void processEvents(sf::Event& _event);
		virtual void draw(sf::RenderWindow& _window) override;

	private:
		sf::Font font;
		sf::Text title;
		std::vector < std::unique_ptr < GUI::GuiElement > > m_gui;
	};
}