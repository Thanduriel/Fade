#include <iostream>

#include "lightsys.hpp"
#include "constants.hpp"
#include "resourcemanager.hpp"

using namespace sf;

namespace Graphic{
	LightSystem g_lightSystem;

	void LightInfo::setPosition(const sf::Vector2f& _vec)
	{
		position.x = _vec.x;
		position.y = _vec.y;//(float)Constants::g_windowSizeY - _vec.y;
	}

	LightSystem::LightSystem():
		m_vertices(Points)
	{
	}

	LightInfo& LightSystem::createLight()
	{
		auto it = std::find_if(m_lightInfos.begin(), m_lightInfos.end(),
			[](const std::unique_ptr<LightInfo>& _info){return !_info->isInUse; });

		if (it != m_lightInfos.end())
		{
			(*it)->isInUse = true;
			return **it;
		}

		m_lightInfos.emplace_back(new LightInfo());

		return *m_lightInfos.back();
	}

	void LightSystem::draw(sf::RenderWindow& _window)
	{
		m_offScreen.clear();

		sf::RenderStates states;
		Shader* shader = g_resourceManager.getShader(DefaultShader::Light);
		states.shader = shader;

		//every light is encoded in one point vertex
		m_vertices.clear();

		for (int i = 0; i < m_lightInfos.size(); ++i)
		{
			LightInfo& lightInfo = *m_lightInfos[i];
			if (lightInfo.radius == 0.f) continue;

			sf::Vertex vertex;
			vertex.color = lightInfo.color;
			sf::Vector2i screenPos = _window.mapCoordsToPixel(lightInfo.position);
			vertex.position = Vector2f((float)screenPos.x, (float)screenPos.y);
			vertex.texCoords.x = lightInfo.radius / Constants::g_worldScale;
			m_vertices.append(vertex);
		}
		_window.setView(_window.getDefaultView());
		m_offScreen.draw(m_vertices, states);
		m_offScreen.display();
		sf::Sprite sprite(m_offScreen.getTexture());
		sf::RenderStates states2;
		states2.blendMode = sf::BlendMultiply;
		_window.draw(sprite, states2);
	}

	void LightSystem::refreshSize()
	{
		m_offScreen.create(Constants::g_windowSizeX, Constants::g_windowSizeY);

		Shader* shader = g_resourceManager.getShader(DefaultShader::Light);

		shader->setParameter("windowSizeX", Constants::g_windowSizeX);
		shader->setParameter("windowSizeY", Constants::g_windowSizeY);
	}

	void LightSystem::setWalls(sf::Glsl::Vec2* _arr, size_t _size)
	{
		//transform to gpu pixel coords
		for (int i = 0; i < _size; ++i)
		{
			_arr[i].y = Constants::g_windowSizeY - _arr[i].y;
		}
		Shader* shader = g_resourceManager.getShader(DefaultShader::Light);
		if(_size)shader->setUniformArray("wallInfos", _arr, _size);
		shader->setUniform("numWalls_x3", (int)_size);
	}
}