#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include "actions.hpp"

namespace Input
{
	class InputInterface
	{
	public:
		virtual ~InputInterface() {};

		virtual bool isKeyPressed(Action _action) const = 0;
		// Axis values range from [-1,1].
		virtual float getAxis(Axis _axis) const = 0;
	};

	/* ******************************
	 * 
	 */
	template<typename T, T Default>
	class InputMap
	{
	public:
		InputMap(std::initializer_list<std::pair<const Action, T>> _init)
			: m_keyMap(_init)
		{}

		T getKey(Action _action) const 
		{
			auto it = m_keyMap.find(_action);

			return it != m_keyMap.end() ? it->second : Default;
		}

	private:
		std::unordered_map<Action, T> m_keyMap;
	};
}