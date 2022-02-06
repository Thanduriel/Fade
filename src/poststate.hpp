#pragma once

#include <memory>
#include "gamestate.hpp"
#include "gui.hpp"
#include "pawn.hpp"

namespace State
{
	class PostState : public Game::GameState
	{
	public:
		PostState(const struct GameSettings& _gameSettings);

		virtual void process() override;
		virtual void processEvents(sf::Event& _event);
		virtual void draw(sf::RenderWindow& _window) override;

	private:
		sf::Font m_font;
		sf::Text m_title;
		std::vector < std::unique_ptr < GUI::GuiElement > > m_gui;
		std::vector < sf::Text > m_playerStatTexts;
		sf::Text m_headerText;
		std::vector < std::unique_ptr< Game::Pawn >> m_players;
	};

}