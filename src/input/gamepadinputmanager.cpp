#include "gamepadinputmanager.hpp"

namespace Input
{
/*	const ConfigSection::Initializer<GamePadButton, 6> Gamepad1Buttons(
		{ {
			{"Jump", GamePadButton::RB},
			{"JumpAlt", GamePadButton::LB},
			{"AccelerateCW", GamePadButton::NONE},
			{"AccelerateCCW", GamePadButton::NONE},
			{"RotateCW", GamePadButton::NONE},
			{"RotateCCW", GamePadButton::NONE}
		} });

	const ConfigSection::Initializer<sf::Joystick::Axis, 2> Gamepad1Axis(
		{ {
			{"Accelerate", sf::Joystick::Axis::U},
			{"Rotate", sf::Joystick::Axis::X},
		} });*/

	GamePadInputInterface::GamePadInputInterface(ConfigSection& _config, unsigned _id)
		: m_inputMap({
			{Action::Fire, _config.get("Fire", GamePadButton::RB)},
			{Action::AltFire, _config.get("AltFire", GamePadButton::LB)},
			{Action::SwitchLight, _config.get("SwitchLight", GamePadButton::B)} }),
		m_axisMap(
			{_config.get("MoveLeftRight", sf::Joystick::Axis::X),
			_config.get("MoveUpDown", sf::Joystick::Axis::Y),
			_config.get("FireLeftRight", sf::Joystick::Axis::U),
			_config.get("FireUpDown", sf::Joystick::Axis::V)}),
		m_id(_id)
	{
	}

	bool GamePadInputInterface::isKeyPressed(Action _action) const
	{
		const GamePadButton button = m_inputMap.getKey(_action);
		return button == GamePadButton::NONE ? false
			: sf::Joystick::isButtonPressed(m_id, static_cast<unsigned>(button));
	}

	float GamePadInputInterface::getAxis(Axis _axis) const
	{
		sf::Joystick::Axis axis = m_axisMap[static_cast<size_t>(_axis)];

		float pos = 0.f;
		if(static_cast<int>(axis) != -1)
			pos = sf::Joystick::getAxisPosition(m_id, axis) * 0.01f;

	/*	const AxisAction aa = AXIS_ACTIONS[static_cast<size_t>(_axis)];
		if (IsKeyPressed(aa.low))
			pos -= 1.f;
		if (IsKeyPressed(aa.high))
			pos += 1.f;*/
		return pos;
	}
}

std::istream& operator >> (std::istream& _in, Input::GamePadButton& _key)
{
	int i;
	_in >> i;
	_key = static_cast<Input::GamePadButton>(i);

	return _in;
}

std::istream& operator >> (std::istream& _in, sf::Joystick::Axis& _axis)
{
	int i = -1;
	_in >> i;
	_axis = static_cast<sf::Joystick::Axis>(i);

	return _in;
}