#include "pawn.hpp"

namespace Game{
	Pawn::Pawn(const sf::Vector2f& _position, sf::Texture& _texture) :
		Actor(_position, _texture)
	{

	}

	void Pawn::process()
	{
		m_position += m_velocity;
	}
}