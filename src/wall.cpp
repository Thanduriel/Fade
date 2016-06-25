#include "wall.hpp"
#include "resourcemanager.hpp"
#include <algorithm>
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

		m_boundingRad = std::max(_size.x, _size.y) * 0.5f;
		m_bbBegin = _pos - m_sprite.getOrigin() * m_sprite.getScale().x;
		m_bbEnd = m_bbBegin + _size;
	}

	Vector2f solveLine(Vector2f v, Vector2f w, Vector2f p, float _rad) {
		// Return minimum distance between line segment vw and point p
		float l2 = lenSq(v - w);  // i.e. |w-v|^2 -  avoid a sqrt
		// Consider the line extending the segment, parameterized as v + t (w - v).
		// We find projection of point p onto the line. 
		// It falls where t = [(p-v) . (w-v)] / |w-v|^2
		// We clamp t from [0,1] to handle points outside the segment vw.
		float t = std::max(0.f, std::min(1.f, dot(p - v, w - v) / l2));
		Vector2f projection = v + t * (w - v);  // Projection falls on the segment

		if (dist(p, projection) < _rad)
		{
			return projection + (projection - p);
		}
	}

	void Wall::collision(Actor& _oth)
	{
		Vector2f newPos = _oth.position();
		const Vector2f& curPos = _oth.position();
		Vector2f prevPos = _oth.position() - _oth.velocity();
		float rad = _oth.boundingRad();

		if (prevPos.x + rad < m_bbBegin.x && curPos.x + rad > m_bbBegin.x)
			newPos.x = m_bbBegin.x - _oth.boundingRad() - 1.f;

		_oth.setPosition(newPos);
	}
}