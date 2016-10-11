#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"

namespace State{
	class LobbyState : public Game::GameState
	{
	public:
		//takes the active window to change its view
		LobbyState(sf::RenderWindow& _window);

		virtual void process() override;
		virtual void processEvents(sf::Event& _event);
		virtual void draw(sf::RenderWindow& _window) override;

	private:
		void rescaleView(uint32_t _size);

		//updates the displayed values
		void refreshGui();

		sf::RenderWindow& m_window;
		sf::Font font;
		sf::Text title;
		std::vector < std::unique_ptr < GUI::ExtGui > > m_gui;
		uint32_t m_state, m_size, m_walls, m_endCondition, m_endValue;
		std::vector < sf::String > m_worldSizes, m_nWalls;
	};
}