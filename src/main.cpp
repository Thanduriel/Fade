// yalasg.cpp : Defines the entry point fo{r the console application.
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include <Windows.h>
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
#include "stats.hpp"

#include <sstream>

namespace Constants{
	int g_windowSizeX;
	int g_windowSizeY;
	float g_worldScale;
	int g_numWalls;
}

namespace Stats{
	StatManager g_statManager;
}

void ErrorMsg(std::string error, std::string title)
{
	MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
}

int main()
{
	using namespace Game;
	using namespace Constants;

	g_resourceManager = std::make_unique<ResourceManager>();
	sf::Music* track = g_resourceManager->getTrack("menu");
	track->play();

	sf::VideoMode desktop = sf::VideoMode().getDesktopMode();

	if (desktop.width < 1920 || desktop.height < 1080) 
		ErrorMsg("Warning: Resolutions bellow 1920x1080 are not fully supported.", "");
	else
	{
		desktop.width = 1920;
		desktop.height = 1080;
	}
	Constants::g_windowSizeX = desktop.width;
	Constants::g_windowSizeY = desktop.height;

#ifndef NDEBUG
	sf::RenderWindow window(desktop, "Fade", sf::Style::Default);
#else
	sf::RenderWindow window(desktop, "Fade", sf::Style::Fullscreen);
#endif
	window.setMouseCursorVisible(false);
	Graphic::g_lightSystem = std::make_unique<Graphic::LightSystem>();
	Graphic::g_lightSystem->refreshSize();

	sf::Clock clock, frameTimeClock;
	sf::Time elapsed;
	std::vector<std::unique_ptr< Game::GameState >> states;
	states.emplace_back(new State::MenuState(window));

	while (window.isOpen())
	{
		using namespace Game;

		//only measure the relevant time
		clock.restart();
		if (track->getStatus() == sf::Music::Status::Playing && states.size() > 2)
			track->stop();
		else if (track->getStatus() == sf::Music::Status::Stopped && states.size() < 2)
			track->play();

		Game::GameState& state = *states.back();
		sf::Event event;
		while (window.pollEvent(event))
			state.processEvents(event);

		state.process();
		window.clear();
		state.draw(window);
		window.display();

		//handle state changes
		GameState* newState = state.fetchNewState();
		//pop state afterwards to allow exchanging one state for a new one
		if (state.isFinished())
		{
			states.pop_back();
			if (states.size()) 
				states.back()->onActivate();
			else
				window.close();
		}

		if (newState)
		{
			states.emplace_back(newState);
			newState->onActivate();
		}

		elapsed = clock.getElapsedTime();

		//temporary stat display
		std::stringstream s;
		for (int i = 0; i < 4; ++i){
			s << i << ": " << Stats::g_statManager.Get(i, Stats::Kills) << "/" << Stats::g_statManager.Get(i, Stats::Deaths) << "   ";
		}
		//window.setTitle(s.str());

		//counts fps
		static int c = 0;
		static int lowFrames = 0;
		static bool messageShown = false;
		++c;
		if (frameTimeClock.getElapsedTime().asMicroseconds() >= 1000000) 
		{
			window.setTitle(std::to_string(c));
			
			if (c < 50)
			{
				lowFrames++;
				if (lowFrames > 6 && !messageShown)
				{
					ErrorMsg("Warning: Low frame rate detected. Try a larger map or fewer walls to improve performance.", "");
					messageShown = true;
				}
			}
			else lowFrames = 0;

			frameTimeClock.restart();
			c = 0;
		}
		//	if (elapsed.asMilliseconds() > 16.667) window.setTitle(std::to_string(elapsed.asMilliseconds()));//std::cout << elapsed.asMilliseconds() << std::endl;
		if (elapsed.asMicroseconds() < 16667)
			sf::sleep((sf::microseconds(16667) - elapsed));
	}
	Graphic::g_lightSystem.reset(nullptr);
	g_resourceManager.reset(nullptr);
	track = nullptr;

	return 0;
}



