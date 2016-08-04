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

	void Wall::lineCollision(const Vector2f& v, const Vector2f& w, Actor& _act)
	{
		const Vector2f& p = _act.position();
		// Return minimum distance between line segment vw and point p
		float l2 = lenSq(v - w);  // i.e. |w-v|^2 -  avoid a sqrt
		// Consider the line extending the segment, parameterized as v + t (w - v).
		// We find projection of point p onto the line. 
		// It falls where t = [(p-v) . (w-v)] / |w-v|^2
		// We clamp t from [0,1] to handle points outside the segment vw.
		float t = std::max(0.f, std::min(1.f, dot(p - v, w - v) / l2));
		Vector2f projection = v + t * (w - v);  // Projection falls on the segment
		if (distSq(projection, p) < _act.boundingRad()*_act.boundingRad()) 
			_act.setPosition(projection - normalize(projection - p) * _act.boundingRad());
		//return distance(p, projection);
	}


	void Wall::collision(Actor& _oth)
	{
		lineCollision(m_bbBegin, Vector2f(m_bbBegin.x, m_bbEnd.y), _oth);
		lineCollision(m_bbBegin, Vector2f(m_bbEnd.x, m_bbBegin.y), _oth);
		lineCollision(m_bbEnd, Vector2f(m_bbBegin.x, m_bbEnd.y), _oth);
		lineCollision(m_bbEnd, Vector2f(m_bbEnd.x, m_bbBegin.y), _oth);
	}
}