#include "controller.hpp"
#include "math.hpp"
#include "constants.hpp"
#include "utils.hpp"

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
			if (_event.joystickButton.button == 5)
				m_pawn->fire();
			else if (_event.joystickButton.button == 4)
				m_pawn->altFire();
			}
	}

	// ******************************************************** //

	void AiController::process()
	{
		if (!m_pawn) return;
		if (m_pawn->isDeath())
		{
			if (m_pawn->alphaVal() < 0.2f)
				m_pawn = nullptr;
			return;
		}

		static int counter = 0;
		++counter;
		if (!(counter % 90)){
			Vector2f dir = Vector2f((float)util::rand(2) - 1.f, (float)util::rand(2) - 1.f);
			if (dir.x + dir.y != 0.f) dir = normalize(dir);
			m_pawn->setVelocity(Constants::c_playerBaseSpeed * dir * m_pawn->speedFactor());
		}
		if(!(counter % 20))m_pawn->fire();
	}
}