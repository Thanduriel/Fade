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
		position.y = (float)Constants::c_windowSizeY - _vec.y;
	}

	LightSystem::LightSystem():
		m_vertices(Quads)
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

		int drawcalls = 0;
		for (auto& lightInfo : m_lightInfos)
		{
			if (lightInfo->radius == 0.f) continue;
			drawcalls++;
			shader->setParameter("lightPosition", lightInfo->position);
			shader->setParameter("lightColor", lightInfo->color);
			shader->setParameter("lightRadius", lightInfo->radius);

			float lightY = (float)Constants::c_windowSizeY - lightInfo->position.y;

			m_vertices[0].position = Vector2f(lightInfo->position.x - lightInfo->radius,
				lightY + lightInfo->radius);
			m_vertices[1].position = Vector2f(lightInfo->position.x - lightInfo->radius,
				lightY - lightInfo->radius);
			m_vertices[2].position = Vector2f(lightInfo->position.x + lightInfo->radius,
				lightY - lightInfo->radius);
			m_vertices[3].position = Vector2f(lightInfo->position.x + lightInfo->radius,
				lightY + lightInfo->radius);
			m_offScreen.draw(m_vertices, states);
		}
		std::cout << drawcalls << std::endl;
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

		m_vertices.append(Vertex(Vector2f(0.f, 0.f)));
		m_vertices.append(Vertex(Vector2f(Constants::c_windowSizeX, 0.f)));
		m_vertices.append(Vertex(Vector2f(Constants::c_windowSizeX, Constants::c_windowSizeY)));
		m_vertices.append(Vertex(Vector2f(0.f, Constants::c_windowSizeY)));
	}
}