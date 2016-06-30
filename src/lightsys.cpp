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
		position.y = _vec.y;//(float)Constants::c_windowSizeY - _vec.y;
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

		shader->setParameter("windowSizeX", Constants::c_windowSizeX);
		shader->setParameter("windowSizeY", Constants::c_windowSizeY);

		m_vertices.resize(m_lightInfos.size());

		for (int i = 0; i < m_lightInfos.size(); ++i)
		{
			LightInfo& lightInfo = *m_lightInfos[i];
			if (lightInfo.radius == 0.f) continue;

			m_vertices[i].color = lightInfo.color;
			m_vertices[i].position = lightInfo.position;
			m_vertices[i].texCoords.x = lightInfo.radius;
		}
		m_offScreen.draw(m_vertices, states);
	//	std::cout << drawcalls << std::endl;
		m_offScreen.display();
		sf::Sprite sprite(m_offScreen.getTexture());
	//	sprite.setColor(Color(255, 255, 255, 255));
		sf::RenderStates states2;
		states2.blendMode = sf::BlendMultiply;
		_window.draw(sprite, states2);
	}

	void LightSystem::refreshSize()
	{
		m_offScreen.create(Constants::c_windowSizeX, Constants::c_windowSizeY);
	}
}