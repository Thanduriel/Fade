#pragma once
#include <SFML/Graphics.hpp>
#include <stdint.h>

namespace Game
{
	enum class States
	{
		None = 0,
		Main,
		Menu,
		Pause,
		Inventory
	};

	class GameState
	{
	public:
		GameState() : newState(States::None){}

		virtual uint32_t process() = 0;
		virtual void processEvents(sf::Event& _event) = 0;
		virtual void draw(sf::RenderWindow& _window) = 0;

		uint32_t m_nextGameState, m_ID;
		States newState;
	};
}