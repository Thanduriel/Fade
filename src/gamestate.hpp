#pragma once
#include <SFML/Graphics.hpp>
#include <stdint.h>

namespace Game
{
	enum class States : int
	{
		None = 0,	// no changes
		Pop,		// pop the current state
		NewState,	// everything > NewState should be a makable state 
		Main,		// push new state
		Menu,
		Pause,
		Options,
		Credits
	};

	class GameState
	{
	public:
		GameState() : m_newState(nullptr), m_finished(false){ onActivate(); }

		//
		virtual void process() = 0;
		virtual void processEvents(sf::Event& _event) = 0;
		virtual void draw(sf::RenderWindow& _window) = 0;
		virtual void onActivate() {};

		bool isFinished() const { return m_finished; }
		GameState* fetchNewState() { GameState* p = m_newState; m_newState = nullptr; return p; }
	protected:
		bool m_finished;

		GameState* m_newState;
	};
}