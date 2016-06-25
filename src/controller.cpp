#include "controller.hpp"
#include "math.hpp"
#include <iostream>
using namespace sf;

namespace Game{

	void PlayerController::process()
	{
		if (!m_pawn) return;

		sf::Vector2f movDir(0.f, 0.f);
		//movement

	//	m_pawn->setVelocity(movDir * 2.f);

		if (sf::Joystick::isConnected(m_id))
		{
			Vector2f jDir;
			jDir.x = sf::Joystick::getAxisPosition(m_id, sf::Joystick::X);
			jDir.y = sf::Joystick::getAxisPosition(m_id, sf::Joystick::Y);
			
			Vector2f lookDir;
			lookDir.x = sf::Joystick::getAxisPosition(m_id, sf::Joystick::U);
			lookDir.y = sf::Joystick::getAxisPosition(m_id, sf::Joystick::R);
	
			m_pawn->setDirAngle(atan2(lookDir.y, lookDir.x) / (2.f*3.1415f) * 360.f);

			if (abs(jDir.x) > 40.f || abs(jDir.y) > 40.f)
				movDir = jDir * 0.1f;
		} 
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				movDir += Vector2f(0.f, -1.f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				movDir += Vector2f(0.f, 1.f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				movDir += Vector2f(-1.f, 0.f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				movDir += Vector2f(1.f, 0.f);
			}
		}

		m_pawn->setVelocity(movDir);
	}

}