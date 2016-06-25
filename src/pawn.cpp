#include "pawn.hpp"
#include "resourcemanager.hpp"

namespace Game{
	Pawn::Pawn(const sf::Vector2f& _position, sf::Texture& _texture) :
		Actor(_position, _texture),
		m_weaponSprite(*g_resourceManager.getTexture("player_inner_halfring.png"))
	{
		m_isStatic = false;

		m_weaponSprite.setOrigin(sf::Vector2f(0.f, m_weaponSprite.getTextureRect().height * 0.5f));//_texture.getSize().x

		m_sprite.setScale(0.4f, 0.4f);
		m_weaponSprite.setScale(0.4f, 0.4f);

		m_weaponSprite.setColor(sf::Color(128, 240, 0, 255));

		//radius of the final scaled sprite
		m_boundingRad = (float)m_sprite.getTexture()->getSize().x * m_sprite.getScale().x * 0.5f;
	}

	void Pawn::process()
	{
		m_position += m_velocity;
		m_dirAngle++;
	}

	void Pawn::draw(sf::RenderWindow& _window)
	{
		Actor::draw(_window);
		m_weaponSprite.setPosition(m_position);
		m_weaponSprite.setRotation(m_dirAngle);
		_window.draw(m_weaponSprite);
	}

	void Pawn::collision(Actor& _oth)
	{
		_oth.damage(1.f);
	}
}