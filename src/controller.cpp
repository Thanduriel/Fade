#include "controller.hpp"
#include "math.hpp"
#include "constants.hpp"

#include <iostream>
using namespace sf;

namespace Game{

	void PlayerController::process()
	{
		if (!m_pawn) return;

		if (m_pawn->isDeath())
		{
			if (m_pawn->alphaVal() < 0.2f)
				m_pawn = nullptr;
			return;
		}

		sf::Vector2f movDir(0.f, 0.f);

		if (sf::Joystick::isConnected(m_id))
		{
			Vector2f jDir;
			jDir.x = sf::Joystick::getAxisPosition(m_id, sf::Joystick::X);
			jDir.y = sf::Joystick::getAxisPosition(m_id, sf::Joystick::Y);

			Vector2f lookDir;
			lookDir.x = sf::Joystick::getAxisPosition(m_id, sf::Joystick::U);
			lookDir.y = sf::Joystick::getAxisPosition(m_id, sf::Joystick::R);

			m_pawn->setDirAngle(atan2(lookDir.y, lookDir.x));

			if (abs(jDir.x) > 40.f || abs(jDir.y) > 40.f)
				movDir = normalize(jDir);
		}
		m_pawn->setVelocity(Constants::c_playerBaseSpeed * movDir * m_pawn->speedFactor());
	}

	void PlayerController::processEvent(sf::Event &_event)
	{
		if (!m_pawn) return;

		if (_event.joystickButton.joystickId == m_id)
			{
			if (_event.joystickButton.button == 0)
				m_pawn->fire();
			else if (_event.joystickButton.button == 1)
				m_pawn->altFire();
			}
	}

}