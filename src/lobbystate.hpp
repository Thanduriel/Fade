#pragma once

#include <memory>

#include "gamestate.hpp"
#include "gui.hpp"
#include "pawn.hpp"

namespace State{
	typedef std::vector < std::unique_ptr< Game::Pawn >> PlayerContainer;

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
		void refreshGuiElement();

		void switchPlayer(int _cid);
		static sf::Vector2f calcScreenPos(int _id)
		{
			return sf::Vector2f(100.f, _id * 120.f + 100.f);
		}

		sf::RenderWindow& m_window;
		sf::Font font;
		sf::Text title;
		GUI::Gui m_gui;
		uint32_t m_state, m_size, m_walls, m_endCondition, m_endValue;
		std::vector < sf::String > m_worldSizes, m_nWalls;
		PlayerContainer m_connectedPlayers;
		std::vector < sf::Text > m_connectedPlayerNames;
		sf::Text m_controlsText;
	};
}