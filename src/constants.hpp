#pragma once

#include "config.hpp"

namespace Constants{
	// defined in main.cpp
	extern int g_windowSizeX;
	extern int g_windowSizeY;
	extern float g_worldScale;
	extern int g_numWalls;

	//game constants ----------------------------
	constexpr float c_scaleFactor = 0.4f; //globally applied to all sprites

	const int c_eventFrequency = CONFIG_SECTION("game").get("eventFrequency", 60 * 20); // in frames

	const float c_playerBaseSpeed = CONFIG_SECTION("game").get("playerBaseSpeed",3.3f);
	const float c_playerBaseFadeFactor = CONFIG_SECTION("game").get("playerBaseFadeFactor", 0.968f); //0.985f
	const float c_projectileBaseSpeed = CONFIG_SECTION("game").get("projectileBaseSpeed", 10.0f);
	const float c_projectileRadius = CONFIG_SECTION("game").get("projectileRadius", 5.f);
	const int c_fastReloadCount = CONFIG_SECTION("game").get("fastReloadCount", 7); // amount of ammo reloaded before it diminishes
	const int	c_baseReloadSpeed = CONFIG_SECTION("game").get("baseReloadSpeed",40); // time for one fast reload projectile in frames
	const float c_playerBaseDamage = CONFIG_SECTION("game").get("playerBaseDamage", 13.f);
	const float c_playerBaseHealth = CONFIG_SECTION("game").get("playerBaseHealth", 100.f);
	const float c_deathTreshhold = CONFIG_SECTION("game").get("deathThreshold", 0.05f);
	const int c_keepDeadBodies = CONFIG_SECTION("game").get("keepDeadBodies", 1);

	const int c_wallThickness = CONFIG_SECTION("game").get("wallThickness", 25);
	const int c_wallMaxLength = CONFIG_SECTION("game").get("wallMaxLength", 500);
	const int c_wallMinLength = CONFIG_SECTION("game").get("wallMinLength", 100);

	//item constants -----------------------------
	const int c_sentinelChargeTime = CONFIG_SECTION("items").get("sentinelChargeTime", 60 * 2);
	const int c_sentinelActiveTime = CONFIG_SECTION("items").get("sentinelActiveTime", 60 * 20);

	const int c_mineChargeTime = CONFIG_SECTION("items").get("mineChargeTime", 60 * 4);

	const int c_clusterGunActiveTime = CONFIG_SECTION("items").get("clusterGunActiveTime", 60 * 10);
	const float c_clusterGunDamage = CONFIG_SECTION("items").get("clusterGunDamage", 9.f);

	const int c_lightAuraActiveTime = CONFIG_SECTION("items").get("lightAuraActiveTime", 60 * 10);
	const int c_lightAuraExtraRadius = CONFIG_SECTION("items").get("lightAuraExtraRadius", 200);

	const int c_speedBonusActiveTime = CONFIG_SECTION("items").get("speedBonusActiveTime", 60 * 8);
	const float c_speedBonusExtraSpeed = CONFIG_SECTION("items").get("speedBonusExtraSpeed", 1.f);

	const int c_shieldActiveTime = CONFIG_SECTION("items").get("shieldActiveTime", 60 * 4);

}