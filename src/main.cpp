// yalasg.cpp : Defines the entry point fo{r the console application.
//
#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <iostream>

// #include "easylogging++.hpp"
#include "gamestate.hpp"
#include "mainstate.hpp"
#include "menustate.hpp"
#include "optionstate.hpp"
#include "creditstate.hpp"

// #define ELPP_NO_DEFAULT_LOG_FILE

// INITIALIZE_EASYLOGGINGPP

namespace Constants{
	int c_windowSizeX;
	int c_windowSizeY;
}

int main()
{
	using namespace Game;

	sf::VideoMode desktop = sf::VideoMode().getDesktopMode();
	Constants::c_windowSizeX = desktop.width;
	Constants::c_windowSizeY = desktop.height;
	Graphic::g_lightSystem.refreshSize();
	sf::RenderWindow window(desktop, "Fade", sf::Style::Default);

	sf::Clock clock;
	sf::Time elapsed;
	std::vector< std::unique_ptr< Game::GameState > > states;
	states.emplace_back(new State::MenuState());
	states.emplace_back(new State::MainState());
	states.emplace_back(new State::OptionState());
	states.emplace_back(new State::CreditState());
	uint32_t current_state(0);

	while (window.isOpen())
	{
		if (current_state > states.size())
		{
			window.close();
			break;
		}

		Game::GameState& state = *states[current_state];
		sf::Event event;
		while (window.pollEvent(event))
			state.processEvents(event);

		current_state = state.process();
		window.clear();
		state.draw(window);
		window.display();

		elapsed = clock.restart();
		if (elapsed.asMilliseconds() < 16.667)
			sf::sleep((sf::milliseconds(16.667) - elapsed));
	}

	return 0;
}



