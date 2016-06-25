#pragma once
#include <SFML/Graphics.hpp>

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

		virtual void process() = 0;
		virtual void processEvents(sf::Event& _event) = 0;
		virtual void draw(sf::RenderWindow& _window) = 0;

		bool bFinished;
		States newState;
	};
}