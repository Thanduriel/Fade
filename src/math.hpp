#pragma once

#include <SFML\System\Vector2.hpp>

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
float normalize(const sf::Vector2<_T>& _vec)
{
	return _vec * 1.f / len(_vec);
}