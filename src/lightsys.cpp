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
		m_vertices.append(Vertex(Vector2f(0.f,0.f)));
		m_vertices.append(Vertex(Vector2f(Constants::c_windowSizeX, 0.f)));
		m_vertices.append(Vertex(Vector2f(Constants::c_windowSizeX, Constants::c_windowSizeY)));
		m_vertices.append(Vertex(Vector2f(0.f, Constants::c_windowSizeY)));
		m_offScreen.create(Constants::c_windowSizeX, Constants::c_windowSizeY);
	//	m_offScreen.

	}

	LightInfo& LightSystem::createLight()
	{
		m_lightInfos.emplace_back(new LightInfo());

		return *m_lightInfos.back();
	}

	void LightSystem::draw(sf::RenderWindow& _window)
	{
		m_offScreen.clear();

		sf::RenderStates states;
		Shader* shader = g_resourceManager.getShader(DefaultShader::Light);
		states.shader = shader;

		for (auto& lightInfo : m_lightInfos)
		{
			shader->setParameter("lightPosition", lightInfo->position);
			shader->setParameter("lightColor", lightInfo->color);
			m_offScreen.draw(m_vertices, states);
		}

		m_offScreen.display();
		sf::Sprite sprite(m_offScreen.getTexture());
	//	sprite.setColor(Color(255, 255, 255, 255));
		sf::RenderStates states2;
		states2.blendMode = sf::BlendMultiply;
		_window.draw(sprite, states2);
	}
}