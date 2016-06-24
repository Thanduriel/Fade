#pragma once
#include <SFML\Graphics.hpp>

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
		GameState() : bFinished(false), newState(States::None){}

		virtual void process() abstract;
		virtual void draw(sf::RenderWindow& _window) abstract;

		bool bFinished;
		States newState;
	};
}