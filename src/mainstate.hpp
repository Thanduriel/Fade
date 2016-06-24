#pragma once

#include "GameState.hpp"

namespace State{
	class MainState: public Game::GameState
	{
	public:
		MainState();

		virtual void process() override;
		virtual void draw(sf::RenderWindow& _window) override;

	private:
	};
}