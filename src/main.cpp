// yalasg.cpp : Defines the entry point fo{r the console application.
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include <vector>
#include <memory>
#include <iostream>

// #include "easylogging++.hpp"
#include "gamestate.hpp"
#include "mainstate.hpp"
#include "menustate.hpp"
#include "optionstate.hpp"
#include "creditstate.hpp"
#include "resourcemanager.hpp"

// #define ELPP_NO_DEFAULT_LOG_FILE

// INITIALIZE_EASYLOGGINGPP

namespace Constants{
	int g_windowSizeX;
	int g_windowSizeY;
	float g_worldScale;
	int g_numWalls;
}

int main()
{
	using namespace Game;

	sf::Music *track;
	track = g_resourceManager.getTrack("menu");
    track->play();

	sf::VideoMode desktop = sf::VideoMode().getDesktopMode();
	Constants::g_windowSizeX = desktop.width;
	Constants::g_windowSizeY = desktop.height;
	Graphic::g_lightSystem.refreshSize();
	sf::RenderWindow window(desktop, "Fade", sf::Style::Default);
	window.setMouseCursorVisible(false);

	sf::Clock clock;
	sf::Time elapsed;
	std::vector< std::unique_ptr< Game::GameState > > states;
	states.emplace_back(new State::MenuState());
	states.emplace_back(new State::MainState());
	states.emplace_back(new State::OptionState(window));
	states.emplace_back(new State::CreditState());
	uint32_t current_state(0);

	while (window.isOpen())
	{
		if (current_state > states.size())
			window.close();
		else
		{
			if (track->getStatus() == sf::Music::Status::Playing && current_state==1)
				track->stop();
			else if (track->getStatus() == sf::Music::Status::Stopped && current_state!=1)
				track->play();
			Game::GameState& state = *states[current_state];
			sf::Event event;
			while (window.pollEvent(event))
				state.processEvents(event);

			current_state = state.process();
			if (current_state != 100 && &state != states[current_state].get()) 
				states[current_state]->onActivate();
			window.clear();
			state.draw(window);
			window.display();

			elapsed = clock.restart();
			if (elapsed.asMilliseconds() > 16.667) window.setTitle(std::to_string(elapsed.asMilliseconds()));//std::cout << elapsed.asMilliseconds() << std::endl;
			if (elapsed.asMilliseconds() < 16.667)
				sf::sleep((sf::milliseconds(16.667) - elapsed));
		}
	}
	track = NULL;

	return 0;
}



