#pragma once

#include <SFML\System\Vector2.hpp>
#include <math.h>

template<class T, class Compare>
const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
{
	return comp(v, hi) ? std::max(v, lo, comp) : std::min(v, hi, comp);
}

// vector stuff

template< typename _T>
float lenSq(const _T& _vec)
{
	return _vec.x*_vec.x + _vec.y*_vec.y;
}

template< typename _T>
float len(const _T& _vec)
{
	return sqrt(_vec.x*_vec.x + _vec.y*_vec.y);
}


template< typename _T>
float distSq(const _T& _first, const _T& _second)
{
	_T dir = _first - _second;
	return lenSq(dir);
}

template< typename _T>
float dist(const _T& _first, const _T& _second)
{
	return sqrt(distSq(_first, _second));
}

template< typename _T>
float dot(const sf::Vector2<_T>& _first, const sf::Vector2<_T>& _second)
{
	return _first.x * _second.x + _first.y * _second.y;
}

template< typename _T>
sf::Vector2<_T> normalize(const sf::Vector2<_T>& _vec)
{
	return _vec * 1.f / len(_vec);
}

template< typename _T>
float angle(const sf::Vector2<_T>& _first, const sf::Vector2<_T>& _second)
{
	return acos(dot(_first, _second) / (len(_first)+len(_second)));
}