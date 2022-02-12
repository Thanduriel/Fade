#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "gamestate.hpp"
#include "mainstate.hpp"
#include "menustate.hpp"
#include "optionstate.hpp"
#include "creditstate.hpp"
#include "resourcemanager.hpp"
#include "stats.hpp"
#include "config.hpp"

#include <sstream>
#include <vector>
#include <memory>
#include <iostream>

#ifdef _MSC_VER
#include <Windows.h>
#endif

// CRT's memory leak detection
#ifndef NDEBUG 
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif

namespace Constants {
	int g_windowSizeX;
	int g_windowSizeY;
	float g_worldScale;
	int g_numWalls;
}

namespace Stats{
	std::unique_ptr<StatManager> g_statManager;
}

void ErrorMsg(const std::string& error, const std::string& title)
{
#ifdef _MSC_VER
	MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
#endif
	std::cerr << "[" << title << "] " << error << "\n";
}

int main()
{
#ifndef NDEBUG 
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetBreakAlloc(14473);
#endif
#endif
	using namespace Game;
	using namespace Constants;

	g_resourceManager = std::make_unique<ResourceManager>();
	sf::Music* track = g_resourceManager->getTrack("menu");
	track->play();

	sf::VideoMode desktop = sf::VideoMode().getDesktopMode();

	if (desktop.width < 1920 || desktop.height < 1080) 
		ErrorMsg("Warning: Resolutions below 1920x1080 are not fully supported.", "");
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
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	Graphic::g_lightSystem = std::make_unique<Graphic::LightSystem>();
	Graphic::g_lightSystem->refreshSize();
	Stats::g_statManager = std::make_unique<Stats::StatManager>();

	sf::Clock clock, frameTimeClock;
	sf::Time elapsed;
	std::vector<std::unique_ptr< Game::GameState >> states;
	states.emplace_back(new State::MenuState(window));

	//counts fps
	int c = 0;
	int lowFrames = 0;
	bool messageShown = false;

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
	}
	states.clear();
	Graphic::g_lightSystem.reset(nullptr);
	g_resourceManager.reset(nullptr);
	Stats::g_statManager.reset(nullptr);
	track = nullptr;
	Config::instance().save();

	return 0;
}