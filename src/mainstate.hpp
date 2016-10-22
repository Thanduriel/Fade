#pragma once

#include "gamestate.hpp"
#include "world.hpp"
#include "pawn.hpp"
#include "lightsys.hpp"

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
		int itemSpawns;

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
		void gameOver();
		void processGameOver();
		bool m_gameOver;
		int m_gameEndTime;
		Graphic::LightInfoHandle m_lightInfo;

		int m_gameTime; //in frames
		Game::World m_world;
		GameSettings m_gameSettings;

		sf::Sound m_gameEndSound;
	};
}