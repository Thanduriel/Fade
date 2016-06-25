// yalasg.cpp : Defines the entry point for the console application.
//
#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <iostream>

// #include "easylogging++.hpp"
#include "gamestate.hpp"
#include "mainstate.hpp"

// #define ELPP_NO_DEFAULT_LOG_FILE

// INITIALIZE_EASYLOGGINGPP

int main()
{
	using namespace Game;

	sf::RenderWindow window(sf::VideoMode(1366, 786), "Fade");

	sf::Clock clock;
	sf::Time elapsed;
	std::vector< std::unique_ptr< Game::GameState > > states;
	states.emplace_back(new State::MainState());

	while (window.isOpen())
	{
		Game::GameState& state = *states.back();
		sf::Event event;
		while (window.pollEvent(event))
			state.processEvents(event);

		elapsed = clock.restart();
		if (elapsed.asMilliseconds() < 16.667)
			sf::sleep((sf::milliseconds(16.667) - elapsed));

		state.process();

		window.clear();
		state.draw(window);
		window.display();

		if (state.bFinished)
		{
			states.pop_back();
			if (states.size()==0)
				window.close();
		}
		// if (state.newState != Game::States::None)
		// {
		// 	GameState* newState;
		// 	switch (state.newState)
		// 	{
		// 	case States::Main: newState = new State::MainState(); break;
		// 	}
		// 	states.emplace_back(newState);
		// }
	}

	return 0;
}

