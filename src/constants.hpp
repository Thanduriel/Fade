#pragma once

namespace Constants{
	//defined in main.cpp
	extern int g_windowSizeX;
	extern int g_windowSizeY;
	extern float g_worldScale;
	extern int g_numWalls;

	//game constants
	const float c_scaleFactor = 0.4f; //globally applied to all sprites

	const int c_eventFrequency = 60 * 20; // in frames

	const float c_playerBaseSpeed = 2.f;
	const float c_projectileBaseSpeed = 4.f;
	const float c_projectileRadius = 5.f;
	const float c_fastReloadCount = 5.f; // amount of ammo reloaded before it diminishes; only int values make sence

	const int c_wallThickness = 25;
	const int c_wallMaxLength = 400;
}