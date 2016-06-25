#pragma once

#include "gamestate.hpp"
#include "world.hpp"

namespace State{
	class MainState: public Game::GameState
	{
	public:
		MainState();

		virtual void process() override;
        virtual void processEvents(sf::Event& _event);
		virtual void draw(sf::RenderWindow& _window) override;

	private:
		Game::World m_world;
	};
}