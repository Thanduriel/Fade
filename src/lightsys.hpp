#pragma once


#include <SFML/Graphics.hpp>
#include <memory>

namespace Graphic{

	struct LightInfo
	{
		LightInfo() : isInUse(false){}
		void setPosition(const sf::Vector2f& _vec);
		sf::Color color;
		sf::Vector2f position;
		float radius;

	private:
		friend class LightInfoHandle;
		friend class LightSystem;
		void destroy() { radius = 0.f; isInUse = false; }
		bool isInUse;
	};

	class LightInfoHandle
	{
	public:
		LightInfoHandle() : m_lightInfo(nullptr){}
		//can only be moved
		LightInfoHandle(LightInfoHandle& _info) = delete;
		LightInfoHandle& operator=(LightInfoHandle& _info) = delete;
		LightInfoHandle& operator=(LightInfoHandle&& _info)
		{
			m_lightInfo = _info.m_lightInfo;
			_info.m_lightInfo = nullptr;
			return *this;
		}

		// 
		LightInfoHandle(LightInfo& _info);
		~LightInfoHandle();

		// stops usage of this light
		void release();

		//access
		LightInfo* get() { return m_lightInfo; }
		LightInfo* operator->();
	private:
		LightInfo* m_lightInfo;
	};

	class LightSystem
	{
	public:
		LightSystem();

		LightInfoHandle createLight();
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