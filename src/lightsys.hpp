#pragma once


#include <SFML/Graphics.hpp>
#include <memory>

namespace Graphic{

	struct LightInfo
	{
		void setPosition(const sf::Vector2f& _vec);
		void destroy() { radius = 0.f; isInUse = false; }
		sf::Color color;
		sf::Vector2f position;
		float radius;
		bool isInUse;
	};

	class LightSystem
	{
	public:
		LightSystem();

		LightInfo& createLight();
		void draw(sf::RenderWindow& _window);
		void refreshSize();

		//transforms the coordinates and pushes wall information to the gpu
		void setWalls(sf::Glsl::Vec2* _arr, size_t _size, float _worldToScreenRatio = 1.f);
	private:
		sf::RenderTexture m_offScreen;
		std::vector<std::unique_ptr<LightInfo>> m_lightInfos;
		sf::VertexArray m_vertices;
	};

	extern LightSystem g_lightSystem;
}