// yalasg.cpp : Defines the entry point for the console application.
//
#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

#include "gamestate.hpp"
#include "mainstate.hpp"

int main()
{
	using namespace Game;

	sf::RenderWindow window(sf::VideoMode(1366, 786), "Fade");
//	window.setMouseCursorVisible(false);
//	window.setView(Graphics::g_camera);

	sf::Clock clock;

	std::vector< std::unique_ptr< Game::GameState > > states;
	states.emplace_back(new State::MainState());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time elapsed = clock.restart();
		if (elapsed.asMilliseconds() < 16.667) _sleep(unsigned long(16.667 - elapsed.asMilliseconds()));

		Game::GameState& state = *states.back();
		state.process();

		window.clear();
		state.draw(window);
		window.display();

		if (state.bFinished) states.pop_back();
		if (state.newState != Game::States::None)
		{
			GameState* newState;
			switch (state.newState)
			{
			case States::Main: newState = new State::MainState(); break;
			}
			states.emplace_back(newState);
		}
	}

	return 0;
}

