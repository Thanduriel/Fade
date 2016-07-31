#include <algorithm>

#include "wall.hpp"
#include "resourcemanager.hpp"
#include "math.hpp"

using namespace sf;

namespace Game{

	Wall::Wall(const sf::Vector2f& _pos, const sf::Vector2f& _size):
		Actor(_pos, *g_resourceManager.getTexture("wall.png"))
	{
		sf::Vector2f texSize;
		texSize.x = (float)m_sprite.getTexture()->getSize().x;
		texSize.y = (float)m_sprite.getTexture()->getSize().y;

		m_sprite.setScale(sf::Vector2f(_size.x / texSize.x, _size.y / texSize.y));

		m_boundingRad = sqrt(_size.x * _size.x + _size.y * _size.y) * 0.57f;
		m_bbBegin = _pos;
		m_bbBegin.x -= m_sprite.getOrigin().x * m_sprite.getScale().x;
		m_bbBegin.y -= m_sprite.getOrigin().y * m_sprite.getScale().y;
		m_bbEnd = m_bbBegin + _size;
	}


	void Wall::collision(Actor& _oth)
	{
		Vector2f newPos = _oth.position();
		const Vector2f& curPos = _oth.position();
		Vector2f prevPos = _oth.position() - _oth.velocity();
		float rad = _oth.boundingRad();
		float radSq = rad * rad;

		//edges

		if (prevPos.x + rad < m_bbBegin.x && curPos.x + rad > m_bbBegin.x)
			newPos.x = m_bbBegin.x - _oth.boundingRad() - 1.f;

		if (prevPos.x - rad > m_bbEnd.x && curPos.x - rad < m_bbEnd.x)
			newPos.x = m_bbEnd.x + _oth.boundingRad() + 1.f;

		if (prevPos.y + rad < m_bbBegin.y && curPos.y + rad > m_bbBegin.y)
			newPos.y = m_bbBegin.y - _oth.boundingRad() - 1.f;

		if (prevPos.y - rad > m_bbEnd.y && curPos.y - rad < m_bbEnd.y)
			newPos.y = m_bbEnd.y + _oth.boundingRad() + 1.f;

		//corners need special treatment because players are circles

		float d = radSq - distSq(newPos, m_bbBegin);
		if (d > 0) newPos += normalize(newPos - m_bbEnd) * sqrt(d);

		Vector2f corner(m_bbBegin.x, m_bbEnd.y);
		d = radSq - distSq(newPos, corner);
		if (d > 0) newPos += normalize(newPos - corner) * sqrt(d);

		corner = Vector2f(m_bbEnd.x, m_bbBegin.y);
		d = radSq - distSq(newPos, corner);
		if (d > 0) newPos += normalize(newPos - corner) * sqrt(d);

		d = radSq - distSq(newPos, m_bbEnd);
		if (d > 0) newPos += normalize(newPos - m_bbEnd) * sqrt(d);

		_oth.setPosition(newPos);
	}
}