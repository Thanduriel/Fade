#pragma once

#include "gamestate.hpp"
#include "world.hpp"

namespace State{

	enum class WinCondition
	{
		Time,
		Kills,
		Count
	};

	struct GameSettings
	{
		WinCondition winCondition;
		int value;
	};

	class MainState: public Game::GameState
	{
	public:
		MainState(const GameSettings& _gameSettings);

		virtual void process() override;
        virtual void processEvents(sf::Event& _event);
		virtual void draw(sf::RenderWindow& _window) override;
		virtual void onActivate() override;
	private:
		int m_gameTime; //in frames
		Game::World m_world;
		GameSettings m_gameSettings;
	};
}