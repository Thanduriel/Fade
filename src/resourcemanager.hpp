#pragma once

#include "SFML\Graphics.hpp"
#include <string>
#include <vector>
#include <memory>

enum DefaultShader
{
	Lighting
};

template< typename _T>
struct Resource
{
	Resource(const std::string& _name, _T* _data) : name(_name), data(_data){}
	Resource(Resource&& _oth) :
		name(std::move(_oth.name)), data(std::move(_oth.data)){}
	std::string name;
	std::unique_ptr< _T > data;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	sf::Texture* getTexture(const std::string& _fileName);
	sf::Font* getFont(const std::string& _fileName);
	sf::Shader* getShader(const std::string& _fileName, bool _geometry = false);

	sf::Shader* getShader(DefaultShader _shader) { return m_shaders[_shader].data.get();  };
private:
	std::vector < Resource< sf::Texture > > m_textures;
	std::vector < Resource< sf::Font > > m_fonts;
	std::vector < Resource< sf::Shader > > m_shaders;
};


extern ResourceManager g_resourceManager;
