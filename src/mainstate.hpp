#pragma once

#include "gamestate.hpp"
#include "world.hpp"
#include "pawn.hpp"

namespace State{

	enum class WinCondition
	{
		Time,
		Kills,
		Count
	};

	struct PlayerInfo
	{
		PlayerInfo(int _cid, Game::PlayerColor _color) : cid(_cid), playerColor(_color){}
		int cid;
		Game::PlayerColor playerColor;
	};

	struct GameSettings
	{
		WinCondition winCondition;
		int value;

		std::vector<PlayerInfo> playerInfos;
	};

	class MainState: public Game::GameState
	{
	public:
		MainState(const GameSettings& _gameSettings,
			const std::vector < std::unique_ptr< Game::Pawn >>& _players);

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