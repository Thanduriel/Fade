#pragma once

#include <array>

namespace Input
{
	enum struct Axis
	{
		MoveLeftRight,
		MoveUpDown,
		FireLeftRight,
		FireUpDown,
		COUNT
	};


	enum struct Action
	{
		None,

		Fire,
		AltFire,
		SwitchLight,

		COUNT
	};

	// Map axis to pairs of actions on keyboards.
	struct AxisAction
	{
		Action low;
		Action high;
	};

/*	constexpr std::array<AxisAction, static_cast<size_t>(Axis::COUNT)> AXIS_ACTIONS =
	{ {
		{Action::RotateCW, Action::RotateCCW},
		{Action::AccelerateCW, Action::AccelerateCCW}
	} };*/
}
