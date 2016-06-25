#include "controller.hpp"

using namespace sf;

namespace Game{

	void Controller::process()
	{
		if (!m_pawn) return;

		sf::Vector2f movDir(0.f, 0.f);
		//movement
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

		m_pawn->setVelocity(movDir * 2.f);
	}

}