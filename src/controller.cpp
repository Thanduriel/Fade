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

		if (m_pawn->isDead())
		{
			if (m_pawn->alphaVal() < Constants::c_deathTreshhold)
			{
				m_pawn->destroy();
				m_pawn = nullptr;
			}
			return;
		}

		sf::Vector2f movDir(0.f, 0.f);
		m_fireCd--;

		if (sf::Joystick::isConnected(m_id))
		{
			if (m_inputs->isKeyPressed(Input::Action::Fire) && m_fireCd <= 0)
			{
				m_pawn->fire();
				m_fireCd = 12;
			}

			const Vector2f jDir(m_inputs->getAxis(Input::Axis::MoveLeftRight),
				m_inputs->getAxis(Input::Axis::MoveUpDown));

			const Vector2f lookDir(m_inputs->getAxis(Input::Axis::FireLeftRight),
				m_inputs->getAxis(Input::Axis::FireUpDown));

			m_pawn->setDirAngle(std::atan2(lookDir.y, lookDir.x));

			if (abs(jDir.x) > 0.4f || abs(jDir.y) > 0.4f)
				movDir = normalize(jDir);
		}
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_fireCd <= 0)
			{
				m_pawn->fire();
				m_fireCd = 12;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movDir.y -= 1.f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movDir.y += 1.f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movDir.x -= 1.f;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movDir.x += 1.f;
		}
		m_pawn->setVelocity(m_pawn->maxSpeed() * movDir);
	}

	void PlayerController::processEvent(sf::Event& _event)
	{
		if (!m_pawn) return;

		if (_event.joystickButton.joystickId == m_id)
			{
		//	if (_event.joystickButton.button == 5)
		//		m_pawn->fire();
			if (m_inputs->isKeyPressed(Input::Action::AltFire))
				m_pawn->altFire();
			else if (m_inputs->isKeyPressed(Input::Action::SwitchLight))
				m_pawn->setLightState(static_cast<Pawn::LightState>(
					(static_cast<int>(m_pawn->lightState())+1) % static_cast<int>(Pawn::LightState::Count)));
			}
	}

	// ******************************************************** //

	void AiController::process()
	{
		if (!m_pawn) return;
		if (m_pawn->isDead())
		{
			if (m_pawn->alphaVal() < 0.2f)
				m_pawn = nullptr;
			return;
		}

		static int counter = 0;
		++counter;
		if (!(counter % 90)){
			Vector2f dir = Vector2f((float)util::rand(2) - 1.f, (float)util::rand(2) - 1.f);
			if (dir.x != 0.f || dir.y != 0.f) dir = normalize(dir);
			m_pawn->setVelocity(Constants::c_playerBaseSpeed * dir * m_pawn->speedFactor());
		}
		if (!(counter % 20)) m_pawn->fire();
	}
}