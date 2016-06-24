#pragma once

#include <SFML/Graphics.hpp>

namespace Graphic{
	class CenteredSprite : public sf::Sprite
	{
	public:
		CenteredSprite(sf::Texture& _texture):
			sf::Sprite(_texture)
		{
			setOrigin(sf::Vector2f(_texture.getSize()) * 0.5f);
		}
	};
}