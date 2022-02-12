#pragma once

#include <SFML/Window/Joystick.hpp>
#include "actions.hpp"
#include "inputmanager.hpp"
#include "../config.hpp"

namespace Input
{
	enum GamePadButton
	{ 
		A, B, X, Y, LB, RB, Select, Start, COUNT, 
		NONE = -1
	};

	class GamePadInputInterface : public InputInterface
	{
	public:
		GamePadInputInterface(ConfigSection& _config, unsigned _id);

		bool isKeyPressed(Action _action) const override;
		float getAxis(Axis _axis) const override;

	private:
		InputMap<GamePadButton, GamePadButton::NONE> m_inputMap;
		std::array<sf::Joystick::Axis, static_cast<size_t>(Axis::COUNT)> m_axisMap;
		unsigned m_id;
	};

}

// allow reading of keys from a config
std::istream& operator >> (std::istream& _in, Input::GamePadButton& _key);
std::istream& operator >> (std::istream& _in, sf::Joystick::Axis& _axis);
